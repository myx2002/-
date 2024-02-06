from Diffusion.Train import train, eval


def main(model_config = None):
    modelConfig = {
        "state": "eval", # or eval
        "epoch": 200,
        "batch_size": 80,  # change if you want to generate different number of photos one time
        "T": 1000,  # 8 or 10 for blur
        "channel": 128,
        "channel_mult": [1, 2, 3, 4],
        "attn": [2],
        "num_res_blocks": 2,
        "dropout": 0.15,
        "lr": 1e-4,
        "multiplier": 2.,
        "beta_1": 1e-4,
        "beta_T": 0.02,
        "img_size": 32, # 64 for LSUN classroom dataset
        "grad_clip": 1.,
        "device": "cuda:0", # or cpu
        "training_load_weight": None,
        "save_weight_dir": "./CarCosineCheckpoints/",
        "test_load_weight": "ckpt_200_.pt",
        "sampled_dir": "./SampledImgs/",
        "sampledNoisyImgName": "NoisyCIFARImgs.png",
        "sampledImgName": "SampledCIFARImgs.png",
        "nrow": 8 # or 4 for LSUN classroom dataset
        }
    if model_config is not None:
        modelConfig = model_config
    if modelConfig["state"] == "train":
        train(modelConfig)
    else:
        eval(modelConfig)


if __name__ == '__main__':
    main()
