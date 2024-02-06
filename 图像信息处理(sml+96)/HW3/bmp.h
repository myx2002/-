#ifndef _BMP_
#define _BMP_

typedef unsigned char bit;
typedef unsigned short word;
typedef unsigned long  dword;


struct tagBITMAPFILEHEADER
{
	word bftype;			//�ļ�����
	dword bfsize;			//�ļ���С���ֽ�Ϊ��λ��
	word bfreserved1;		//������������Ϊ0
	word bfreserved2;		//������������Ϊ0
	dword bfoffbits;		//��ʵ��ͼ��ʵ�ʿ�ʼ��ƫ����
};

struct tagBITMAPINFOHEADER
{
	dword bisize;			//�ṹ�����ֽ���
	long biwidth;			//ͼ���ȣ�����Ϊ��λ��
	long biheight;			//ͼ�񳤶ȣ�����Ϊ��λ��--��ֵ��ӳͼ����
	word biplanes;			//λ��������Ϊ1
	word bibitcounts;		//����/���ر�
	dword bicompression;	//ѹ������
	dword bisizeimage;		//ͼ���С
	long bixpelspermeter;	//ˮƽ�ֱ���
	long biypelspermeter;	//��ֱ�ֱ���
	dword biclrused;		//��ɫ������
	dword biclrimportant;	//��ҪӰ����ɫ������
};

#endif


