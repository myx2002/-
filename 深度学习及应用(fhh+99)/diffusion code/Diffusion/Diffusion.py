import torch
import torch.nn as nn
import torch.nn.functional as F
from torchvision.utils import save_image
import os


import numpy as np


def extract(v, t, x_shape):
    device = t.device
    out = torch.gather(v, index=t, dim=0).float().to(device)
    return out.view([t.shape[0]] + [1] * (len(x_shape) - 1))


def cosine_beta_schedule(timesteps, s=0.008):
    steps = timesteps + 1
    x = torch.linspace(0, timesteps, steps).double()
    alphas_cumprod = torch.cos(((x / timesteps) + s) / (1 + s) * torch.pi * 0.5) ** 2
    alphas_cumprod = alphas_cumprod / alphas_cumprod[0]
    betas = 1 - (alphas_cumprod[1:] / alphas_cumprod[:-1])
    return torch.clip(betas, 0.0001, 0.9999)


class GaussianDiffusionTrainer(nn.Module):
    def __init__(self, model, beta_1, beta_T, T):
        super().__init__()

        self.model = model
        self.T = T

        # betas = torch.linspace(beta_1, beta_T, T).double()
        betas = cosine_beta_schedule(T)
        alphas = 1. - betas
        alphas_bar = torch.cumprod(alphas, dim=0)

        # calculations for diffusion q(x_t | x_{t-1}) and others
        self.register_buffer(
            'sqrt_alphas_bar', torch.sqrt(alphas_bar))
        self.register_buffer(
            'sqrt_one_minus_alphas_bar', torch.sqrt(1. - alphas_bar))

    def forward(self, x_0):
        t = torch.randint(self.T, size=(x_0.shape[0], ), device=x_0.device)
        noise = torch.randn_like(x_0)
        # noise=torch.poisson(torch.ones(x_0.shape)).to(x_0.device)
        x_t = (
            extract(self.sqrt_alphas_bar, t, x_0.shape) * x_0 +
            extract(self.sqrt_one_minus_alphas_bar, t, x_0.shape) * noise)
        loss = F.mse_loss(self.model(x_t, t), noise, reduction='none')
        return loss


class GaussianDiffusionSampler(nn.Module):
    def __init__(self, model, beta_1, beta_T, T):
        super().__init__()

        self.model = model
        self.T = T

        # betas = torch.linspace(beta_1, beta_T, T).double()
        betas = cosine_beta_schedule(T)
        self.register_buffer('betas', betas)
        alphas = 1. - betas
        alphas_bar = torch.cumprod(alphas, dim=0)
        alphas_bar_prev = F.pad(alphas_bar, [1, 0], value=1)[:T]

        # self.register_buffer(
        #     'sqrt_alphas_bar', torch.sqrt(alphas_bar))
        # self.register_buffer(
        #     'sqrt_one_minus_alphas_bar', torch.sqrt(1. - alphas_bar))

        self.register_buffer('coeff1', torch.sqrt(1. / alphas))
        self.register_buffer('coeff2', self.coeff1 * (1. - alphas) / torch.sqrt(1. - alphas_bar))

        self.register_buffer('posterior_var', self.betas * (1. - alphas_bar_prev) / (1. - alphas_bar))

    def predict_xt_prev_mean_from_eps(self, x_t, t, eps):
        assert x_t.shape == eps.shape
        return (
            extract(self.coeff1, t, x_t.shape) * x_t -
            extract(self.coeff2, t, x_t.shape) * eps
        )

    def p_mean_variance(self, x_t, t):
        # below: only log_variance is used in the KL computations
        var = torch.cat([self.posterior_var[1:2], self.betas[1:]])
        var = extract(var, t, x_t.shape)

        eps = self.model(x_t, t)
        xt_prev_mean = self.predict_xt_prev_mean_from_eps(x_t, t, eps=eps)

        return xt_prev_mean, var

    def forward(self, x_T):
        x_t = x_T
        for time_step in reversed(range(self.T)):
            print(time_step)
            t = x_t.new_ones([x_T.shape[0], ], dtype=torch.long) * time_step
            mean, var= self.p_mean_variance(x_t=x_t, t=t)
            # no noise when t == 0
            if time_step > 0:
                noise = torch.randn_like(x_t)
            else:
                noise = 0
            x_t = mean + torch.sqrt(var) * noise

            assert torch.isnan(x_t).int().sum() == 0, "nan in tensor."
        x_0 = x_t
        return torch.clip(x_0, -1, 1)

    # def q_sample(self, x_start, x_end, t):
    #     # simply use the alphas to interpolate
    #     return (
    #         extract(self.sqrt_alphas_bar, t, x_start.shape) * x_start +
    #         extract(self.sqrt_one_minus_alphas_bar, t, x_start.shape) * x_end
    #     )
    #
    # def get_x2_bar_from_xt(self, x1_bar, xt, t):
    #     return (
    #         (xt - extract(self.sqrt_alphas_bar, t, x1_bar.shape) * x1_bar) /
    #         extract(self.sqrt_one_minus_alphas_bar, t, x1_bar.shape)
    #     )

    # # Animorph
    # def forward(self, img):
    #     t = 1000
    #     while t != 0:
    #         print(t)
    #         step = torch.full((16,), t - 1, dtype=torch.long).cuda()
    #         x1_bar = self.model(img.float(), step)  # 应用训练的网络，直接得到的初始图像
    #         x2_bar = self.get_x2_bar_from_xt(x1_bar, img, step)  # 图片退化时的图，即z
    #
    #         xt_bar = x1_bar
    #         if t != 0:
    #             xt_bar = self.q_sample(x_start=xt_bar, x_end=x2_bar, t=step)  # 对应公式中的D(x0, t)
    #
    #         xt_sub1_bar = x1_bar
    #         if t - 1 != 0:
    #             step2 = torch.full((16,), t - 2, dtype=torch.long).cuda()
    #             xt_sub1_bar = self.q_sample(x_start=xt_sub1_bar, x_end=x2_bar, t=step2)  # 对应公式中的D(x0, t-1)
    #
    #         # x = img - xt_bar + xt_sub1_bar  # 最终得到的，对应公式中的x_(t-1)
    #         x = xt_sub1_bar  # 最终得到的，对应公式中的x_(t-1)
    #         img = x
    #         t = t - 1
    #         if t % 50 == 0:
    #             save_img = torch.clip(img, -1, 1)
    #             save_img = save_img * 0.5 + 0.5
    #             save_image(save_img, os.path.join("./AnimorphRes/", str(t)+".png"), nrow=4)
    #     return torch.clip(img, -1, 1)


class DDIMSampler(nn.Module):
    def __init__(self, model, beta_1, beta_T, T):
        super().__init__()

        self.model = model
        self.T = T

        # betas=torch.linspace(beta_1, beta_T, T).double()
        betas = cosine_beta_schedule(T)
        self.register_buffer('betas', betas)
        self.register_buffer('alphas', 1. - betas)
        self.register_buffer('alphas_bar', torch.cumprod(self.alphas, dim=0))

    def forward(self, x_T,interval=50):
        """
        Algorithm 2.
        """
        x_t = x_T
        for time_step in reversed(range(int(self.T/interval))):
            # print(time_step)
            t = x_t.new_ones([x_T.shape[0], ], dtype=torch.long) * time_step * interval
            print(time_step*interval)
            # no noise when t == 0
            if time_step > 0:
                # noise = torch.randn_like(x_t)
                eps = self.model(x_t, t)
                x_t = torch.div(x_t-torch.sqrt(1-self.alphas_bar[time_step*interval])*eps,torch.sqrt(self.alphas_bar[time_step*interval]/self.alphas_bar[(time_step-1)*interval]))+torch.sqrt(1-self.alphas_bar[(time_step-1)*interval])*eps
            else:
                eps = 0
                x_t = torch.div(x_t-torch.sqrt(1-self.alphas_bar[time_step*interval])*eps,torch.sqrt(self.alphas_bar[time_step*interval]))

            #assert torch.isnan(x_t).int().sum() == 0, "nan in tensor."
        x_0 = x_t
        return torch.clip(x_0, -1, 1)

    # def forward(self, x_T):
    #     t = x_T.new_ones([x_T.shape[0], ], dtype=torch.long) * (self.T - 1)
    #     temp = torch.cumprod(1. - self.betas, dim=0)[-1]
    #     print(temp)
    #     x_0 = torch.div((x_T - torch.sqrt(1 - temp) * self.model(x_T, t)), torch.sqrt(temp))
    #     print(torch.mean(x_0))
    #     return torch.clip(x_0, -1, 1)


class PLMSSampler(nn.Module):
    def __init__(self, model, beta_1, beta_T, T):
        super().__init__()

        self.model = model
        self.T = T

        # betas = torch.linspace(beta_1, beta_T, T).double()
        betas = cosine_beta_schedule(T)
        self.register_buffer('betas', betas)
        self.register_buffer('alphas', 1. - betas)
        self.register_buffer('alphas_bar', torch.cumprod(self.alphas, dim=0))

    def forward(self, x_T, interval=50):
        """
        Algorithm 2.
        """
        x_t = x_T
        eps_list = []
        for time_step in reversed(range(int(self.T / interval))):
            # print(time_step)
            t = x_t.new_ones([x_T.shape[0], ], dtype=torch.long) * time_step * interval
            # mean, var= self.p_mean_variance(x_t=x_t, t=t)
            # no noise when t == 0
            print(time_step*interval)
            # noise = torch.randn_like(x_t)
            eps = self.model(x_t, t)
            lens = len(eps_list)
            if lens:
                if lens == 1:
                    eps_ = (3 * eps - eps_list[-1])/ 2
                elif lens == 2:
                    eps_ = (23 * eps - 16 * eps_list[-1] + 5 * eps_list[-2])/ 12
                else:
                    eps_ = (55 * eps - 59 * eps_list[-1] + 37 * eps_list[-2] - 9 * eps_list[-3]) / 24
            else:
                eps_ = eps
            if time_step:
                x_t = torch.div(x_t - torch.sqrt(1 - self.alphas_bar[time_step * interval]) * eps_, torch.sqrt(
                    self.alphas_bar[time_step * interval] / self.alphas_bar[(time_step - 1) * interval])) + torch.sqrt(
                    1 - self.alphas_bar[(time_step - 1) * interval]) * eps_
                # print(time_step*interval,torch.mean(x_t))
            else:
                x_t = torch.div(x_t - torch.sqrt(1 - self.alphas_bar[time_step * interval]) * eps_,
                                torch.sqrt(self.alphas_bar[time_step * interval]))
            eps_list.append(eps)
            # x_t = mean + torch.sqrt(var) * noise
            # assert torch.isnan(x_t).int().sum() == 0, "nan in tensor."
        x_0 = x_t
        return torch.clip(x_0, -1, 1)
