import torch
import torch.nn as nn
import torch.nn.functional as F
from torchvision.utils import save_image
import os


def extract(a, t, x_shape):
    b, *_ = t.shape
    out = a.gather(-1, t)
    return out.reshape(b, *((1,) * (len(x_shape) - 1)))


def cosine_beta_schedule(timesteps, s=0.008):
    steps = timesteps + 1
    x = torch.linspace(0, timesteps, steps).double()
    alphas_cumprod = torch.cos(((x / timesteps) + s) / (1 + s) * torch.pi * 0.5) ** 2
    alphas_cumprod = alphas_cumprod / alphas_cumprod[0]
    betas = 1 - (alphas_cumprod[1:] / alphas_cumprod[:-1])
    return torch.clip(betas, 0.0001, 0.9999)


class AnimorphTrainer(nn.Module):
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

    def q_sample(self, x_start, x_end, t):
        # simply use the alphas to interpolate
        return (
            extract(self.sqrt_alphas_bar, t, x_start.shape) * x_start +
            extract(self.sqrt_one_minus_alphas_bar, t, x_start.shape) * x_end
        )

    def forward(self, x1, x2):
        t = torch.randint(0, 1000, (80,), device=x1.device).long()
        x_mix = self.q_sample(x_start=x1, x_end=x2, t=t)
        x_recon = self.model(x_mix.float(), t)
        loss = (x1 - x_recon).abs().mean()
        return loss


class AnimorphSampler(nn.Module):
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

        self.register_buffer(
            'sqrt_alphas_bar', torch.sqrt(alphas_bar))
        self.register_buffer(
            'sqrt_one_minus_alphas_bar', torch.sqrt(1. - alphas_bar))

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
        var = torch.cat([self.posterior_var[1:2], self.betas[1:]])
        var = extract(var, t, x_t.shape)

        eps = self.model(x_t, t)
        xt_prev_mean = self.predict_xt_prev_mean_from_eps(x_t, t, eps=eps)

        return xt_prev_mean, var

    def q_sample(self, x_start, x_end, t):
        return (
            extract(self.sqrt_alphas_bar, t, x_start.shape) * x_start +
            extract(self.sqrt_one_minus_alphas_bar, t, x_start.shape) * x_end
        )

    def get_x2_bar_from_xt(self, x1_bar, xt, t):
        return (
            (xt - extract(self.sqrt_alphas_bar, t, x1_bar.shape) * x1_bar) /
            extract(self.sqrt_one_minus_alphas_bar, t, x1_bar.shape)
        )

    def forward(self, img):
        t = 1000
        while t != 0:
            print(t)
            step = torch.full((16,), t - 1, dtype=torch.long).cuda()
            x1_bar = self.model(img.float(), step)  # 应用训练的网络，直接得到的初始图像
            x2_bar = self.get_x2_bar_from_xt(x1_bar, img, step)  # 图片退化时的图，即z

            xt_bar = x1_bar
            if t != 0:
                xt_bar = self.q_sample(x_start=xt_bar, x_end=x2_bar, t=step)  # 对应公式中的D(x0, t)

            xt_sub1_bar = x1_bar
            if t - 1 != 0:
                step2 = torch.full((16,), t - 2, dtype=torch.long).cuda()
                xt_sub1_bar = self.q_sample(x_start=xt_sub1_bar, x_end=x2_bar, t=step2)  # 对应公式中的D(x0, t-1)

            x = img - xt_bar + xt_sub1_bar  # 最终得到的，对应公式中的x_(t-1)
            img = x
            t = t - 1
            if t % 50 == 0:
                save_img = torch.clip(img, -1, 1)
                save_img = save_img * 0.5 + 0.5
                save_image(save_img, os.path.join("./AnimorphRes/", str(t)+".png"), nrow=4)
        return torch.clip(img, -1, 1)