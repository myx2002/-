#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

//����vs��fopen�İ�ȫ�Դ���
#pragma warning(disable:4996)	
#define pi 3.1415926

//��������
//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);
//YUVת��RGB����
void change(long int picheight, long int picwidth, float* newy, float* yuvpic, bit* rgbpic1);

double function(double var,double x);

int main()
{
	// ����ͼƬ��Ϣ���ձ���
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;
	//����ͼ�񳤿�
	long int picwidth;
	long int picheight;
	//����ѭ��ָ��
	long int i, j;


	//��ȡbmp�ļ�ͷ����Ϣ
	FILE* fp;
	fp = fopen("try.bmp", "rb");
	fread(&(file.bftype), 1, sizeof(word), fp);
	fread(&(file.bfsize), 1, sizeof(dword), fp);
	fread(&(file.bfreserved1), 1, sizeof(word), fp);
	fread(&(file.bfreserved2), 1, sizeof(word), fp);
	fread(&(file.bfoffbits), 1, sizeof(dword), fp);
	fread((&information), sizeof(tagBITMAPINFOHEADER), 1, fp);

	//��ȡbmp��С������Ϣ
	picwidth = information.biwidth;
	picheight = information.biheight;

	//����bmpͼ��������Ϣ
	bit* rgbpic = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	bit* rgbpic1 = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	fseek(fp, file.bfoffbits, SEEK_SET);
	fread(rgbpic, sizeof(bit), picwidth * picheight * 3, fp);
	float* yuvpic = (float*)malloc(picwidth * picheight * sizeof(float) * 3);
	float* newy = (float*)malloc(picwidth * picheight * sizeof(float) * 3);
	float* recordy = (float*)malloc(picwidth * picheight * sizeof(float) * 3);
	//���ù�ʽ��BGR����ת��ΪYUV����
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
	//���ݶԽ��߳��ȵ�2%����sigema_s
	double optimal;
	optimal = sqrt(pow(picwidth, 2) + pow(picheight, 2));
	//����sigema_r��sigema_s��ǰ�߶�ӳ����Ȩ�أ����߶�ӳ����ֵȨ��
	double omg = 7;
	//ȡ��˹�˴�СΪ2*sigema_r������֤��Ϊ����
	int rage = 2 * omg + 1;
	double smd = 0.02 * optimal;
	//������룬����ֵ��ֵ�ȸ���Ȩ��
	double distance;
	double light;
	//�����һ���ܺ�
	double sum;
	//����������ֵ֮��
	double real;
	double record;
	//����ͼ�����岿������ֵ
	for (i = rage / 2 ;i < picheight - rage / 2;i++)
		for (j = rage / 2 ;j < picwidth - rage / 2;j++)
		{
			//��ÿһ��˹�ˣ����û����롢����ֵȨ��Ϊ0
			sum = 0;
			real = 0;
			//������˹����ÿһ��
			for (x = 0;x < rage;x++)
				for (y = 0;y < rage;y++)
				{
					//����ÿһ��ľ���Ȩ��������ֵ��ֵȨ��
					distance = sqrt(pow(rage / 2 - x, 2) + pow(rage / 2 - y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i - rage / 2 + y) * picheight + j - rage / 2 + x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//���¹�һ����ֵ������ֵ������ֵ
					real = real + record * newy[(i - rage / 2 + y) * picheight + j - rage / 2 + x];
					sum = sum + record;
				}
			//�����������ص�����ֵ
			recordy[i * picheight + j] = real / sum;
		}
	//�������²��ֵı߽����ص�����ֵ
	for (i = 0;i < rage / 2;i++)
		for (j = 0;j < picwidth - rage / 2;j++)
		{
			//��ÿһ��˹�ˣ����û����롢����ֵȨ��Ϊ0
			sum = 0;
			real = 0;
			//������˹����ÿһ��
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// ����ÿһ��ľ���Ȩ��������ֵ��ֵȨ��
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i + y) * picheight + j + x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//���¹�һ����ֵ������ֵ������ֵ
					real = real + record * newy[(i + y) * picheight + j + x];
					sum = sum + record;
				}
			//�����������ص�����ֵ
			recordy[i * picheight + j] = real / sum;
		}
	//�������󲿷ֵı߽����ص�����ֵ
	for (i = rage / 2;i < picheight;i++)
		for (j = 0;j < rage / 2;j++)
		{
			//��ÿһ��˹�ˣ����û����롢����ֵȨ��Ϊ0
			sum = 0;
			real = 0;
			//������˹����ÿһ��
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// ����ÿһ��ľ���Ȩ��������ֵ��ֵȨ��
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i - y) * picheight + j + x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//���¹�һ����ֵ������ֵ������ֵ
					real = real + record * newy[(i - y) * picheight + j + x];
					sum = sum + record;
				}
			//�����������ص�����ֵ
			recordy[i * picheight + j] = real / sum;
		}
	//�������ϲ��ֵı߽����ص�����ֵ
	for (i = picheight - rage / 2;i < picheight;i++)
		for (j = rage / 2;j < picwidth;j++)
		{
			//��ÿһ��˹�ˣ����û����롢����ֵȨ��Ϊ0
			sum = 0;
			real = 0;
			//������˹����ÿһ��
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// ����ÿһ��ľ���Ȩ��������ֵ��ֵȨ��
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i - y) * picheight + j - x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//���¹�һ����ֵ������ֵ������ֵ
					real = real + record * newy[(i - y) * picheight + j - x];
					sum = sum + record;
				}
			//�����������ص�����ֵ
			recordy[i * picheight + j] = real / sum;
		}
	//�������Ҳ��ֵı߽����ص�����ֵ
	for (i = 0;i < picheight - rage / 2;i++)
		for (j = picwidth - rage / 2;j < picwidth;j++)
		{
			//��ÿһ��˹�ˣ����û����롢����ֵȨ��Ϊ0
			sum = 0;
			real = 0;
			//������˹����ÿһ��
			for (x = 0;x < rage / 2;x++)
				for (y = 0;y < rage / 2;y++)
				{
					// ����ÿһ��ľ���Ȩ��������ֵ��ֵȨ��
					distance = sqrt(pow(x, 2) + pow(y, 2));
					light = fabs(newy[i * picheight + j] - newy[(i + y) * picheight + j - x]);
					record = function(pow(omg, 2), distance) * function(pow(smd, 2), light);
					//���¹�һ����ֵ������ֵ������ֵ
					real = real + record * newy[(i + y) * picheight + j - x];
					sum = sum + record;
				}
			//�����������ص�����ֵ
			recordy[i * picheight + j] = real / sum;
		}
	//��YUV��ʽת���RGBģʽ
	change(picheight, picwidth, recordy, yuvpic, rgbpic1);
	//������˫���˲��ĺ���д��ͼ���ļ�
	write(file, information, rgbpic1, picheight * picwidth, "look4.bmp");
	//�ͷŶ�̬����ı���
	free(rgbpic);
	free(rgbpic1);
	free(yuvpic);
	free(newy);
	free(recordy);
	return 0;
}

//һԪ��̫�ֲ���������Ȩ��ֵ
double function(double var, double x)
{
	return 1 / sqrt(2 * pi * var) * exp(-pow(x, 2) / (2 * var));
}

//YUVת��ΪRGB
void change(long int picheight, long int picwidth, float* newy, float* yuvpic, bit* rgbpic1)
{
	//��YUV��ʽת��ΪRGB��ʽ
	//��YUV��Ӧת��ֵ����255������255
	//��YUV��Ӧת��ֵС��0������0
	int i;
	for (i = 0; i < picwidth * picheight; i++)
	{
		//YUVת��B
		if (newy[i] + 2.0284 * yuvpic[3 * i + 1] < 0)
			rgbpic1[3 * i] = 0;
		else if (newy[i] + 2.0284 * yuvpic[3 * i + 1] > 255)
			rgbpic1[3 * i] = 255;
		else
			rgbpic1[3 * i] = newy[i] + 2.0284 * yuvpic[3 * i + 1];
		//YUVת��G
		if (newy[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] < 0)
			rgbpic1[3 * i + 1] = 0;
		else if (newy[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] > 255)
			rgbpic1[3 * i + 1] = 255;
		else
			rgbpic1[3 * i + 1] = newy[i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2];
		//YUVת��R
		if (newy[i] + 1.1403 * yuvpic[3 * i + 2] < 0)
			rgbpic1[3 * i + 2] = 0;
		else if (newy[i] + 1.1403 * yuvpic[3 * i + 2] > 255)
			rgbpic1[3 * i + 2] = 255;
		else
			rgbpic1[3 * i + 2] = newy[i] + 1.1403 * yuvpic[3 * i + 2];
	}
}

//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[])
{
	FILE* fp;

	//��/�½��ļ�
	fp = fopen(name, "wb");

	//д��bmpͼ���������
	fwrite(&(file.bftype), 1, sizeof(file.bftype), fp);
	fwrite(&(file.bfsize), 1, sizeof(file.bfsize), fp);
	fwrite(&(file.bfreserved1), 1, sizeof(file.bfreserved1), fp);
	fwrite(&(file.bfreserved2), 1, sizeof(file.bfreserved2), fp);
	fwrite(&(file.bfoffbits), 1, sizeof(file.bfoffbits), fp);
	fwrite(&information, 1, sizeof(information), fp);
	fwrite(rgbpic, sizeof(bit), number * 3, fp);

	//�ر�bmpͼ���ļ�
	fclose(fp);
}