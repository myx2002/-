#ifndef _BMP_
#define _BMP_

typedef unsigned char bit;
typedef unsigned short word;
typedef unsigned long  dword;


struct tagBITMAPFILEHEADER
{
	word bftype;			//文件类型
	dword bfsize;			//文件大小（字节为单位）
	word bfreserved1;		//保留，必须设为0
	word bfreserved2;		//保留，必须设为0
	dword bfoffbits;		//到实际图像实际开始的偏移量
};

struct tagBITMAPINFOHEADER
{
	dword bisize;			//结构所需字节数
	long biwidth;			//图像宽度（像素为单位）
	long biheight;			//图像长度（像素为单位）--正值对映图像倒立
	word biplanes;			//位面数，常为1
	word bibitcounts;		//比特/像素比
	dword bicompression;	//压缩类型
	dword bisizeimage;		//图像大小
	long bixpelspermeter;	//水平分辨率
	long biypelspermeter;	//垂直分辨率
	dword biclrused;		//颜色索引数
	dword biclrimportant;	//重要影响颜色索引数
};

#endif


