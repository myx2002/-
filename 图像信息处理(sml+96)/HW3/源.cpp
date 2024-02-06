#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmppart.h"

//忽略vs对fopen的安全性错误
#pragma warning(disable:4996)					

//函数声明
//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, PALETTE* rgblib, bit* change, long int number, const char name[]);

int main()
{
	// 定义图片信息接收变量
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;
	//定义图像长宽
	long int picwidth;
	long int picheight;
	//定义循环指数
	long int i;

	//提取bmp文件头部信息
	FILE* fp;
	fp = fopen("lena.bmp", "rb");
	fread(&(file.bftype), 1, sizeof(word), fp);
	fread(&(file.bfsize), 1, sizeof(dword), fp);
	fread(&(file.bfreserved1), 1, sizeof(word), fp);
	fread(&(file.bfreserved2), 1, sizeof(word), fp);
	fread(&(file.bfoffbits), 1, sizeof(dword), fp);
	fread((&information), sizeof(tagBITMAPINFOHEADER), 1, fp);

	//获取bmp大小像素信息
	picwidth = information.biwidth;
	picheight = information.biheight;
	//获取调色板索引表---256色位图
	PALETTE* rgblib = (PALETTE*)malloc(sizeof(PALETTE) * 256);
	fread(rgblib, 256 * sizeof(PALETTE), 1, fp);
	//读入bmp中bitmap data的索引值信息
	bit* rgbgray = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	fread(rgbgray, picwidth * picheight * sizeof(bit), 1, fp);

	//对数变换
	bit max = 0;
	bit* logchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	double* logre = (double*)malloc(sizeof(double) * picwidth * picheight);
	double maxi = 0, mini = 1;
	double result;

	//记录bitmap data的信息
	for (i = 0;i < picwidth * picheight;i++)
		logchange[i] = rgbgray[i];
	//寻找最大灰度值像素
	for (i = 0;i < picwidth * picheight;i++)
		if (logchange[i] > max)
			max = logchange[i];
	//通过对数值将所有像素灰度值归化为 0 - 1 区间内
	for (i = 0;i < picheight * picwidth;i++)
		logre[i] = log(logchange[i] + 1) / log(max + 1);
	//寻找所有像素归化值在 0 - 1 间的分布情况
	for (i = 0;i < picwidth * picheight;i++)
	{
		if (logre[i] > maxi)
			maxi = logre[i];
		if (logre[i] < mini)
			mini = logre[i];
	}
	result = 255 / (maxi - mini);
	//经过对数变化后，重新对每一像素赋灰度值
	for (i = 0;i < picheight * picwidth;i++)
		logchange[i] =(logre[i] - mini) * result;
	//写入bmp图像文件
	write(file, information, rgblib, logchange, picheight*picwidth, "logchange.bmp");
	free(logchange);
	free(logre);

	//直方图均衡化
	int j;
	bit* histo = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	double* copy = (double*)malloc(sizeof(double) * 255);
	//初始化数组为 0
	for (i = 0;i < 255;i++)
		copy[i] = 0;
	//记录bitmap data的信息
	for (i = 0;i < picwidth * picheight;i++)
		histo[i] = rgbgray[i];
	//统计任意灰度总像素数
	for (i = 0;i < picheight * picwidth;i++)
		copy[histo[i]]++;
	//求得任意灰度总占比概率
	for (i = 0;i < 255;i++)
		copy[i] = copy[i] / (picheight * picwidth);
	//进行概率变换对映
	for (i = 0;i < 255;i++)
	{
		if (i == 0);
		else
			copy[i] = copy[i] + copy[i - 1];
	}
	//根据概率对每一灰度值重新赋值
	for (i = 0;i < 255;i++)
		copy[i] = copy[i] * 255;
	//对任意像素进行对映灰度函数值转化
	for (i = 0;i < picheight * picwidth;i++)
		histo[i] = copy[histo[i]];
	//写入bmp图像文件
	write(file, information, rgblib, histo, picheight * picwidth, "histo.bmp");
	free(copy);
	free(histo);

	//释放动态申请的空间，关闭bmp图片文件
	free(rgbgray);
	free(rgblib);
	fclose(fp);

	return 0;
}

//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, PALETTE* rgblib, bit* change, long int number, const char name[])
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
	fwrite(rgblib, 1, sizeof(PALETTE) * 256, fp);
	fwrite(change, 1, sizeof(bit) * number, fp);

	//关闭bmp图像文件
	fclose(fp);
}