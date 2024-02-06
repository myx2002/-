import os
from typing import Dict
import torch
import torch.optim as optim
from tqdm import tqdm
from torch.utils.data import DataLoader
from torchvision import transforms
from torchvision.datasets import CIFAR10, LSUN
from torchvision.utils import save_image
from Diffusion import GaussianDiffusionSampler, GaussianDiffusionTrainer, DDIMSampler, PLMSSampler
from Diffusion.Model import UNet
from Scheduler import GradualWarmupScheduler
from torch.utils.data import Dataset
from Blur import BlurTrainer, BlurSampler
from Animorph import AnimorphTrainer, AnimorphSampler
import numpy as np
import time


def cycle(dl):
    while True:
        for data, label in dl:
            yield data


class CustomDataset(Dataset):
    def __init__(self, dataset, indices):
        self.dataset = dataset
        self.indices = [int(i) for i in indices]

    def __len__(self):
        return len(self.indices)

    def __getitem__(self, item):
        x, y = self.dataset[self.indices[item]]
        return x, y


class ClassDataset(Dataset):
    def __init__(self, dataset, trainClass):
        self.dataset = dataset
        self.classes = dataset.classes
        self.indices = [i for i, x in enumerate(self.dataset.targets) if x == trainClass]

    def __len__(self):
        return 5000

    def __getitem__(self, item):
        x, y = self.dataset[self.indices[item]]
        return x, y


dataset = CIFAR10(
    root='./CIFAR10', train=True, download=True,
    transform=transforms.Compose([
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),  # [0, 1]
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
    ]))

# dataset = LSUN(
#     root='./lsun-master', classes=['classroom_train'],
#     transform=transforms.Compose([
#         transforms.Resize(64),
#         transforms.RandomCrop(64),
#         # transforms.RandomHorizontalFlip(),
#         transforms.ToTensor(),
#         transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
#     ]))


# this is for Animorph
# ds1 = ClassDataset(dataset, 1)  # car
# ds2 = ClassDataset(dataset, 5)  # dog
# # 此处的dl借助了cycle形成了无限循环的迭代器。
# # dl1为car， dl2为dog
# dl1 = cycle(DataLoader(ds1, 16, shuffle=True, num_workers=4, drop_last=True, pin_memory=True))
# dl2 = cycle(DataLoader(ds2, 16, shuffle=True, num_workers=4, drop_last=True, pin_memory=True))


def train(modelConfig: Dict):
    device = torch.device(modelConfig["device"])
    # for CIFAR dataset
    ds_train = ClassDataset(dataset, 1)  # 1 is car, 5 is dog

    # for LSUN dataset
    # ds_train = CustomDataset(dataset, np.arange(0, 20000)) # change (0, 20000) for different train data size

    dataloader = DataLoader(
        ds_train, batch_size=modelConfig["batch_size"], shuffle=True, num_workers=4, drop_last=True, pin_memory=True)

    # model setup
    net_model = UNet(T=modelConfig["T"], ch=modelConfig["channel"], ch_mult=modelConfig["channel_mult"],
                     attn=modelConfig["attn"],
                     num_res_blocks=modelConfig["num_res_blocks"], dropout=modelConfig["dropout"]).to(device)
    if modelConfig["training_load_weight"] is not None:
        net_model.load_state_dict(torch.load(os.path.join(
            modelConfig["save_weight_dir"], modelConfig["training_load_weight"]), map_location=device))
    optimizer = torch.optim.AdamW(
        net_model.parameters(), lr=modelConfig["lr"], weight_decay=1e-4)
    cosineScheduler = optim.lr_scheduler.CosineAnnealingLR(
        optimizer=optimizer, T_max=modelConfig["epoch"], eta_min=0, last_epoch=-1)
    warmUpScheduler = GradualWarmupScheduler(
        optimizer=optimizer, multiplier=modelConfig["multiplier"], warm_epoch=modelConfig["epoch"] // 10,
        after_scheduler=cosineScheduler)
    trainer = GaussianDiffusionTrainer(
        net_model, modelConfig["beta_1"], modelConfig["beta_T"], modelConfig["T"]).to(device)
    # trainer = BlurTrainer(
    #     net_model, modelConfig["T"]).to(device)
    # trainer = AnimorphTrainer(
    #     net_model, modelConfig["beta_1"], modelConfig["beta_T"], modelConfig["T"]).to(device)

    # start training
    for e in range(modelConfig["epoch"]):
        with tqdm(dataloader, dynamic_ncols=True) as tqdmDataLoader:
            # for images, labels in tqdmDataLoader:
            for images, labels in tqdmDataLoader:
                # train
                optimizer.zero_grad()
                x_0 = images.to(device)
                print("train images:", images.shape)
                loss = trainer(x_0).sum() / 1000.
                loss.backward()
                torch.nn.utils.clip_grad_norm_(
                    net_model.parameters(), modelConfig["grad_clip"])
                optimizer.step()
                tqdmDataLoader.set_postfix(ordered_dict={
                    "epoch": e,
                    "loss: ": loss.item(),
                    "img shape: ": x_0.shape,
                    "LR": optimizer.state_dict()['param_groups'][0]["lr"]
                })
        warmUpScheduler.step()
        if (e + 1) % 10 == 0:  # change 10 for save frequency
            torch.save(net_model.state_dict(), os.path.join(
                modelConfig["save_weight_dir"], 'ckpt_' + str(e + 1) + "_.pt"))

    # ************************ For Animorph ***********************
    # step = 0
    # while step < 5000:		# 相当于epoch\
    #     flag = 0
    #     for i in range(2):	# 规定了一个epoch中的训练次数
    #         data_1 = next(dl1).float().cuda()		# 取数据是采用循环取数据的方式实现的
    #         data_2 = next(dl2).float().cuda()
    #         loss = torch.mean(trainer(data_1, data_2))
    #         loss.backward()
    #         torch.nn.utils.clip_grad_norm_(
    #             net_model.parameters(), modelConfig["grad_clip"])
    #         optimizer.step()
    #         if (step + 1) % 100 == 0 and flag == 0:
    #             flag = 1
    #             print("loss = ", loss.item())
    #             print("lr = ", optimizer.state_dict()['param_groups'][0]["lr"])
    #     if (step+1) % 20 == 0:
    #         print("step = ", step+1)
    #     if (step+1) % 30 == 0:
    #         warmUpScheduler.step()
    #     if (step+1) % 1000 == 0:
    #         torch.save(net_model.state_dict(), os.path.join(
    #             modelConfig["save_weight_dir"], 'ckpt_' + str(step+1) + "_.pt"))
    #     step = step + 1


def eval(modelConfig: Dict):
    # load model and evaluate
    with torch.no_grad():
        device = torch.device(modelConfig["device"])
        model = UNet(T=modelConfig["T"], ch=modelConfig["channel"], ch_mult=modelConfig["channel_mult"],
                     attn=modelConfig["attn"],
                     num_res_blocks=modelConfig["num_res_blocks"], dropout=0.)
        ckpt = torch.load(os.path.join(
            modelConfig["save_weight_dir"], modelConfig["test_load_weight"]), map_location=device)
        model.load_state_dict(ckpt)
        print("model load weight done.")
        model.eval()
        sampler = DDIMSampler(
            model, modelConfig["beta_1"], modelConfig["beta_T"], modelConfig["T"]).to(device)
        # trainer = BlurSampler(
        #     model, modelConfig["T"]).to(device)
        # sampler = AnimorphSampler(
        #     model, modelConfig["beta_1"], modelConfig["beta_T"], modelConfig["T"]).to(device)

        # Sampled from standard normal distribution
        noisyImage = torch.randn(
            size=[modelConfig["batch_size"], 3, modelConfig["img_size"], modelConfig["img_size"]], device=device)

        # for i in modelConfig["batch_size"]:
        #     noisyImage[i, :, :, :] = torch.full(size=[3, 32, 32], fill_value=i * 3)  #for Blur

        # noisyImage = next(dl2).cuda()  # for Animorph

        saveNoisy = torch.clamp(noisyImage * 0.5 + 0.5, 0, 1)
        save_image(saveNoisy, os.path.join(
            modelConfig["sampled_dir"], modelConfig["sampledNoisyImgName"]), nrow=modelConfig["nrow"])
        print("original image: ", noisyImage.shape)
        start = time.time()
        sampledImgs = sampler(noisyImage)
        end = time.time()
        print(f"The running time is: {end - start} s")
        sampledImgs = sampledImgs * 0.5 + 0.5  # [0 ~ 1]
        save_image(sampledImgs, os.path.join(
            modelConfig["sampled_dir"], modelConfig["sampledImgName"]), nrow=modelConfig["nrow"])
