import math

import torch
from torch import nn
from torch.nn import functional as F, init


class MyUnet(nn.Module):
    def __init__(self, T, first_channel, channel_mult):
        super().__init__()
        tdim = 4 * first_channel
        self.Time_Embedding = TimeEmbedding(T, first_channel, tdim)
        self.HeadBlock = nn.Conv2d(3, first_channel, kernel_size=3, stride=1, padding=1)
        self.DownBlocks = nn.ModuleList()
        self.UpBlocks = nn.ModuleList()
        self.MiddleBlock = nn.ModuleList()

        channels = [first_channel]
        current_channel = first_channel
        for i, mul in enumerate(channel_mult):
            current_out_channel = current_channel * mul
            self.DownBlocks.append(ResBlock(current_channel, current_out_channel,tdim))
            channels.append(current_out_channel)
            current_channel = current_out_channel
            if i != len(channel_mult) - 1:
                self.DownBlocks.append(DownSampling(current_channel))
                channels.append(current_channel)
        # print(current_channel)
        self.MiddleBlock.append(ResBlock(current_channel, current_channel,tdim))
        self.MiddleBlock.append(ResBlock(current_channel, current_channel,tdim))
        for i, mul in reversed(list(enumerate(channel_mult))):
            current_out_channel = int(current_channel / mul)
            if i != len(channel_mult) - 1:
                current_channel = channels.pop(-1)
                self.UpBlocks.append(UpSampling(current_channel))
            self.UpBlocks.append(ResBlock(current_channel*2, current_out_channel,tdim))
            channels.pop(-1)
            current_channel = current_out_channel
        self.TailBlock = nn.Sequential(
            nn.GroupNorm(32, current_channel),
            nn.Sigmoid(),
            nn.Conv2d(current_channel, first_channel, kernel_size=3, stride=1, padding=1),
            nn.GroupNorm(32, current_channel),
            nn.Sigmoid(),
            nn.Conv2d(first_channel, 3, kernel_size=3, stride=1, padding=1)
        )
        # self.Embedding = nn.Embedding()

    def initialize(self):
        init.xavier_uniform_(self.HeadBlock.weight)
        init.zeros_(self.HeadBlock.bias)
        init.xavier_uniform_(self.TailBlock.weight, gain=1e-5)
        init.zeros_(self.TailBlock.bias)

    def forward(self, x, t):
        time_embedding = self.Time_Embedding(t)
        h = self.HeadBlock(x)
        hs = [h]
        for layer in self.DownBlocks:
            h = layer(h, time_embedding)

            if isinstance(layer, ResBlock):
                hs.append(h)
        for layer in self.MiddleBlock:
            h = layer(h, time_embedding)
            # print("2",h.shape)
        for layer in self.UpBlocks:
            if isinstance(layer, ResBlock):
                # print(h.shape,hs[-1].shape)
                h = torch.cat([h, hs.pop(-1)], dim=1)
            # print(layer)
            h = layer(h, time_embedding)
            # print("3", h.shape)
        # print(h.shape)
        h = self.TailBlock(h)

        return h


class DownSampling(nn.Module):
    def __init__(self, C):
        super().__init__()
        self.Down = nn.Sequential(
            nn.Conv2d(C, C, kernel_size=3, stride=2, padding=1),
            nn.LeakyReLU()
        )
        self.initialize()

    def initialize(self):
        init.xavier_uniform_(self.Down[0].weight)
        init.zeros_(self.Down[0].bias)

    def forward(self, x, temb):
        x = self.Down(x)
        return x


class UpSampling(nn.Module):
    def __init__(self, C):
        super().__init__()
        # print(C)
        self.Up = nn.Conv2d(C, C, kernel_size=3, stride=1, padding=1)
        self.initialize()

    def initialize(self):
        init.xavier_uniform_(self.Up.weight)
        init.zeros_(self.Up.bias)

    def forward(self, x, r):

        x = F.interpolate(x, scale_factor=2)
        #print(x.shape)
        x = self.Up(x)
        return x


class ResBlock(nn.Module):
    def __init__(self, inplanes, planes,tdim):
        super().__init__()
        self.norm1 = nn.GroupNorm(32, inplanes)
        self.conv1 = nn.Conv2d(inplanes, inplanes, kernel_size=3, stride=1, padding=1)
        self.norm2 = nn.GroupNorm(32, planes)
        self.conv2 = nn.Conv2d(inplanes, planes, kernel_size=3, stride=1, padding=1)
        self.attn = AttnBlock(planes)
        self.time_embedding=nn.Linear(tdim,inplanes)
        self.initialize()

    def initialize(self):
        init.xavier_uniform_(self.conv1.weight)
        init.xavier_uniform_(self.conv2.weight, gain=1e-5)

    def forward(self, x, embedding):
        residual = x
        # print(embedding.shape)
        embedding=self.time_embedding(embedding)
        # print(x.shape,embedding.shape)
        out = F.relu(self.norm1(self.conv1(x + embedding[:, :, None, None])))
        out += residual
        out = F.relu(self.norm2(self.conv2(x)))
        out = self.attn(out)
        return out


class TimeEmbedding(nn.Module):
    def __init__(self, T, d_model, dim):
        assert d_model % 2 == 0
        super().__init__()
        emb = torch.arange(0, d_model, step=2) / d_model * math.log(10000)
        emb = torch.exp(-emb)
        pos = torch.arange(T).float()
        emb = pos[:, None] * emb[None, :]
        assert list(emb.shape) == [T, d_model // 2]
        emb = torch.stack([torch.sin(emb), torch.cos(emb)], dim=-1)
        assert list(emb.shape) == [T, d_model // 2, 2]
        emb = emb.view(T, d_model)

        self.timembedding = nn.Sequential(
            nn.Embedding.from_pretrained(emb),
            nn.Linear(d_model, dim),
            nn.Sigmoid(),
            nn.Linear(dim, dim),
        )
        self.initialize()

    def initialize(self):
        for module in self.modules():
            if isinstance(module, nn.Linear):
                init.xavier_uniform_(module.weight)
                init.zeros_(module.bias)

    def forward(self, t):
        emb = self.timembedding(t)
        return emb


class AttnBlock(nn.Module):
    def __init__(self, in_ch):
        super().__init__()
        self.group_norm = nn.GroupNorm(32, in_ch)
        self.proj_q = nn.Conv2d(in_ch, in_ch, 1, stride=1, padding=0)
        self.proj_k = nn.Conv2d(in_ch, in_ch, 1, stride=1, padding=0)
        self.proj_v = nn.Conv2d(in_ch, in_ch, 1, stride=1, padding=0)
        self.proj = nn.Conv2d(in_ch, in_ch, 1, stride=1, padding=0)
        self.initialize()

    def initialize(self):
        for module in [self.proj_q, self.proj_k, self.proj_v, self.proj]:
            init.xavier_uniform_(module.weight)
            init.zeros_(module.bias)
        init.xavier_uniform_(self.proj.weight, gain=1e-5)

    def forward(self, x):
        B, C, H, W = x.shape
        h = self.group_norm(x)
        q = self.proj_q(h)
        k = self.proj_k(h)
        v = self.proj_v(h)

        q = q.permute(0, 2, 3, 1).view(B, H * W, C)
        k = k.view(B, C, H * W)
        w = torch.bmm(q, k) * (int(C) ** (-0.5))
        assert list(w.shape) == [B, H * W, H * W]
        w = F.softmax(w, dim=-1)

        v = v.permute(0, 2, 3, 1).view(B, H * W, C)
        h = torch.bmm(w, v)
        assert list(h.shape) == [B, H * W, C]
        h = h.view(B, H, W, C).permute(0, 3, 1, 2)
        h = self.proj(h)

        return x + h


if __name__ == '__main__':
    batch_size = 8
    model = Unet(
        T=1000, first_channel=128, channel_mult=[1, 2, 2, 2])
    x = torch.randn(batch_size, 3, 32, 32)
    t = torch.randint(1000, (batch_size,))
    y = model(x, t)
    print(y.shape)
