#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmppart.h"

//����vs��fopen�İ�ȫ�Դ���
#pragma warning(disable:4996)					

//��������
//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, PALETTE* rgblib, bit* change, long int number, const char name[]);

int main()
{
	// ����ͼƬ��Ϣ���ձ���
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;
	//����ͼ�񳤿�
	long int picwidth;
	long int picheight;
	//����ѭ��ָ��
	long int i;

	//��ȡbmp�ļ�ͷ����Ϣ
	FILE* fp;
	fp = fopen("lena.bmp", "rb");
	fread(&(file.bftype), 1, sizeof(word), fp);
	fread(&(file.bfsize), 1, sizeof(dword), fp);
	fread(&(file.bfreserved1), 1, sizeof(word), fp);
	fread(&(file.bfreserved2), 1, sizeof(word), fp);
	fread(&(file.bfoffbits), 1, sizeof(dword), fp);
	fread((&information), sizeof(tagBITMAPINFOHEADER), 1, fp);

	//��ȡbmp��С������Ϣ
	picwidth = information.biwidth;
	picheight = information.biheight;
	//��ȡ��ɫ��������---256ɫλͼ
	PALETTE* rgblib = (PALETTE*)malloc(sizeof(PALETTE) * 256);
	fread(rgblib, 256 * sizeof(PALETTE), 1, fp);
	//����bmp��bitmap data������ֵ��Ϣ
	bit* rgbgray = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	fread(rgbgray, picwidth * picheight * sizeof(bit), 1, fp);

	//�����任
	bit max = 0;
	bit* logchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	double* logre = (double*)malloc(sizeof(double) * picwidth * picheight);
	double maxi = 0, mini = 1;
	double result;

	//��¼bitmap data����Ϣ
	for (i = 0;i < picwidth * picheight;i++)
		logchange[i] = rgbgray[i];
	//Ѱ�����Ҷ�ֵ����
	for (i = 0;i < picwidth * picheight;i++)
		if (logchange[i] > max)
			max = logchange[i];
	//ͨ������ֵ���������ػҶ�ֵ�黯Ϊ 0 - 1 ������
	for (i = 0;i < picheight * picwidth;i++)
		logre[i] = log(logchange[i] + 1) / log(max + 1);
	//Ѱ���������ع黯ֵ�� 0 - 1 ��ķֲ����
	for (i = 0;i < picwidth * picheight;i++)
	{
		if (logre[i] > maxi)
			maxi = logre[i];
		if (logre[i] < mini)
			mini = logre[i];
	}
	result = 255 / (maxi - mini);
	//���������仯�����¶�ÿһ���ظ��Ҷ�ֵ
	for (i = 0;i < picheight * picwidth;i++)
		logchange[i] =(logre[i] - mini) * result;
	//д��bmpͼ���ļ�
	write(file, information, rgblib, logchange, picheight*picwidth, "logchange.bmp");
	free(logchange);
	free(logre);

	//ֱ��ͼ���⻯
	int j;
	bit* histo = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	double* copy = (double*)malloc(sizeof(double) * 255);
	//��ʼ������Ϊ 0
	for (i = 0;i < 255;i++)
		copy[i] = 0;
	//��¼bitmap data����Ϣ
	for (i = 0;i < picwidth * picheight;i++)
		histo[i] = rgbgray[i];
	//ͳ������Ҷ���������
	for (i = 0;i < picheight * picwidth;i++)
		copy[histo[i]]++;
	//�������Ҷ���ռ�ȸ���
	for (i = 0;i < 255;i++)
		copy[i] = copy[i] / (picheight * picwidth);
	//���и��ʱ任��ӳ
	for (i = 0;i < 255;i++)
	{
		if (i == 0);
		else
			copy[i] = copy[i] + copy[i - 1];
	}
	//���ݸ��ʶ�ÿһ�Ҷ�ֵ���¸�ֵ
	for (i = 0;i < 255;i++)
		copy[i] = copy[i] * 255;
	//���������ؽ��ж�ӳ�ҶȺ���ֵת��
	for (i = 0;i < picheight * picwidth;i++)
		histo[i] = copy[histo[i]];
	//д��bmpͼ���ļ�
	write(file, information, rgblib, histo, picheight * picwidth, "histo.bmp");
	free(copy);
	free(histo);

	//�ͷŶ�̬����Ŀռ䣬�ر�bmpͼƬ�ļ�
	free(rgbgray);
	free(rgblib);
	fclose(fp);

	return 0;
}

//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, PALETTE* rgblib, bit* change, long int number, const char name[])
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
	fwrite(rgblib, 1, sizeof(PALETTE) * 256, fp);
	fwrite(change, 1, sizeof(bit) * number, fp);

	//�ر�bmpͼ���ļ�
	fclose(fp);
}