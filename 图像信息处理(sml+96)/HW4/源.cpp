#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mew.h"

#define pi 3.1415926
//忽略vs对fopen的安全性错误
#pragma warning(disable:4996)					

//函数声明
//写入文件函数
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);
//位移几何变换操作函数
void translation(bit* rgbpic, bit* copy, long int height, long int width, int x, int y);
//镜像几何变换操作函数
void mirror(bit* rgbpic,bit* trans,long int height,long int width,char ch);
//错切几何变换操作函数
void shear(bit* rgbpic, bit* change, long int picheight, long int picwidth, long int width, char ch, double tan);
//缩放几何变换操作函数
void scale(bit* rgbpic, bit* scalech, long int picheight, long int picwidth, long int height, long int width, double changex, double changey);
//根据 RGB 位置分布在特定像素位置填入对应 RGB 数值
void copy(bit* change, bit* rgbpic, int record, long int picwidth, int i, int j);
//旋转的几何变换操作函数
void rotation(bit* rgbpic, bit* rotatry, long int picheight, long int picwidth, long int height, long int width, double theta);
//将 int 类型数值转化为 8 的倍数
int ochange(int number);

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

	//接收bmp图像数据信息
	bit* rgbpic = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	fseek(fp, file.bfoffbits, SEEK_SET);
	fread(rgbpic, sizeof(bit), picwidth * picheight * 3, fp);
	//测试bmp数据读取的正确性
	//write(file, information, rgbpic, picheight * picwidth, "check.bmp");


	//进行平移的几何变换(尽量采用8的倍数）
	//下以水平位移64像素，垂直位移64像素为例
	int tx, ty;
	tx = 64;
	ty = 64;
	//将位移量转换为 8 的倍数
	tx = ochange(tx);
	ty = ochange(ty);
	//定义新bitdata map的数据区域大小（附加平移量区域）
	bit* copy = (bit*)malloc(sizeof(bit) * (picheight + fabs(ty)) * (picwidth + fabs(tx)) * 3 );
	//对图片进行位移变化
	translation(rgbpic, copy, picheight, picwidth, tx, ty);
	//定义位移变换后bmp文件的数据头信息
	tagBITMAPFILEHEADER file1 = file;
	tagBITMAPINFOHEADER information1 = information;
	//定义位移后图像行列新像素变量和图像bisizeimage大小
	information1.biwidth = picwidth + fabs(tx);
	information1.biheight = picheight + fabs(ty);
	information1.bisizeimage = (picheight + fabs(ty)) * (picwidth + fabs(tx)) / 8 + 2;
	//定义位移后图像总字节大小
	file1.bfsize = (picheight + fabs(ty)) * (picwidth + fabs(tx)) / 8;
	//写入图像文件
	write(file1, information1, copy, (picheight + fabs(ty)) * (picwidth + fabs(tx)), "out1.bmp");
	free(copy);


	//进行镜像的几何变换
	//定义新bitdata map的数据区域大小（附加平移量区域）
	bit* trans = (bit*)malloc(sizeof(bit) * picheight  * picwidth  * 3);
	//对图像进行y轴镜像变换
	mirror(rgbpic, trans, picheight, picwidth, 'y');
	//写入图像文件
	write(file, information, trans, picheight * picwidth, "out2.bmp");
	//对图像进行x轴镜像变换
	mirror(rgbpic, trans, picheight, picwidth, 'x');
	//写入图像文件
	write(file, information, trans, picheight * picwidth, "out3.bmp");
	free(trans);


	//进行错切的几何变换
	//定义位移变换后bmp文件的数据头信息
	tagBITMAPFILEHEADER file2 = file;
	tagBITMAPINFOHEADER information2 = information;
	//输入错切因数大小
	double tan = 0.8;
	//进行横向错切
	//计算指定错切因数基础上，x 方向偏移量
	long int width;
	width = picwidth + tan * picheight;
	//转化为 8 的倍数
	width = ochange(width);
	bit* change = (bit*)malloc(sizeof(bit) * picheight * width * 3);
	//进行横向错切几何变换
	shear(rgbpic, change, picheight, picwidth, width, 'x', tan);
	//更新错切后图像数据像素宽度和图像 bisizeimage 大小
	information2.biwidth = width;
	information2.bisizeimage = width * picheight / 8 + 2;
	//更新错切后图像总字节大小
	file2.bfsize = width * picheight / 8;
	//写入图像文件
	write(file2, information2, change, picheight * width, "out4.bmp");
	free(change);

	//进行纵向错切
	//计算指定错切因数基础上，x 方向偏移量
	long int height;
	height = picheight + tan * picwidth;
	//转化为 8 的倍数
	height = ochange(height);
	bit* longch = (bit*)malloc(sizeof(bit) * height * picwidth * 3);
	//进行横向错切几何变换
	shear(rgbpic, longch, picheight, picwidth, height, 'y', tan);
	//重置错切后图像数据像素宽度并更新图像像素长度和图像 bisizeimage 大小
	information2.biwidth = picwidth;
	information2.biheight = height;
	information2.bisizeimage = height * picwidth / 8 + 2;
	//更新错切后图像总字节大小
	file2.bfsize = picwidth * height / 8;
	//写入图像文件
	write(file2, information2, longch, height* picwidth, "out5.bmp");
	free(longch);


	//进行缩放的几何变换
	//定义位移变换后bmp文件的数据头信息
	tagBITMAPFILEHEADER file3 = file;
	tagBITMAPINFOHEADER information3 = information;

	//定义缩放比例，x，y 均放大
	double changex = 2;
	double changey = 2;
	//定义新图像行像素个数与列像素个数
	height = picheight * changey;
	width = picwidth * changex;
	//将新定义像素长宽 8 倍数化
	height = ochange(height);
	width = ochange(width);
	bit* scalech = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scalech, picheight, picwidth, height, width, changex, changey);
	//更新图像像素长度与宽度和图像 bisizeimage 大小
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//更新错切后图像总字节大小
	file3.bfsize = width * height / 8;
	//写入图像文件
	write(file3, information3, scalech, height* width, "out6.bmp");
	free(scalech);

	//定义缩放比例，x，放大，y 缩小
	changex = 2;
	changey = 0.5;
	//定义新图像行像素个数与列像素个数
	height = picheight * changey;
	width = picwidth * changex;
	//将新定义像素长宽 8 倍数化
	height = ochange(height);
	width = ochange(width);
	bit* scalebs = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scalebs, picheight, picwidth, height, width, changex, changey);
	//更新图像像素长度与宽度和图像 bisizeimage 大小
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//更新错切后图像总字节大小
	file3.bfsize = width * height / 8;
	//写入图像文件
	write(file3, information3, scalebs, height* width, "out7.bmp");
	free(scalebs);

	//定义缩放比例，x 缩小，y 放大
	changex = 0.5;
	changey = 2;
	//定义新图像行像素个数与列像素个数
	height = picheight * changey;
	width = picwidth * changex;
	//将新定义像素长宽 8 倍数化
	height = ochange(height);
	width = ochange(width);
	bit* scalesb = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scalesb, picheight, picwidth, height, width, changex, changey);
	//更新图像像素长度与宽度和图像 bisizeimage 大小
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//更新错切后图像总字节大小
	file3.bfsize = width * height / 8;
	//写入图像文件
	write(file3, information3, scalesb, height* width, "out8.bmp");
	free(scalesb);

	//定义缩放比例，x，y均 缩小
	changex = 0.5;
	changey = 0.5;
	//定义新图像行像素个数与列像素个数
	height = picheight * changey;
	width = picwidth * changex;
	//将新定义像素长宽 8 倍数化
	height = ochange(height);
	width = ochange(width);
	bit* scaless = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scaless, picheight, picwidth, height, width, changex, changey);
	//更新图像像素长度与宽度和图像 bisizeimage 大小
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//更新错切后图像总字节大小
	file3.bfsize = width * height / 8;
	//写入图像文件
	write(file3, information3, scaless, height* width, "out9.bmp");
	free(scaless);


	//进行旋转的几何变换
	//定义位移变换后bmp文件的数据头信息
	tagBITMAPFILEHEADER file4 = file;
	tagBITMAPINFOHEADER information4 = information;
	//定义需要旋转的角度
	double rota = pi / 24;
	//定义新图像行像素个数与列像素个数
	height = picheight * (sin(rota) + cos(rota));
	width = picwidth * (sin(rota) + cos(rota));
	//将新定义像素长宽 8 倍数化
	height = ochange(height);
	width = ochange(width);
	bit* rotatry = (bit*)malloc(sizeof(bit) * height * width * 3);
	rotation(rgbpic, rotatry, picheight, picwidth, height, width, rota);
	//更新图像像素长度与宽度和图像 bisizeimage 大小
	information4.biwidth = width;
	information4.biheight = height;
	information4.bisizeimage = height * width / 8 + 2;
	//更新错切后图像总字节大小
	file4.bfsize = width * height / 8;
	//写入图像文件
	write(file4, information4, rotatry, height* width, "out10.bmp");
	free(rotatry);

	return 0;
}

//旋转几何变换操作
void rotation(bit* rgbpic, bit* rotatry, long int picheight, long int picwidth, long int height, long int width, double rota)
{
	int i, j;
	int start;
	int begin;
	int hei, wid;
	int record;
	//先将背景至于黑色，方便显示旋转后效果
	for (i = 0;i < height * width * 3;i++)
		rotatry[i] = 0;
	//寻找旋转后图像像素的起始输出位置
	start = picheight * sin(rota);
	//考虑 RGB 数值分布
	start = start * 3;
	begin = start;
	for (i = 0;i < picheight;i++)
	{
		if (i != 0)
		{
			//通过起始位置寻找原始图像每行输出的起始位置
			hei = i * cos(rota);
			wid = i * sin(rota);
			//计算偏移 RGB 数值的位数
			begin = start + hei * width * 3 - wid * 3;
		}
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			//从每一行起始位置开始，通过偏移寻找原始图像每行的新像素位置
			hei = j * sin(rota) / 3;
			wid = j * cos(rota) / 3;
			record = width * 3 * hei + begin + wid * 3;
			//记录对应的 RGB 数值
			copy(rotatry, rgbpic, record, picwidth, i, j);
		}
	}
	//旋转后部分点因坐标求取取整原因未接受到像素 RGB 数值，通过临近值方法进行填补
	for (i = 0;i < height;i++)
		for (j = 3;j < width * 3 - 3;j = j + 3)
		{
			//判断空缺像素位置，非黑色背景区域
			if (rotatry[i * width * 3 + j] == 0 && rotatry[i * width * 3 + j + 1] == 0 && rotatry[i * width * 3 + j + 2] == 0)
				if (rotatry[i * width * 3 + j - 3] != 0 && rotatry[i * width * 3 + j - 2] != 0 && rotatry[i * width * 3 + j - 1] != 0
					&& rotatry[i * width * 3 + j + 3] != 0 && rotatry[i * width * 3 + j + 4] != 0 && rotatry[i * width * 3 + j + 5] != 0)
				{
					//使用有临近值进行填补
					rotatry[i * width * 3 + j] = rotatry[i * width * 3 + j + 3];
					rotatry[i * width * 3 + j + 1] = rotatry[i * width * 3 + j + 4];
					rotatry[i * width * 3 + j + 2] = rotatry[i * width * 3 + j + 5];
				}
		}
}

//缩放几何变换操作
void scale(bit* rgbpic, bit* scalech, long int picheight, long int picwidth, long int height, long int width, double changex, double changey)
{
	int x, y;
	int i, j, record;
	int que, pue;
	int flag1 = 0, flag2 = 0;
	//先将背景至于白色，方便后续填补空白
	for (i = 0;i < height * width * 3;i++)
		scalech[i] = 255;
	//当 x，y 方向均放大时
	if (changex >= 1 && changey >= 1)
	{
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
			{
				//按照放大比列，每隔指定像素赋一原图像素值
				record = changey * i * width * 3 + j * changex;
				scalech[record] = rgbpic[i * picwidth * 3 + j];
				scalech[record + 1] = rgbpic[i * picwidth * 3 + j + 1];
				scalech[record + 2] = rgbpic[i * picwidth * 3 + j + 2];
			}
	}
	//当 x 放大，y 缩小时
	else if (changex >= 1 && changey < 1)
	{
		//求得缩小比
		que = round(1 / changey);
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
				if (i % que == 0)
				{
					//按照比例，在 x 方向上每隔指定像素赋以原图像素值，在 y 方向上每隔一定原图像素取一新图像素
					record = round(i / que);
					record = record * width * 3 + j * changex;
					scalech[record] = rgbpic[i * picwidth * 3 + j];
					scalech[record + 1] = rgbpic[i * picwidth * 3 + j + 1];
					scalech[record + 2] = rgbpic[i * picwidth * 3 + j + 2];
				}
		//填补因缩小比意外带来的整行型空白
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			record = (height - 1) * width * 3 + j * changex;
			copy(scalech, rgbpic, record, picwidth, picheight - 1, j);
		}
	}
	//当 x 缩小，y 放大时
	else if (changex < 1 && changey >= 1)
	{
		//求得缩小比
		que = round(1 / changex);
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
				if (j % que == 0)
				{
					//按照比例，在 y 方向上每隔指定像素赋以原图像素值，在 x 方向上每隔一定原图像素取一新图像素
					record = changey * i;
					record = record * width * 3 + j / que;
					copy(scalech, rgbpic, record, picwidth, i, j);
				}
		//填补因缩小比意外带来的整行型空白
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			record = (height - 1) * width * 3 + j * changex;
			copy(scalech, rgbpic, record, picwidth, picheight - 1, j);
		}
	}
	//当 x，y 方向均缩小时
	else if (changex < 1 && changey < 1)
	{
		pue = round(1 / changey);
		que = round(1 / changex);
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
				if (j % que == 0 && i % pue == 0)
				{
					//按照比例，在 x，y 方向上每隔一定原图像素取一新图像素
					record = i / pue;
					record = record * width * 3 + j / que;
					copy(scalech, rgbpic, record, picwidth, i, j);
				}
		//填补因缩小比意外带来的整行型空白
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			record = (height - 1) * width * 3 + j * changex;
			copy(scalech, rgbpic, record, picwidth, picheight - 1, j);
		}

	}
	//向左取值，填补放大过程中带来的空白像素间隙
	for (i = 0;i < height;i++)
		for (j = 0;j < width * 3;j = j + 3)
			//排除行末尾的自动延展情况，防止填补过程影响其他完备行
			if (scalech[i * width * 3 + j] == 255 && j % (width * 3) != 0)
			{
				scalech[i * width * 3 + j] = scalech[i * width * 3 + j - 3];
				scalech[i * width * 3 + j + 1] = scalech[i * width * 3 + j - 2];
				scalech[i * width * 3 + j + 2] = scalech[i * width * 3 + j - 1];
			}
	//向上取值，填补放大过程中带来的空白像素间隙
	for (j = 0;j < width * 3;j = j + 3)
	{
		for (i = height - 1;i >= 0;i--)
		{
			//排除最高行情况，防止取值溢出发生
			if (scalech[i * width * 3 + j] == 255 && i != height - 1)
			{
				scalech[i * width * 3 + j] = scalech[(i + 1) * width * 3 + j];
				scalech[i * width * 3 + j + 1] = scalech[(i + 1) * width * 3 + j + 1];
				scalech[i * width * 3 + j + 2] = scalech[(i + 1) * width * 3 + j + 2];
			}
		}
	}
}

//根据 RGB 位置分布在特定像素位置填入对应 RGB 数值
void copy(bit* change, bit* rgbpic, int record, long int picwidth, int i, int j)
{
	//若结果为 3 的倍数，直接赋予对映像素的 RGB 数值 
	if (record % 3 == 0)
	{
		change[record] = rgbpic[i * picwidth * 3 + j];
		change[record + 1] = rgbpic[i * picwidth * 3 + j + 1];
		change[record + 2] = rgbpic[i * picwidth * 3 + j + 2];
	}
	//若 mod 3 结果不为 0 ，则对新像素填入区域作适当调整，使得其中 RGB 数值与原图一一对应
	else if (record % 3 == 1)
	{
		change[record - 1] = rgbpic[i * picwidth * 3 + j];
		change[record] = rgbpic[i * picwidth * 3 + j + 1];
		change[record + 1] = rgbpic[i * picwidth * 3 + j + 2];
	}
	else
	{
		change[record - 2] = rgbpic[i * picwidth * 3 + j];
		change[record - 1] = rgbpic[i * picwidth * 3 + j + 1];
		change[record] = rgbpic[i * picwidth * 3 + j + 2];
	}
}

//错切几何变换操作函数
void shear(bit* rgbpic, bit* change, long int picheight, long int picwidth, long int width, char ch, double tan)
{
	int i, j, k = 0;
	int record;
	int height;
	//将背景颜色赋予黑色，方便判断错切后图像输出状况
	for (i = 0;i < picheight * width * 3;i++)
		change[i] = 0;
	//若输入为 x ，进行横向错切
	if (ch == 'x')
	{
		//对错切后图像每一像素 RGB 数值分别赋值
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
			{
				//计算特定行错切偏移量，保留整数结果
				record = i * width * 3 + j + round(tan * (i + 1)) * 3;
				//向特定像素位置填入对应的 RGB 数值
				copy(change, rgbpic, record, picwidth, i, j);
			}
	}
	//若输入为'y', 进行纵向横切
	else if (ch == 'y')
	{
		//纵向填充新图像像素 RGB 数值
		for (i = 0;i < picwidth * 3;i = i + 3)
			for (j = 0;j < picheight;j++)
			{
				//计算特定行错切偏移量，保留整数结果
				record = j * picwidth * 3 + i + round(i / 3 * tan + 0.5) * picwidth * 3;
				//分别填入新像素对映 RGB 数值
				change[record] = rgbpic[j * picwidth * 3 + i];
				change[record + 1] = rgbpic[j * picwidth * 3 + i + 1];
				change[record + 2] = rgbpic[j * picwidth * 3 + i + 2];
			}
	}
}

//镜像几何变换操作函数
void mirror(bit* rgbpic, bit* trans, long int height, long int width, char ch)
{
	int i, j;
	//如果选择对y轴进行镜像变换
	if (ch == 'y')
	{
		//对新图像每一像素 RGB 数值分别赋值
		for (i = 0;i < height;i++)
			for (j = 0;j < width * 3;j = j + 3)
			{
				trans[(i + 1) * width * 3 - j - 1] = rgbpic[i * width * 3 + j + 2];
				trans[(i + 1) * width * 3 - j - 2] = rgbpic[i * width * 3 + j + 1];
				trans[(i + 1) * width * 3 - j - 3] = rgbpic[i * width * 3 + j ];
			}
	}
	//如果选择对x轴进行镜像变换
	else if (ch == 'x')
	{
		//从底向上对新图像各像素进行 RGB 赋值
		for (i = 0;i < height;i++)
			for (j = 0;j < width * 3;j++)
				trans[(height - 1 - i) * width * 3 + j] = rgbpic[i * width * 3 + j];
	}
}

//位移几何变换操作函数
void translation(bit* rgbpic, bit* copy, long int height, long int width, int x, int y)
{
	int i,j;
	//定义位移后起始点
	int startx, starty;
	//int widthbit = (fabs(x) + width) / 8;
	//将新像素背景值均置为黑色（利于观察效果）
	for (i = 0;i < (height + fabs(y)) * (fabs(x) + width) * 3;i++)
		copy[i] = 0;
	//若位移量为负，保持图像从原点开始输出
	if (x < 0)
		startx = 0;
	//若位移量为正，更新图像开始输出位置
	else
		startx = x;
	if (y < 0)
		starty = 0;
	else
		starty = y;
	//将原始像素数值填入对映新像素位置中
	for (i = 0;i < height;i++)
		for (j = 0;j < width * 3;j++)
			copy[(i + starty) * (width + startx) * 3 + j + startx * 3] = rgbpic[i * width * 3 + j];
}

//将数值向上转化为 8 的倍数
int ochange(int number)
{
	double end;
	//求得最大商与对应余数
	end = number % 8;
	number = number / 8;
	//若余数为0，保留原数大小，不然则向上取为 8 的倍数
	if (end != 0)
		number = number * 8 + 8;
	else
		number = number * 8;
	return number;
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