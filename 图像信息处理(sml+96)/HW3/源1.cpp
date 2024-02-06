#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

//����vs��fopen�İ�ȫ�Դ���
#pragma warning(disable:4996)					

//��������
//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);

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
	fp = fopen("try24.bmp", "rb");
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
	fseek(fp, file.bfoffbits, SEEK_SET);
	fread(rgbpic, sizeof(bit), picwidth * picheight * 3, fp);
	float* yuvpic = (float*)malloc(picwidth * picheight * sizeof(float) * 3);

	//���ù�ʽ��BGR����ת��ΪYUV����
	for (i = 0;i < picwidth * picheight;i++)
	{
		yuvpic[3 * i] = 0.11 * rgbpic[3 * i] + 0.59 * rgbpic[3 * i + 1] + 0.3 * rgbpic[3 * i + 2];
		yuvpic[3 * i + 1] = 0.493 * (rgbpic[3 * i] - yuvpic[3 * i]);
		yuvpic[3 * i + 2] = 0.877 * (rgbpic[3 * i + 2] - yuvpic[3 * i]);
	}

	//�����任
	bit max = 0;
	float* logchange = (float*)malloc(sizeof(float) * picwidth * picheight);
	double* logre = (double*)malloc(sizeof(double) * picwidth * picheight);
	double maxi = 0, mini = 1;
	double result;

	//��¼ͼ���������صĻҶ�ֵ��Ϣ
	for (i = 0;i < picwidth * picheight;i++)
		logchange[i] = yuvpic[3 * i];
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
		logchange[i] = (logre[i] - mini) * result;
		//logchange[i] = logre[i] * max;


	//��YUV��ʽת��ΪRGB��ʽ
	//��YUV��Ӧת��ֵ����255������255
	//��YUV��Ӧת��ֵС��0������0
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
	//д��bmpͼ���ļ�
	write(file, information, rgbpic1, picheight * picwidth, "logchangergb.bmp");
	free(rgbpic1);
	free(logchange);
	free(logre);

	//ֱ��ͼ���⻯
	int j;
	bit* histor = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	bit* histog = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	bit* histob = (bit*)malloc(sizeof(bit) * picheight * picwidth);
	double* copy = (double*)malloc(sizeof(double) * 255);
	double* copy1 = (double*)malloc(sizeof(double) * 255);
	double* copy2 = (double*)malloc(sizeof(double) * 255);

	//��ʼ������Ϊ 0
	for (i = 0;i < 255;i++)
	{
		copy[i] = 0;
		copy1[i] = 0;
		copy2[i] = 0;
	}
	//��¼bitmap data����Ϣ
	for (i = 0;i < picwidth * picheight;i++)
	{
		histob[i] = rgbpic[3 * i];
		histog[i] = rgbpic[3 * i + 1];
		histor[i] = rgbpic[3 * i + 2];

	}
	//ͳ������Ҷ���������
	for (i = 0;i < picheight * picwidth;i++)
	{
		copy[histob[i]]++;
		copy1[histog[i]]++;
		copy2[histor[i]]++;

	}
	//�������Ҷ���ռ�ȸ���
	for (i = 0;i < 255;i++)
	{
		copy[i] = copy[i] / (picheight * picwidth);
		copy1[i] = copy1[i] / (picheight * picwidth);
		copy2[i] = copy2[i] / (picheight * picwidth);
	}
	//���и��ʱ任��ӳ
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
	//���ݸ��ʶ�ÿһ�Ҷ�ֵ���¸�ֵ
	for (i = 0;i < 255;i++)
	{
		copy[i] = copy[i] * 255;
		copy1[i] = copy1[i] * 255;
		copy2[i] = copy2[i] * 255;
	}
	//���������ؽ��ж�ӳ�ҶȺ���ֵת��
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

	//д��bmpͼ���ļ�
	write(file, information, rgbpic3, picheight * picwidth, "historgb.bmp");
	free(copy);
	free(histob);
	free(histor);
	free(histog);

	//�ͷŶ�̬����Ŀռ䣬�ر�bmpͼƬ�ļ�
	free(rgbpic);
	free(yuvpic);
	fclose(fp);

	return 0;
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