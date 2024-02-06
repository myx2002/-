#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

//忽略vs对fopen的安全性错误
#pragma warning(disable:4996)					

//函数声明
//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);

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
	fp = fopen("try24.bmp", "rb");
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
	fseek(fp, file.bfoffbits, SEEK_SET);
	fread(rgbpic, sizeof(bit), picwidth * picheight * 3, fp);
	float* yuvpic = (float*)malloc(picwidth * picheight * sizeof(float) * 3);

	//利用公式将BGR类型转换为YUV类型
	for (i = 0;i < picwidth * picheight;i++)
	{
		yuvpic[3 * i] = 0.11 * rgbpic[3 * i] + 0.59 * rgbpic[3 * i + 1] + 0.3 * rgbpic[3 * i + 2];
		yuvpic[3 * i + 1] = 0.493 * (rgbpic[3 * i] - yuvpic[3 * i]);
		yuvpic[3 * i + 2] = 0.877 * (rgbpic[3 * i + 2] - yuvpic[3 * i]);
	}

	//对数变换
	bit max = 0;
	float* logchange = (float*)malloc(sizeof(float) * picwidth * picheight);
	double* logre = (double*)malloc(sizeof(double) * picwidth * picheight);
	double maxi = 0, mini = 1;
	double result;

	//记录图像所有像素的灰度值信息
	for (i = 0;i < picwidth * picheight;i++)
		logchange[i] = yuvpic[3 * i];
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
		logchange[i] = (logre[i] - mini) * result;
		//logchange[i] = logre[i] * max;


	//将YUV格式转化为RGB格式
	//若YUV相应转换值大于255，则保留255
	//若YUV相应转换值小于0，则保留0
	bit* rgbpic1 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	for (i = 0; i < picwidth * picheight; i++)
	{
		if (logchange[i] + 2.0284 * yuvpic[3 * i + 1] < 0)
			rgbpic1[3 * i] = 0;
		else if (logchange[i] + 2.0284 * yuvpic[3 * i + 1] > 255)
			rgbpic1[3 * i] = 255;
		else
			rgbpic1[3 * i] = logchange[i] + 2.0284 * yuvpic[3 * i + 1];

		if (logchange[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] < 0)
			rgbpic1[3 * i + 1] = 0;
		else if (logchange[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] > 255)
			rgbpic1[3 * i + 1] = 255;
		else
			rgbpic1[3 * i + 1] = logchange[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2];

		if (logchange[i] + 1.1403 * yuvpic[3 * i + 2] < 0)
			rgbpic1[3 * i + 2] = 0;
		else if (logchange[i] + 1.1403 * yuvpic[3 * i + 2] > 255)
			rgbpic1[3 * i + 2] = 255;
		else
			rgbpic1[3 * i + 2] = logchange[i] + 1.1403 * yuvpic[3 * i + 2];
	}
	//写入bmp图像文件
	write(file, information, rgbpic1, picheight * picwidth, "logchangergb.bmp");
	free(rgbpic1);
	free(logchange);
	free(logre);

	//直方图均衡化
	int j;
	bit* histor = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	bit* histog = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	bit* histob = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	double* copy = (double*)malloc(sizeof(double) * 255);
	double* copy1 = (double*)malloc(sizeof(double) * 255);
	double* copy2 = (double*)malloc(sizeof(double) * 255);

	//初始化数组为 0
	for (i = 0;i < 255;i++)
	{
		copy[i] = 0;
		copy1[i] = 0;
		copy2[i] = 0;
	}
	//记录bitmap data的信息
	for (i = 0;i < picwidth * picheight;i++)
	{
		histob[i] = rgbpic[3 * i];
		histog[i] = rgbpic[3 * i + 1];
		histor[i] = rgbpic[3 * i + 2];

	}
	//统计任意灰度总像素数
	for (i = 0;i < picheight * picwidth;i++)
	{
		copy[histob[i]]++;
		copy1[histog[i]]++;
		copy2[histor[i]]++;

	}
	//求得任意灰度总占比概率
	for (i = 0;i < 255;i++)
	{
		copy[i] = copy[i] / (picheight * picwidth);
		copy1[i] = copy1[i] / (picheight * picwidth);
		copy2[i] = copy2[i] / (picheight * picwidth);
	}
	//进行概率变换对映
	for (i = 0;i < 255;i++)
	{
		if (i == 0);
		else
		{
			copy[i] = copy[i] + copy[i - 1];
			copy1[i] = copy1[i] + copy1[i - 1];
			copy2[i] = copy2[i] + copy2[i - 1];
		}

	}
	//根据概率对每一灰度值重新赋值
	for (i = 0;i < 255;i++)
	{
		copy[i] = copy[i] * 255;
		copy1[i] = copy1[i] * 255;
		copy2[i] = copy2[i] * 255;
	}
	//对任意像素进行对映灰度函数值转化
	for (i = 0;i < picheight * picwidth;i++)
	{
		histob[i] = copy[histob[i]];
		histog[i] = copy1[histog[i]];
		histor[i] = copy2[histor[i]];
	}

	bit* rgbpic3 = (bit*)malloc(sizeof(bit) * picheight * picwidth * 3);
	for (i = 0;i < picheight * picwidth;i++)
	{
		rgbpic3[3 * i] = histob[i];
		rgbpic3[3 * i + 1] = histog[i];
		rgbpic3[3 * i + 2] = histor[i];
	}

	//写入bmp图像文件
	write(file, information, rgbpic3, picheight * picwidth, "historgb.bmp");
	free(copy);
	free(histob);
	free(histor);
	free(histog);

	//释放动态申请的空间，关闭bmp图片文件
	free(rgbpic);
	free(yuvpic);
	fclose(fp);

	return 0;
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