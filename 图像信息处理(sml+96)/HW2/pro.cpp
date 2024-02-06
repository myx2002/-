#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"gray.h"

//����vs��fopen�İ�ȫ�Դ���
#pragma warning(disable:4996)					

//��������
//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, PALETTE* rgblib, bit* change, long int number, const char name[]);
//��ֵ��ת������
void binary(bit* firstchange, long int number, int threshold);
//����㷨��������ֵ
int otsu(bit* secondchange, long int number);
//�ֲ�����㷨��ֱ�������ֵ
void partotsu(bit* thirdchange, int height, int width, int n);
//��ֵ����������Ͳ���
void dilation(bit* forthchange, int height, int width, int value);
//��ֵ������и�ʴ����
void erosion(bit* fifthchange, int height, int width, int value);
//��ֵ������п�����
void open(bit* sixthchange, int height, int width, int value);
//��ֵ������бղ���
void close(bit* seventhchange, int height, int width, int value);

int main()
{
	//����ͼƬ��Ϣ���ձ���
	tagBITMAPFILEHEADER file;
	tagBITMAPINFOHEADER information;

	//����ͼ�񳤿�
	long int picwidth;
	long int picheight;
	// ������ֵ
	int threshold;			
	//����ѭ������
	int i;

	//��ȡbmp�ļ�ͷ����Ϣ
	FILE* fp;
	fp = fopen("zmygray.bmp", "rb");
	//���ͼƬ����������ռ�ã����쳣��ֱ�ӷ���-1
	if (fp == NULL)
		return -1;
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

	//Ψһ��ֵ��ֵ��
	bit* firstchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		firstchange[i] = rgbgray[i];
	//��0-255��ֵ127��ΪΨһ��ֵ
	//threshold = 127;
	//��Ϊȫ�ִ���㷨���ս��Ϊ124����0-255��ֵ127���Ͻ���Ϊ͹�Բ�ͬ����������Ч����ͬ���趨Ψһ��ֵΪ100
	threshold = 100;
	//��ֵ����д���ļ������
	binary(firstchange, picheight * picwidth, threshold);
	write(file, information, rgblib, firstchange, picheight * picwidth, "only threshold.bmp");
	free(firstchange);

	//ȫ�ֶ�ֵ��
	bit* secondchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		secondchange[i] = rgbgray[i];
	//ͨ������㷨���������ֵ
	threshold = otsu(secondchange, picheight * picwidth);
	//��ֵ����д���ļ������
	binary(secondchange, picheight * picwidth, threshold);
	write(file, information, rgblib, secondchange, picheight * picwidth, "total otsu.bmp");
	free(secondchange);

	//�ֲ���ֵ��
	bit* thirdchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		thirdchange[i] = rgbgray[i];
	//��ͼ����зֿ飬ʹ�ô���㷨Ѱ��ÿ��������ֵ��������Ӧ�Ķ�����
	partotsu(thirdchange, picheight, picwidth,128 );
	write(file, information, rgblib, thirdchange, picheight * picwidth, "part otsu.bmp");
	free(thirdchange);

	//���Ͳ���
	bit* forthchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		forthchange[i] = rgbgray[i];
	//�ȶ�bmp�ļ�����ȫ�ִ���㷨������
	threshold = otsu(forthchange, picheight * picwidth);
	binary(forthchange, picheight * picwidth, threshold);
	//���Ͳ���
	dilation(forthchange, picheight, picwidth, 0);
	write(file, information, rgblib, forthchange, picheight * picwidth, "dilation.bmp");
	free(forthchange);

	//��ʴ����
	bit* fifthchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		fifthchange[i] = rgbgray[i];
	//�ȶ�bmp�ļ�����ȫ�ִ���㷨������
	threshold = otsu(fifthchange, picheight * picwidth);
	binary(fifthchange, picheight * picwidth, threshold);
	//��ʴ����
	erosion(fifthchange, picheight, picwidth, 0);
	write(file, information, rgblib, fifthchange, picheight * picwidth, "erosion.bmp");
	free(fifthchange);

	//������
	bit* sixthchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		sixthchange[i] = rgbgray[i];
	// �ȶ�bmp�ļ�����ȫ�ִ���㷨������
	threshold = otsu(sixthchange, picheight * picwidth);
	binary(sixthchange, picheight * picwidth, threshold);
	//������
	open(sixthchange, picheight, picwidth, 0);
	write(file, information, rgblib, sixthchange, picheight * picwidth, "open.bmp");
	free(sixthchange);

	//�ղ���
	bit* seventhchange = (bit*)malloc(sizeof(bit) * picwidth * picheight);
	for (i = 0;i < picheight * picwidth;i++)
		seventhchange[i] = rgbgray[i];
	// �ȶ�bmp�ļ�����ȫ�ִ���㷨������
	threshold = otsu(seventhchange, picheight * picwidth);
	binary(seventhchange, picheight * picwidth, threshold);
	//�ղ���
	close(seventhchange, picheight, picwidth, 0);
	write(file, information, rgblib, seventhchange, picheight * picwidth, "close.bmp");
	free(seventhchange);


	//�ͷŶ�̬����Ŀռ䣬�ر�bmpͼƬ�ļ�
	free(rgbgray);
	free(rgblib);
	fclose(fp);
	return 0;
}

//����㷨
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
	//����bitmap data��Ѱ��bmp�ļ������С�ֵ�ֵ
	for (i = 0;i < number;i++)
	{
		if (secondchange[i] < min)
			min = secondchange[i];
		if (secondchange[i] > max)
			max = secondchange[i];
	}
	index = min;
	//�����С�ֵ�ֵ�����ڼ���
	//printf("%d", index);
	//������С������ֵ�ֵ��Ѱ�� k ʹ�ô���㷨������
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
				//����ָ���ֵ�ֵ������up����
				numup++;
				//�ۼӴ����ֵ���ֵ
				sumup = sumup + secondchange[j];
			}
			else
			{
				//С��ָ���ֵ�ֵ������down����
				numdown++;
				//�ۼ�С���ֵ���ֵ
				sumdown = sumdown + secondchange[j];
			}
		}
		//����ָ���ֵ�ֵ�´���㷨���
		avgup = numup * 1.0 / number;
		avgdown = numdown * 1.0 / number;
		avg = sumup * 1.0 / numup - sumdown * 1.0 / numdown;
		result = avgup * avgdown * pow(avg, 2);
		//�������Ž��������ֵ�ֵ
		if (result >= remain)
		{
			index = i;
			remain = result;
		}
	}
	//���ڼ��������ȷ��
	//printf("%d", index);
	return index;
}

//�ֲ�����㷨����n*nΪ��С��Ԫ
void partotsu(bit* thirdchange, int height, int width, int n)
{
	bit* data = (bit*)malloc(sizeof(bit) * n * n);
	int i,j,x,y;
	int start;
	int threshold;
	//��bmp�ļ��������ֿ�
	for (i = 0;i < height / n;i++)
	{
		for (j = 0;j < width / n;j++)
		{
			//���õ�ǰ�����ʼλ��
			start =  n * width * i + j * n;
			for (x = 0;x < n;x++)
			{
				//���鸳ֵ
				for (y = 0;y < n ;y++)
					data[x * n + y] = thirdchange[start + width * x + y];
			}
			//Ѱ�þֲ�������ֵ����������Ӧ������
			threshold = otsu(data, n * n);
			binary(data, n * n, threshold);
			for (x = 0;x < n;x++)
			{
				for (y = 0;y < n;y++)
					//�������ú���
					thirdchange[start + width * x + y] = data[n * x + y];
			}
		}
	}
}

//��ֵ������
void binary(bit* firstchange, long int number, int threshold)
{
	long int i;
	for (i = 0;i < number;i++)
	{
		if (firstchange[i] > threshold)
			//������ֵ������Ϊ��ɫ
			firstchange[i] = 255;
		else
			//С����ֵ������Ϊ��ɫ
			firstchange[i] = 0;
	}
}

//ʮ�������Ͳ���
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
			//�����ؿ鱾��Ϊ�ڣ�����ǰ��
			if (forthchange[start] == value)
				continue;
			//�����ؿ���Χ����ǰ�����أ�����������Ϊǰ��
			else if (data[start - 1] == value || data[start + 1] == value || data[start + width] == value || data[start - width] == value)
				forthchange[start] = value;
		}
	}
	//��һ��ǰ������
	for (i = 1;i < width - 1;i++)
	{
		if (forthchange[i] == value)
			continue;
		else if (data[i - 1] == value || data[i + 1] == value || data[i + width] == value)
			forthchange[i] = value;
	}
	//���һ��ǰ������
	for (i = 1;i < width - 1;i++)
	{
		j = (height - 1) * width + i;
		if (forthchange[j] == value)
			continue;
		else if (data[j - 1] == value || data[j + 1] == value || data[j - width] == value)
			forthchange[j] = value;
	}
	//��һ��ǰ���ظ�
	for (i = 1;i < height - 1;i++)
	{
		j = i * width;
		if (forthchange[j] == value)
			continue;
		else if (data[j + 1] == value || data[j - width] == value || data[j + width] == value)
			forthchange[j] = value;
	}
	//���һ��ǰ������
	for (i = 1;i < height - 1;i++)
	{
		j = (i + 1) * width - 1;
		if (forthchange[j] == value)
			continue;
		else if (data[j - 1] == value || data[j - width] == value || data[j + width] == value)
			forthchange[j] = value;
	}
	//�Ľ�ǰ������
	//����
	if (forthchange[0] == value);
	else if (data[width] == value || data[1] == value)
		forthchange[0] == value;
	//����
	if (forthchange[width - 1] == value);
	else if (data[width - 2] == value || data[width * 2 - 1] == value)
		forthchange[width - 1] == value;
	//����
	j = (height - 1) * width;
	if (forthchange[j] == value);
	else if (data[j + 1] == value || data[j - width] == value)
		forthchange[0] == value;
	//����
	j = height * width - 1;
	if (forthchange[j] == value);
	else if (data[j-1] == value || data[j-width] == value)
		forthchange[j] == value;
	free(data);
}

//ʮ���͸�ʴ����
void erosion(bit* fifthchange, int height, int width, int value)
{
	if (value == 0)
		value = 255;
	else
		value = 0;
	//�ȼ��ڱ�������
	dilation(fifthchange, height, width, value);
}

//���������ȸ�ʴ������
void open(bit* sixthchange, int height, int width, int value)
{
	erosion(sixthchange, height, width, value);
	dilation(sixthchange, height, width, value);
}

//�ղ���,�����ͺ�ʴ
void close(bit* seventhchange, int height, int width, int value)
{
	dilation(seventhchange, height, width, value);
	erosion(seventhchange, height, width, value);
}

//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information,PALETTE* rgblib, bit* change, long int number, const char name[])
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
	fwrite(rgblib, 1, sizeof(PALETTE) * 256 , fp);
	fwrite(change, 1, sizeof(bit) * number, fp);

	//�ر�bmpͼ���ļ�
	fclose(fp);

}