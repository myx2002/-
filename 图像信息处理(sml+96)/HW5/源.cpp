#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

//忽略vs对fopen的安全性错误
#pragma warning(disable:4996)	

//函数声明
//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);
//YUV转换RGB函数
void change(long int picheight, long int picwidth, float* newy, float* yuvpic, bit* rgbpic1);
//均值滤波函数
void meanchange(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
				tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, const char name[]);
//拉普拉斯增强函数
void laplacian(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
				tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information);
//高斯滤波函数
void gauss(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic);

int main()
{
	// 定义图片信息接收变量
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;
	//定义图像长宽
	long int picwidth;
	long int picheight;
	//定义循环指数
	long int i,j;

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

	//定义进行滤波的灰度值贮存数组
	float* newy = (float*)malloc(picwidth * picheight * sizeof(float));
	float* ypic = (float*)malloc(picwidth * picheight * sizeof(float));

	//进行均值滤波
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = yuvpic[3 * i];
	//第一次 5*5 卷积
	meanchange(picheight, picwidth, newy, ypic, yuvpic, file, information, "mean fliter first.bmp");
	//更新bmp图像当前状态
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = newy[i];
	//第二次 5*5 卷积
	meanchange(picheight, picwidth, newy, ypic, yuvpic, file, information, "mean fliter second.bmp");
	//更新bmp图像状态
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = newy[i];
	//第三次 5*5 卷积
	for (j = 0;j <= 6;j++)
	{
		meanchange(picheight, picwidth, newy, ypic, yuvpic, file, information, "mean fliter third.bmp");
		for (i = 0;i < picwidth * picheight;i++)
			ypic[i] = newy[i];
	}

	//高斯滤波（拓展部分，采用 3*3 多次卷积，比较效果）

	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = yuvpic[3 * i];
	gauss(picheight, picwidth, newy, ypic, yuvpic);
	//以共十次卷积为例
	for (j = 0;j <= 8;j++)
	{
		//更新图像灰度值状态
		for (i = 0;i < picwidth * picheight;i++)
			ypic[i] = newy[i];
		//进行高斯滤波
		gauss(picheight, picwidth, newy, ypic, yuvpic);
	}
	bit* rgbpic1 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	//将YUV格式转换为RGB格式
	change(picheight, picwidth, newy, yuvpic, rgbpic1);
	//写入bmp图像文件
	write(file, information, rgbpic1, picheight * picwidth, " gauss fliter.bmp");

	//拉普拉斯增强
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = yuvpic[3 * i];
	//进行拉普拉斯增强变换
	laplacian(picheight, picwidth, newy, ypic, yuvpic, file, information);// "laplacian add.bmp")

	return 0;
}
//高斯滤波函数
void gauss(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic)
{
	long int i, j, k;
	//赋予背景灰度值
	for (i = 0;i < picwidth * picheight;i++)
		newy[i] = ypic[i];
	long int index;
	float sum;
	//进行（n-1）*（n-1）的像素遍历与赋值
	for (i = 1;i < picheight - 1;i++)
		for (j = 1;j < picwidth - 1;j++)
		{
			//计算当前像素位置
			index = i * picwidth + j;
			//重置求和基数
			sum = 0;
			//根据3*3矩阵不同位置加权求取均值
			for (k = -1;k <= 1;k++)
			{
				if (k == 0)
					//若处于矩阵中间行，则加权 2 4 2
					sum = sum + 2 * ypic[index - 1] + 4 * ypic[index] + 2 * ypic[index + 1];
				else
					//若处于矩阵首尾行，则加权 1 2 1
					sum = sum + ypic[index + k * picwidth - 1] + 2 * ypic[index + k * picwidth] + ypic[index + k * picwidth + 1];
			}
			//新灰度值归一化
			newy[index] = round(sum / 16);
			//规定新灰度值合法范围
			if (newy[index] > 255)
				newy[index] = 255;
			else if (newy[index] < 0)
				newy[index] = 0;
		}

}

//拉普拉斯增强函数
void laplacian(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
	tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information)
{
	long int i,j,k;
	int index;
	//赋予背景灰度值
	for (i = 0;i < picheight * picwidth;i++)
		newy[i] = ypic[i];
	float sum;
	//进行（n-1）*（n-1）的像素遍历与赋值
	for (i = 1;i < picheight - 1;i++)
		for (j = 1;j < picwidth - 1;j++)
		{
			//确定当前像素位置
			index = i * picwidth + j;
			//重置求和基数
			sum = 0;
			//根据3*3矩阵不同位置加权计算梯度（采用斜角元素包含方式卷积）
			for (k = -1;k <= 1;k++)
			{
				if (k == 0)
					//若为中心行，则赋权 1 -8 1
					sum = sum + ypic[index - 1] - 8 * ypic[index] + ypic[index + 1];
				else
					//若为首尾行，则赋权 1 1 1 
					sum = sum + ypic[index + k * picwidth - 1] + ypic[index + k * picwidth] + ypic[index + k * picwidth + 1];
			}
			//更新灰度值预期缩减值
			newy[index] = sum;
			//规定新灰度值合法范围
			if (newy[index] > 255)
				newy[index] = 255;
			else if (newy[index] < 0)
				newy[index] = 0;
		}
	//直接输出拉普拉斯算子图像效果
	bit* rgbpic1 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	change(picheight, picwidth, newy, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian operator.bmp");

	//更新原图像灰度值，增加/减少相应的拉普拉斯算子
	for (i = 0;i < picheight * picwidth;i++)
	{
		//根据输出效果判断拉普拉斯算子的运算符号
		ypic[i] = ypic[i] - newy[i];
		//ypic[i] = ypic[i] + newy[i];
		//规定更新灰度值范围
		if (ypic[i] < 0)
			ypic[i] = 0;
	}
	change(picheight, picwidth, ypic, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian enhance.bmp");

	//对拉普拉斯算子进行调整，是图像锐化更明显
	//增大拉普拉斯算子
	for (i = 0;i < picheight * picwidth;i++)
	{
		//乘以自定义系数
		newy[i] = newy[i] * 1.2;
		ypic[i] = ypic[i] - newy[i];
		//规定灰度值输出范围
		if (ypic[i] < 0)
			ypic[i] = 0;
	}
	change(picheight, picwidth, ypic, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian enhance1.bmp");

	//减少拉普拉斯算子
	for (i = 0;i < picheight * picwidth;i++)
	{
		//除以自定义系数
		newy[i] = newy[i] / 1.2;
		ypic[i] = ypic[i] - newy[i];
		//规定灰度值输出范围
		if (ypic[i] < 0)
			ypic[i] = 0;
	}
	change(picheight, picwidth, ypic, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian enhance2.bmp");

	free(rgbpic1);
}

//均值滤波函数
void meanchange(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
		tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, const char name[])
{
	long int i, j, k;
	//赋予背景灰度值
	for (i = 0;i < picwidth * picheight;i++)
		newy[i] = ypic[i];
	long int index;
	float sum;
	//进行（n-2）*（n-2）的像素遍历与赋值
	//采用5*5卷积核大小
	for (i = 2;i < picheight - 2;i++)
		for (j = 2;j < picwidth - 2;j++)
		{
			//测试程序bug时使用
			//printf("%d\n", j);
			//计算当前像素位置
			index = i * picwidth + j;
			//重置求和基数
			sum = 0;
			//对中心像素进行5*5大小的卷积
			//系数占比均为1
			for (k = -2;k <= 2;k++)
				sum = sum + ypic[index + k * picwidth - 1] + ypic[index + k * picwidth] + ypic[index + k * picwidth + 1]
				+ ypic[index + k * picwidth - 2] + ypic[index + k * picwidth + 2];
			//新灰度值归一化
			newy[index] = round(sum / 25);
			//规定新灰度值范围
			if (newy[index] > 255)
				newy[index] = 255;
			else if (newy[index] < 0)
				newy[index] = 0;
		}
	//对边缘像素进行不完全均值滤波操作
	//对顶行进行 2*3 矩阵均值滤波
	for (index = 1;index < 2 * picwidth - 1;index++)
	{
		//跳过最左最右端像素
		if (index == picwidth || index == picwidth - 1);
		else
		{
			sum = 0;
			sum = ypic[index - 1] + ypic[index] + ypic[index + 1] + ypic[index + picwidth - 1] + ypic[index + picwidth + 1] + ypic[index + picwidth];
			newy[index] = sum / 6;
		}
	}
	//对底行进行 2*3 矩阵均值滤波
	for (index = (picheight - 2) * picwidth + 1;index < picheight * picwidth - 1;index++)
	{
		//跳过最左最右端像素
		if (index == picwidth * (picheight - 1) || index == (picheight - 1) * picwidth - 1);
		else
		{
			sum = 0;
			sum = ypic[index - 1] + ypic[index] + ypic[index + 1] + ypic[index - picwidth - 1] + ypic[index - picwidth + 1] + ypic[index - picwidth];
			newy[index] = sum / 6;
		}
	}
	//对最左列进行 2*3 矩阵滤波
	for (index = 1;index < picheight - 1;index++)
	{
		//跳过最上最下端像素
		for (j = 0;j <= 1;j++)
		{
			sum = 0;
			sum = ypic[index * picwidth - picwidth + j] + ypic[index * picwidth + j] + ypic[index * picwidth + 1 + j]
				+ ypic[index * picwidth - picwidth + 1 + j] + ypic[index * picwidth + picwidth + 1 + j] + ypic[index * picwidth + picwidth + j];
			newy[index] = sum / 6;
		}
	}
	//对最右列进行 2*3 矩阵滤波
	for (index = 2;index < picheight;index++)
	{
		//跳过最上最下端像素
		for (j = 0;j <= 1;j++)
		{
			sum = 0;
			sum = ypic[index * picwidth - 1 - picwidth - j] + ypic[index * picwidth - 1 - j] + ypic[index * picwidth - 1 - 1 - j]
				+ ypic[index * picwidth - 1 - picwidth - 1 - j] + ypic[index * picwidth - 1 + picwidth - 1 - j] + ypic[index * picwidth + picwidth - 1 - j];
			newy[index] = sum / 6;
		}
	}

	bit* rgbpic1 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	change(picheight, picwidth, newy, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, name);
	free(rgbpic1);
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