class BlurTrainer(nn.Module):
    def __init__(self, model, T):
        super().__init__()

        self.model = model
        self.T = T
        self.kernels = nn.ModuleList()
        for i in range(self.T):
            print(i)
            self.kernels.append(nn.Conv2d(in_channels=3, out_channels=3,
                                          kernel_size=5, padding=2, padding_mode='circular', bias=False,
                                          groups=3, device="cuda:0"))
            nn.init.xavier_uniform_(self.kernels[i].weight, gain=0.01 * i + 0.35)

    def forward(self, x_0):
        img = x_0
        blurs = []
        for i in range(self.T):
            with torch.no_grad():
                img = self.kernels[i](img)
                blurs.append(img)
                # torchvision.utils.save_image(img, './test/test%d.png' % (i), normalize=True)
        blurs.pop()
        img = torch.mean(img, [2, 3], keepdim=True)
        img = img.expand(x_0.shape[0], x_0.shape[1], x_0.shape[2], x_0.shape[3])
        blurs.append(img)
        blurs = torch.stack(blurs)
        choose_blur = []
        t = torch.randint(self.T, size=(x_0.shape[0],), device=x_0.device)
        for step in range(t.shape[0]):
            if step != -1:
                choose_blur.append(blurs[t[step], step])
            else:
                choose_blur.append(x_0[step])
        choose_blur = torch.stack(choose_blur)
        loss = F.mse_loss(self.model(choose_blur, t), x_0, reduction='none')
        return loss


class BlurSampler(nn.Module):
    def __init__(self, model, T):
        super().__init__()

        self.model = model
        self.T = T
        self.kernels =  nn.ModuleList()

        # betas = cosine_beta_schedule(T)
        for i in range(self.T):
            self.kernels.append(nn.Conv2d(in_channels=3, out_channels=3,
                                          kernel_size=5, padding=2, padding_mode='circular', bias=False,
                                          groups=3, device="cuda:0"))
            nn.init.xavier_uniform_(self.kernels[i].weight, gain=0.01 * i + 0.35)

    def forward(self, x_T):
        """
        Algorithm 2.
        """
        img = x_T.cuda()
        for i in range(self.T):
            with torch.no_grad():
                img = self.kernels[i](img)
        temp = img
        times = self.T
        while times:
            # t = torch.full((80,), times - 1, dtype=torch.long).cuda()
            t = torch.randint(self.T, size=(x_T.shape[0],), device=x_T.device)

            x = self.model(img, t)
            x_times=x
            for i in range(times):
                with torch.no_grad():
                    x_times = self.kernels[i](x_times)
                    if i == (self.T - 1):
                        x_times = torch.mean(x_times, [2, 3], keepdim=True)
                        x_times = x_times.expand(temp.shape[0], temp.shape[1], temp.shape[2],
                                                 temp.shape[3])
            x_times_sub_1 = x
            for i in range(times - 1):
                with torch.no_grad():
                    x_times_sub_1 = self.kernels[i](x_times_sub_1)
            x = img - x_times + x_times_sub_1
            img = x
            times = times - 1
            print(torch.mean(x))
        return torch.clip(x, -1, 1)