%可选不同采样频率
fs=1000;
%fs=2000;
%fs=3000;
T=1/fs;
%可选采样长度，等价于采样点数
%n=500;
n=1500;
%n=2000;
t=(0:n-1)*T;

%构建含有至少三个分量的信号
s=0.7*sin(2*pi*50*t)+sin(2*pi*130*t)+sin(2*pi*90*t);
%添加随机噪声
x=s+2*randn(size(t));

%绘制创造信号时域图
figure(1);
plot(t(1:n/4),s(1:n/4));
xlabel("Time(s)");
ylabel("Amolitude");
title("Time Domain Signal");


figure(4);
plot(t(1:n/4),x(1:n/4));
xlabel("Time(s)");
ylabel("Amolitude");
title("Signal Corrupted with Zero_Mean Random Noise");


%对时域图进行快速傅里叶变换，输出频谱图
%y=fft(s);
%观察含有噪声信号的快速傅里叶变换结果
y=fft(x);
%计算谱密度
p2=abs(y/n);
%计算单侧频谱
p1=p2(1:n/2+1);
%将双侧频谱变换为单侧后，需要将除两端外幅值加倍
p1(2:end-1)=2*p1(2:end-1);
f=fs*(0:n/2)/n;
figure(2);
plot(f,p1);
xlabel("frequency (Hz)");
ylabel("|p1(f)|");
title("single-sided amplitude spectrum of X(t)");

%d对频谱进行一些滤波操作后，使用快速傅里叶逆变换，观察信号变化
%删除小于100Hz的部分
y(1:151)=0;
y(1350:1500)=0;
w=ifft(y);
%取实部输出
w=real(w);
figure(3);
plot(t(1:250),w(1:250));
xlabel("Time(s)");
ylabel("w(t)");
title("Inverse Fast Fourier transform After Signal Processing");

