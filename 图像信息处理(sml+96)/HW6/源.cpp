#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

//忽略vs对fopen的安全性错误
#pragma warning(disable:4996)	
#define pi 3.1415926

//函数声明
//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);
//YUV转换RGB函数
void change(long int picheight, long int picwidth, float* newy, float* yuvpic, bit* rgbpic1);

double function(double var,double x);

int main()
{
	// 定义图片信息接收变量
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;
	//定义图像长宽
	long int picwidth;
	long int picheight;
	//定义循环指数
	long int i, j;


	//提取bmp文件头部信息
	FILE* fp;
	fp = fopen("try.bmp", "rb");
	fread(&(file.bftype), 1, sizeof(word), fp);
	fread(&(file.bfsize), 1, sizeof(dword), fp);
	fread(&(file.bfreserved1), 1, sizeof(word), fp);
	fread(&(file.bfreserved2), 1, sizeof(word), fp);
	fread(&(file.bfoffbits), 1, sizeof(dword), fp);
	fread((&information), sizeof(tagBITMAPINFOHEADER), 1, fp);

	//获取bmp大小像素信息
	picwidth = information.biwidth;
	picheight = information.biheight;

	//接收bmp图像数据信息
	bit* rgbpic = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	bit* rgbpic1 = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	fseek(fp, file.bfoffbits, SEEK_SET);
	fread(rgbpic, sizeof(bit), picwidth * picheight * 3, fp);
	float* yuvpic = (float*)malloc(picwidth * picheight * sizeof(float) * 3);
	float* newy = (float*)malloc(picwidth * picheight * sizeof(float) * 3);
	float* recordy = (float*)malloc(picwidth * picheight * sizeof(float) * 3);
	//利用公式将BGR类型转换为YUV类型
	for (i = 0;i < picwidth * picheight;i++)
	{
		yuvpic[3 * i] = 0.11 * rgbpic[3 * i] + 0.59 * rgbpic[3 * i + 1] + 0.3 * rgbpic[3 * i + 2];
		yuvpic[3 * i + 1] = 0.493 * (rgbpic[3 * i] - yuvpic[3 * i]);
		yuvpic[3 * i + 2] = 0.877 * (rgbpic[3 * i + 2] - yuvpic[3 * i]);
	}
	for (i = 0;i < picwidth * picheight;i++)
	{
		newy[i] = yuvpic[3 * i];
		recordy[i] = yuvpic[3 * i];
	}
	int x, y;
	//根据对角线长度的2%定义sigema_s
	double optimal;
	optimal = sqrt(pow(picwidth, 2) + pow(picheight, 2));
	//定义sigema_r和sigema_s，前者对映距离权重，后者对映像素值权重
	double omg = 7;
	//取高斯核大小为2*sigema_r，并保证其为奇数
	int rage = 2 * omg + 1;
	double smd = 0.02 * optimal;
	//定义距离，像素值差值等各项权重
	double distance;
	double light;
	//定义归一化总和
	double sum;
	//定义总像素值之和
	double real;
	double record;
	//处理图像主体部分像素值
	for (i = rage / 2 ;i < picheight - rage / 2;i++)
		for (j = rage / 2 ;j < picwidth - rage / 2;j++)
		{
			//对每一高斯核，重置化距离、像素值权重为0
			sum = 0;
			real = 0;
			//遍历高斯核中每一点
			for (x = 0;x < rage;x++)
				for (y = 0;y < rage;y++)
				{
					//计算每一点的距离权重与像素值差值权重
					distance = sqrt(pow(rage / 2 - x, 2) + pow(rage / 2 - y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i - rage / 2 + y) * picheight + j - rage / 2 + x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//更新归一化数值与像素值计算数值
					real = real + record * newy[(i - rage / 2 + y) * picheight + j - rage / 2 + x];
					sum = sum + record;
				}
			//更新中心像素点像素值
			recordy[i * picheight + j] = real / sum;
		}
	//计算最下部分的边界像素点像素值
	for (i = 0;i < rage / 2;i++)
		for (j = 0;j < picwidth - rage / 2;j++)
		{
			//对每一高斯核，重置化距离、像素值权重为0
			sum = 0;
			real = 0;
			//遍历高斯核中每一点
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// 计算每一点的距离权重与像素值差值权重
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i + y) * picheight + j + x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//更新归一化数值与像素值计算数值
					real = real + record * newy[(i + y) * picheight + j + x];
					sum = sum + record;
				}
			//更新中心像素点像素值
			recordy[i * picheight + j] = real / sum;
		}
	//计算最左部分的边界像素点像素值
	for (i = rage / 2;i < picheight;i++)
		for (j = 0;j < rage / 2;j++)
		{
			//对每一高斯核，重置化距离、像素值权重为0
			sum = 0;
			real = 0;
			//遍历高斯核中每一点
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// 计算每一点的距离权重与像素值差值权重
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i - y) * picheight + j + x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//更新归一化数值与像素值计算数值
					real = real + record * newy[(i - y) * picheight + j + x];
					sum = sum + record;
				}
			//更新中心像素点像素值
			recordy[i * picheight + j] = real / sum;
		}
	//计算最上部分的边界像素点像素值
	for (i = picheight - rage / 2;i < picheight;i++)
		for (j = rage / 2;j < picwidth;j++)
		{
			//对每一高斯核，重置化距离、像素值权重为0
			sum = 0;
			real = 0;
			//遍历高斯核中每一点
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// 计算每一点的距离权重与像素值差值权重
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i - y) * picheight + j - x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//更新归一化数值与像素值计算数值
					real = real + record * newy[(i - y) * picheight + j - x];
					sum = sum + record;
				}
			//更新中心像素点像素值
			recordy[i * picheight + j] = real / sum;
		}
	//计算最右部分的边界像素点像素值
	for (i = 0;i < picheight - rage / 2;i++)
		for (j = picwidth - rage / 2;j < picwidth;j++)
		{
			//对每一高斯核，重置化距离、像素值权重为0
			sum = 0;
			real = 0;
			//遍历高斯核中每一点
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// 计算每一点的距离权重与像素值差值权重
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i + y) * picheight + j - x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//更新归一化数值与像素值计算数值
					real = real + record * newy[(i + y) * picheight + j - x];
					sum = sum + record;
				}
			//更新中心像素点像素值
			recordy[i * picheight + j] = real / sum;
		}
	//将YUV格式转变回RGB模式
	change(picheight, picwidth, recordy, yuvpic, rgbpic1);
	//将进过双边滤波的函数写入图像文件
	write(file, information, rgbpic1, picheight * picwidth, "look4.bmp");
	//释放动态申请的变量
	free(rgbpic);
	free(rgbpic1);
	free(yuvpic);
	free(newy);
	free(recordy);
	return 0;
}

//一元正太分布函数计算权重值
double function(double var, double x)
{
	return 1 / sqrt(2 * pi * var) * exp(-pow(x, 2) / (2 * var));
}

//YUV转换为RGB
void change(long int picheight, long int picwidth, float* newy, float* yuvpic, bit* rgbpic1)
{
	//将YUV格式转化为RGB格式
	//若YUV相应转换值大于255，则保留255
	//若YUV相应转换值小于0，则保留0
	int i;
	for (i = 0; i < picwidth * picheight; i++)
	{
		//YUV转化B
		if (newy[i] + 2.0284 * yuvpic[3 * i + 1] < 0)
			rgbpic1[3 * i] = 0;
		else if (newy[i] + 2.0284 * yuvpic[3 * i + 1] > 255)
			rgbpic1[3 * i] = 255;
		else
			rgbpic1[3 * i] = newy[i] + 2.0284 * yuvpic[3 * i + 1];
		//YUV转化G
		if (newy[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] < 0)
			rgbpic1[3 * i + 1] = 0;
		else if (newy[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] > 255)
			rgbpic1[3 * i + 1] = 255;
		else
			rgbpic1[3 * i + 1] = newy[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2];
		//YUV转化R
		if (newy[i] + 1.1403 * yuvpic[3 * i + 2] < 0)
			rgbpic1[3 * i + 2] = 0;
		else if (newy[i] + 1.1403 * yuvpic[3 * i + 2] > 255)
			rgbpic1[3 * i + 2] = 255;
		else
			rgbpic1[3 * i + 2] = newy[i] + 1.1403 * yuvpic[3 * i + 2];
	}
}

//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[])
{
	FILE* fp;

	//打开/新建文件
	fp = fopen(name, "wb");

	//写入bmp图像各类数据
	fwrite(&(file.bftype), 1, sizeof(file.bftype), fp);
	fwrite(&(file.bfsize), 1, sizeof(file.bfsize), fp);
	fwrite(&(file.bfreserved1), 1, sizeof(file.bfreserved1), fp);
	fwrite(&(file.bfreserved2), 1, sizeof(file.bfreserved2), fp);
	fwrite(&(file.bfoffbits), 1, sizeof(file.bfoffbits), fp);
	fwrite(&information, 1, sizeof(information), fp);
	fwrite(rgbpic, sizeof(bit), number * 3, fp);

	//关闭bmp图像文件
	fclose(fp);
}