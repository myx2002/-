#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

//����vs��fopen�İ�ȫ�Դ���
#pragma warning(disable:4996)	

//��������
//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);
//YUVת��RGB����
void change(long int picheight, long int picwidth, float* newy, float* yuvpic, bit* rgbpic1);
//��ֵ�˲�����
void meanchange(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
				tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, const char name[]);
//������˹��ǿ����
void laplacian(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
				tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information);
//��˹�˲�����
void gauss(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic);

int main()
{
	// ����ͼƬ��Ϣ���ձ���
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;
	//����ͼ�񳤿�
	long int picwidth;
	long int picheight;
	//����ѭ��ָ��
	long int i,j;

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

	//��������˲��ĻҶ�ֵ��������
	float* newy = (float*)malloc(picwidth * picheight * sizeof(float));
	float* ypic = (float*)malloc(picwidth * picheight * sizeof(float));

	//���о�ֵ�˲�
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = yuvpic[3 * i];
	//��һ�� 5*5 ���
	meanchange(picheight, picwidth, newy, ypic, yuvpic, file, information, "mean fliter first.bmp");
	//����bmpͼ��ǰ״̬
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = newy[i];
	//�ڶ��� 5*5 ���
	meanchange(picheight, picwidth, newy, ypic, yuvpic, file, information, "mean fliter second.bmp");
	//����bmpͼ��״̬
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = newy[i];
	//������ 5*5 ���
	for (j = 0;j <= 6;j++)
	{
		meanchange(picheight, picwidth, newy, ypic, yuvpic, file, information, "mean fliter third.bmp");
		for (i = 0;i < picwidth * picheight;i++)
			ypic[i] = newy[i];
	}

	//��˹�˲�����չ���֣����� 3*3 ��ξ�����Ƚ�Ч����

	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = yuvpic[3 * i];
	gauss(picheight, picwidth, newy, ypic, yuvpic);
	//�Թ�ʮ�ξ��Ϊ��
	for (j = 0;j <= 8;j++)
	{
		//����ͼ��Ҷ�ֵ״̬
		for (i = 0;i < picwidth * picheight;i++)
			ypic[i] = newy[i];
		//���и�˹�˲�
		gauss(picheight, picwidth, newy, ypic, yuvpic);
	}
	bit* rgbpic1 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	//��YUV��ʽת��ΪRGB��ʽ
	change(picheight, picwidth, newy, yuvpic, rgbpic1);
	//д��bmpͼ���ļ�
	write(file, information, rgbpic1, picheight * picwidth, " gauss fliter.bmp");

	//������˹��ǿ
	for (i = 0;i < picwidth * picheight;i++)
		ypic[i] = yuvpic[3 * i];
	//����������˹��ǿ�任
	laplacian(picheight, picwidth, newy, ypic, yuvpic, file, information);// "laplacian add.bmp")

	return 0;
}
//��˹�˲�����
void gauss(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic)
{
	long int i, j, k;
	//���豳���Ҷ�ֵ
	for (i = 0;i < picwidth * picheight;i++)
		newy[i] = ypic[i];
	long int index;
	float sum;
	//���У�n-1��*��n-1�������ر����븳ֵ
	for (i = 1;i < picheight - 1;i++)
		for (j = 1;j < picwidth - 1;j++)
		{
			//���㵱ǰ����λ��
			index = i * picwidth + j;
			//������ͻ���
			sum = 0;
			//����3*3����ͬλ�ü�Ȩ��ȡ��ֵ
			for (k = -1;k <= 1;k++)
			{
				if (k == 0)
					//�����ھ����м��У����Ȩ 2 4 2
					sum = sum + 2 * ypic[index - 1] + 4 * ypic[index] + 2 * ypic[index + 1];
				else
					//�����ھ�����β�У����Ȩ 1 2 1
					sum = sum + ypic[index + k * picwidth - 1] + 2 * ypic[index + k * picwidth] + ypic[index + k * picwidth + 1];
			}
			//�»Ҷ�ֵ��һ��
			newy[index] = round(sum / 16);
			//�涨�»Ҷ�ֵ�Ϸ���Χ
			if (newy[index] > 255)
				newy[index] = 255;
			else if (newy[index] < 0)
				newy[index] = 0;
		}

}

//������˹��ǿ����
void laplacian(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
	tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information)
{
	long int i,j,k;
	int index;
	//���豳���Ҷ�ֵ
	for (i = 0;i < picheight * picwidth;i++)
		newy[i] = ypic[i];
	float sum;
	//���У�n-1��*��n-1�������ر����븳ֵ
	for (i = 1;i < picheight - 1;i++)
		for (j = 1;j < picwidth - 1;j++)
		{
			//ȷ����ǰ����λ��
			index = i * picwidth + j;
			//������ͻ���
			sum = 0;
			//����3*3����ͬλ�ü�Ȩ�����ݶȣ�����б��Ԫ�ذ�����ʽ�����
			for (k = -1;k <= 1;k++)
			{
				if (k == 0)
					//��Ϊ�����У���Ȩ 1 -8 1
					sum = sum + ypic[index - 1] - 8 * ypic[index] + ypic[index + 1];
				else
					//��Ϊ��β�У���Ȩ 1 1 1 
					sum = sum + ypic[index + k * picwidth - 1] + ypic[index + k * picwidth] + ypic[index + k * picwidth + 1];
			}
			//���»Ҷ�ֵԤ������ֵ
			newy[index] = sum;
			//�涨�»Ҷ�ֵ�Ϸ���Χ
			if (newy[index] > 255)
				newy[index] = 255;
			else if (newy[index] < 0)
				newy[index] = 0;
		}
	//ֱ�����������˹����ͼ��Ч��
	bit* rgbpic1 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	change(picheight, picwidth, newy, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian operator.bmp");

	//����ԭͼ��Ҷ�ֵ������/������Ӧ��������˹����
	for (i = 0;i < picheight * picwidth;i++)
	{
		//�������Ч���ж�������˹���ӵ��������
		ypic[i] = ypic[i] - newy[i];
		//ypic[i] = ypic[i] + newy[i];
		//�涨���»Ҷ�ֵ��Χ
		if (ypic[i] < 0)
			ypic[i] = 0;
	}
	change(picheight, picwidth, ypic, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian enhance.bmp");

	//��������˹���ӽ��е�������ͼ���񻯸�����
	//����������˹����
	for (i = 0;i < picheight * picwidth;i++)
	{
		//�����Զ���ϵ��
		newy[i] = newy[i] * 1.2;
		ypic[i] = ypic[i] - newy[i];
		//�涨�Ҷ�ֵ�����Χ
		if (ypic[i] < 0)
			ypic[i] = 0;
	}
	change(picheight, picwidth, ypic, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian enhance1.bmp");

	//����������˹����
	for (i = 0;i < picheight * picwidth;i++)
	{
		//�����Զ���ϵ��
		newy[i] = newy[i] / 1.2;
		ypic[i] = ypic[i] - newy[i];
		//�涨�Ҷ�ֵ�����Χ
		if (ypic[i] < 0)
			ypic[i] = 0;
	}
	change(picheight, picwidth, ypic, yuvpic, rgbpic1);
	write(file, information, rgbpic1, picheight * picwidth, " laplacian enhance2.bmp");

	free(rgbpic1);
}

//��ֵ�˲�����
void meanchange(long int picheight, long int picwidth, float* newy, float* ypic, float* yuvpic,
		tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, const char name[])
{
	long int i, j, k;
	//���豳���Ҷ�ֵ
	for (i = 0;i < picwidth * picheight;i++)
		newy[i] = ypic[i];
	long int index;
	float sum;
	//���У�n-2��*��n-2�������ر����븳ֵ
	//����5*5����˴�С
	for (i = 2;i < picheight - 2;i++)
		for (j = 2;j < picwidth - 2;j++)
		{
			//���Գ���bugʱʹ��
			//printf("%d\n", j);
			//���㵱ǰ����λ��
			index = i * picwidth + j;
			//������ͻ���
			sum = 0;
			//���������ؽ���5*5��С�ľ��
			//ϵ��ռ�Ⱦ�Ϊ1
			for (k = -2;k <= 2;k++)
				sum = sum + ypic[index + k * picwidth - 1] + ypic[index + k * picwidth] + ypic[index + k * picwidth + 1]
				+ ypic[index + k * picwidth - 2] + ypic[index + k * picwidth + 2];
			//�»Ҷ�ֵ��һ��
			newy[index] = round(sum / 25);
			//�涨�»Ҷ�ֵ��Χ
			if (newy[index] > 255)
				newy[index] = 255;
			else if (newy[index] < 0)
				newy[index] = 0;
		}
	//�Ա�Ե���ؽ��в���ȫ��ֵ�˲�����
	//�Զ��н��� 2*3 �����ֵ�˲�
	for (index = 1;index < 2 * picwidth - 1;index++)
	{
		//�����������Ҷ�����
		if (index == picwidth || index == picwidth - 1);
		else
		{
			sum = 0;
			sum = ypic[index - 1] + ypic[index] + ypic[index + 1] + ypic[index + picwidth - 1] + ypic[index + picwidth + 1] + ypic[index + picwidth];
			newy[index] = sum / 6;
		}
	}
	//�Ե��н��� 2*3 �����ֵ�˲�
	for (index = (picheight - 2) * picwidth + 1;index < picheight * picwidth - 1;index++)
	{
		//�����������Ҷ�����
		if (index == picwidth * (picheight - 1) || index == (picheight - 1) * picwidth - 1);
		else
		{
			sum = 0;
			sum = ypic[index - 1] + ypic[index] + ypic[index + 1] + ypic[index - picwidth - 1] + ypic[index - picwidth + 1] + ypic[index - picwidth];
			newy[index] = sum / 6;
		}
	}
	//�������н��� 2*3 �����˲�
	for (index = 1;index < picheight - 1;index++)
	{
		//�����������¶�����
		for (j = 0;j <= 1;j++)
		{
			sum = 0;
			sum = ypic[index * picwidth - picwidth + j] + ypic[index * picwidth + j] + ypic[index * picwidth + 1 + j]
				+ ypic[index * picwidth - picwidth + 1 + j] + ypic[index * picwidth + picwidth + 1 + j] + ypic[index * picwidth + picwidth + j];
			newy[index] = sum / 6;
		}
	}
	//�������н��� 2*3 �����˲�
	for (index = 2;index < picheight;index++)
	{
		//�����������¶�����
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