#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"gray.h"

//忽略vs对fopen的安全性错误
#pragma warning(disable:4996)					

//函数声明
//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, PALETTE* rgblib, bit* change, long int number, const char name[]);
//二值化转化函数
void binary(bit* firstchange, long int number, int threshold);
//大津算法求得最佳阈值
int otsu(bit* secondchange, long int number);
//局部大津算法求分别的最佳阈值
void partotsu(bit* thirdchange, int height, int width, int n);
//二值化后进行膨胀操作
void dilation(bit* forthchange, int height, int width, int value);
//二值化后进行腐蚀操作
void erosion(bit* fifthchange, int height, int width, int value);
//二值化后进行开操作
void open(bit* sixthchange, int height, int width, int value);
//二值化后进行闭操作
void close(bit* seventhchange, int height, int width, int value);

int main()
{
	//定义图片信息接收变量
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;

	//定义图像长宽
	long int picwidth;
	long int picheight;
	// 定义阈值
	int threshold;			
	//定义循环变量
	int i;

	//提取bmp文件头部信息
	FILE* fp;
	fp = fopen("zmygray.bmp", "rb");
	//如果图片被其他程序占用，打开异常，直接返回-1
	if (fp == NULL)
		return -1;
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

	//唯一阈值二值化
	bit* firstchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		firstchange[i] = rgbgray[i];
	//以0-255均值127作为唯一阈值
	//threshold = 127;
	//因为全局大津算法最终结果为124，与0-255均值127相差较近；为凸显不同方法的运算效果不同，设定唯一阈值为100
	threshold = 100;
	//二值化后写入文件并输出
	binary(firstchange, picheight * picwidth, threshold);
	write(file, information, rgblib, firstchange, picheight * picwidth, "only threshold.bmp");
	free(firstchange);

	//全局二值化
	bit* secondchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		secondchange[i] = rgbgray[i];
	//通过大津算法获得最优阈值
	threshold = otsu(secondchange, picheight * picwidth);
	//二值化后写入文件并输出
	binary(secondchange, picheight * picwidth, threshold);
	write(file, information, rgblib, secondchange, picheight * picwidth, "total otsu.bmp");
	free(secondchange);

	//局部二值化
	bit* thirdchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		thirdchange[i] = rgbgray[i];
	//对图像进行分块，使用大津算法寻得每块最优阈值并进行相应的二极化
	partotsu(thirdchange, picheight, picwidth,128 );
	write(file, information, rgblib, thirdchange, picheight * picwidth, "part otsu.bmp");
	free(thirdchange);

	//膨胀操作
	bit* forthchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		forthchange[i] = rgbgray[i];
	//先对bmp文件进行全局大津算法二极化
	threshold = otsu(forthchange, picheight * picwidth);
	binary(forthchange, picheight * picwidth, threshold);
	//膨胀操作
	dilation(forthchange, picheight, picwidth, 0);
	write(file, information, rgblib, forthchange, picheight * picwidth, "dilation.bmp");
	free(forthchange);

	//腐蚀操作
	bit* fifthchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		fifthchange[i] = rgbgray[i];
	//先对bmp文件进行全局大津算法二极化
	threshold = otsu(fifthchange, picheight * picwidth);
	binary(fifthchange, picheight * picwidth, threshold);
	//腐蚀操作
	erosion(fifthchange, picheight, picwidth, 0);
	write(file, information, rgblib, fifthchange, picheight * picwidth, "erosion.bmp");
	free(fifthchange);

	//开操作
	bit* sixthchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		sixthchange[i] = rgbgray[i];
	// 先对bmp文件进行全局大津算法二极化
	threshold = otsu(sixthchange, picheight * picwidth);
	binary(sixthchange, picheight * picwidth, threshold);
	//开操作
	open(sixthchange, picheight, picwidth, 0);
	write(file, information, rgblib, sixthchange, picheight * picwidth, "open.bmp");
	free(sixthchange);

	//闭操作
	bit* seventhchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		seventhchange[i] = rgbgray[i];
	// 先对bmp文件进行全局大津算法二极化
	threshold = otsu(seventhchange, picheight * picwidth);
	binary(seventhchange, picheight * picwidth, threshold);
	//闭操作
	close(seventhchange, picheight, picwidth, 0);
	write(file, information, rgblib, seventhchange, picheight * picwidth, "close.bmp");
	free(seventhchange);


	//释放动态申请的空间，关闭bmp图片文件
	free(rgbgray);
	free(rgblib);
	fclose(fp);
	return 0;
}

//大津算法
int otsu(bit* secondchange, long int number)
{
	int i,j;
	int index;
	int max=0, min=255;
	int numdown = 0, numup = 0;
	double sumdown=0, sumup=0;
	double result;
	double remain = 0;
	double avgup,avgdown;
	double avg;
	//遍历bitmap data，寻得bmp文件最大最小字典值
	for (i = 0;i < number;i++)
	{
		if (secondchange[i] < min)
			min = secondchange[i];
		if (secondchange[i] > max)
			max = secondchange[i];
	}
	index = min;
	//输出最小字典值，用于检验
	//printf("%d", index);
	//遍历最小至最大字典值，寻得 k 使得大津算法结果最大化
	for (i = min;i <= max;i++)
	{
		numdown = 0;
		sumdown = 0;
		numup = 0;
		sumup = 0;
		for (j = 0;j < number;j++)
		{
			if (secondchange[j] > i)
			{
				//大于指定字典值，增加up比列
				numup++;
				//累加大于字典总值
				sumup = sumup + secondchange[j];
			}
			else
			{
				//小于指定字典值，增加down比列
				numdown++;
				//累加小于字典总值
				sumdown = sumdown + secondchange[j];
			}
		}
		//计算指定字典值下大津算法结果
		avgup = numup * 1.0 / number;
		avgdown = numdown * 1.0 / number;
		avg = sumup * 1.0 / numup - sumdown * 1.0 / numdown;
		result = avgup * avgdown * pow(avg, 2);
		//更新最优解与最优字典值
		if (result >= remain)
		{
			index = i;
			remain = result;
		}
	}
	//用于检验程序正确性
	//printf("%d", index);
	return index;
}

//局部大津算法，以n*n为最小单元
void partotsu(bit* thirdchange, int height, int width, int n)
{
	bit* data = (bit*)malloc(sizeof(bit) * n * n);
	int i,j,x,y;
	int start;
	int threshold;
	//将bmp文件总体进项分块
	for (i = 0;i < height / n;i++)
	{
		for (j = 0;j < width / n;j++)
		{
			//重置当前区块初始位置
			start =  n * width * i + j * n;
			for (x = 0;x < n;x++)
			{
				//区块赋值
				for (y = 0;y < n ;y++)
					data[x * n + y] = thirdchange[start + width * x + y];
			}
			//寻得局部最优阈值，并进行相应二极化
			threshold = otsu(data, n * n);
			binary(data, n * n, threshold);
			for (x = 0;x < n;x++)
			{
				for (y = 0;y < n;y++)
					//返回重置后结果
					thirdchange[start + width * x + y] = data[n * x + y];
			}
		}
	}
}

//二值化函数
void binary(bit* firstchange, long int number, int threshold)
{
	long int i;
	for (i = 0;i < number;i++)
	{
		if (firstchange[i] > threshold)
			//大于阈值，保留为黑色
			firstchange[i] = 255;
		else
			//小于阈值，保留为白色
			firstchange[i] = 0;
	}
}

//十字形膨胀操作
void dilation(bit* forthchange, int height, int width, int value)
{
	int i, j;
	int start;
	bit* data = (bit*)malloc(sizeof(bit) * height * width);
	for (i = 0;i < height * width;i++)
		data[i] = forthchange[i];
	for (i = 1;i < height - 1;i++)
	{
		for (j = 1;j < width - 1;j++)
		{
			start = i * width + j;
			//若像素块本身为黑，保留前景
			if (forthchange[start] == value)
				continue;
			//若像素块周围存在前景像素，将此像素置为前景
			else if (data[start - 1] == value || data[start + 1] == value || data[start + width] == value || data[start - width] == value)
				forthchange[start] = value;
		}
	}
	//第一行前景重置
	for (i = 1;i < width - 1;i++)
	{
		if (forthchange[i] == value)
			continue;
		else if (data[i - 1] == value || data[i + 1] == value || data[i + width] == value)
			forthchange[i] = value;
	}
	//最后一行前景重置
	for (i = 1;i < width - 1;i++)
	{
		j = (height - 1) * width + i;
		if (forthchange[j] == value)
			continue;
		else if (data[j - 1] == value || data[j + 1] == value || data[j - width] == value)
			forthchange[j] = value;
	}
	//第一列前景重复
	for (i = 1;i < height - 1;i++)
	{
		j = i * width;
		if (forthchange[j] == value)
			continue;
		else if (data[j + 1] == value || data[j - width] == value || data[j + width] == value)
			forthchange[j] = value;
	}
	//最后一列前景重置
	for (i = 1;i < height - 1;i++)
	{
		j = (i + 1) * width - 1;
		if (forthchange[j] == value)
			continue;
		else if (data[j - 1] == value || data[j - width] == value || data[j + width] == value)
			forthchange[j] = value;
	}
	//四角前景重置
	//左上
	if (forthchange[0] == value);
	else if (data[width] == value || data[1] == value)
		forthchange[0] == value;
	//右上
	if (forthchange[width - 1] == value);
	else if (data[width - 2] == value || data[width * 2 - 1] == value)
		forthchange[width - 1] == value;
	//左下
	j = (height - 1) * width;
	if (forthchange[j] == value);
	else if (data[j + 1] == value || data[j - width] == value)
		forthchange[0] == value;
	//右下
	j = height * width - 1;
	if (forthchange[j] == value);
	else if (data[j-1] == value || data[j-width] == value)
		forthchange[j] == value;
	free(data);
}

//十字型腐蚀操作
void erosion(bit* fifthchange, int height, int width, int value)
{
	if (value == 0)
		value = 255;
	else
		value = 0;
	//等价于背景膨胀
	dilation(fifthchange, height, width, value);
}

//开操作，先腐蚀后膨胀
void open(bit* sixthchange, int height, int width, int value)
{
	erosion(sixthchange, height, width, value);
	dilation(sixthchange, height, width, value);
}

//闭操作,先膨胀后腐蚀
void close(bit* seventhchange, int height, int width, int value)
{
	dilation(seventhchange, height, width, value);
	erosion(seventhchange, height, width, value);
}

//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information,PALETTE* rgblib, bit* change, long int number, const char name[])
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
	fwrite(rgblib, 1, sizeof(PALETTE) * 256 , fp);
	fwrite(change, 1, sizeof(bit) * number, fp);

	//关闭bmp图像文件
	fclose(fp);

}