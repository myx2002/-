#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mew.h"

#define pi 3.1415926
//����vs��fopen�İ�ȫ�Դ���
#pragma warning(disable:4996)					

//��������
//д���ļ�����
void write(tagBITMAPFILEHEADER file, tagBITMAPINFOHEADER information, bit* rgbpic, long int number, const char name[]);
//λ�Ƽ��α任��������
void translation(bit* rgbpic, bit* copy, long int height, long int width, int x, int y);
//���񼸺α任��������
void mirror(bit* rgbpic,bit* trans,long int height,long int width,char ch);
//���м��α任��������
void shear(bit* rgbpic, bit* change, long int picheight, long int picwidth, long int width, char ch, double tan);
//���ż��α任��������
void scale(bit* rgbpic, bit* scalech, long int picheight, long int picwidth, long int height, long int width, double changex, double changey);
//���� RGB λ�÷ֲ����ض�����λ�������Ӧ RGB ��ֵ
void copy(bit* change, bit* rgbpic, int record, long int picwidth, int i, int j);
//��ת�ļ��α任��������
void rotation(bit* rgbpic, bit* rotatry, long int picheight, long int picwidth, long int height, long int width, double theta);
//�� int ������ֵת��Ϊ 8 �ı���
int ochange(int number);

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

	//����bmpͼ��������Ϣ
	bit* rgbpic = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	fseek(fp, file.bfoffbits, SEEK_SET);
	fread(rgbpic, sizeof(bit), picwidth * picheight * 3, fp);
	//����bmp���ݶ�ȡ����ȷ��
	//write(file, information, rgbpic, picheight * picwidth, "check.bmp");


	//����ƽ�Ƶļ��α任(��������8�ı�����
	//����ˮƽλ��64���أ���ֱλ��64����Ϊ��
	int tx, ty;
	tx = 64;
	ty = 64;
	//��λ����ת��Ϊ 8 �ı���
	tx = ochange(tx);
	ty = ochange(ty);
	//������bitdata map�����������С������ƽ��������
	bit* copy = (bit*)malloc(sizeof(bit) * (picheight + fabs(ty)) * (picwidth + fabs(tx)) * 3 );
	//��ͼƬ����λ�Ʊ仯
	translation(rgbpic, copy, picheight, picwidth, tx, ty);
	//����λ�Ʊ任��bmp�ļ�������ͷ��Ϣ
	tagBITMAPFILEHEADER file1 = file;
	tagBITMAPINFOHEADER information1 = information;
	//����λ�ƺ�ͼ�����������ر�����ͼ��bisizeimage��С
	information1.biwidth = picwidth + fabs(tx);
	information1.biheight = picheight + fabs(ty);
	information1.bisizeimage = (picheight + fabs(ty)) * (picwidth + fabs(tx)) / 8 + 2;
	//����λ�ƺ�ͼ�����ֽڴ�С
	file1.bfsize = (picheight + fabs(ty)) * (picwidth + fabs(tx)) / 8;
	//д��ͼ���ļ�
	write(file1, information1, copy, (picheight + fabs(ty)) * (picwidth + fabs(tx)), "out1.bmp");
	free(copy);


	//���о���ļ��α任
	//������bitdata map�����������С������ƽ��������
	bit* trans = (bit*)malloc(sizeof(bit) * picheight  * picwidth  * 3);
	//��ͼ�����y�᾵��任
	mirror(rgbpic, trans, picheight, picwidth, 'y');
	//д��ͼ���ļ�
	write(file, information, trans, picheight * picwidth, "out2.bmp");
	//��ͼ�����x�᾵��任
	mirror(rgbpic, trans, picheight, picwidth, 'x');
	//д��ͼ���ļ�
	write(file, information, trans, picheight * picwidth, "out3.bmp");
	free(trans);


	//���д��еļ��α任
	//����λ�Ʊ任��bmp�ļ�������ͷ��Ϣ
	tagBITMAPFILEHEADER file2 = file;
	tagBITMAPINFOHEADER information2 = information;
	//�������������С
	double tan = 0.8;
	//���к������
	//����ָ���������������ϣ�x ����ƫ����
	long int width;
	width = picwidth + tan * picheight;
	//ת��Ϊ 8 �ı���
	width = ochange(width);
	bit* change = (bit*)malloc(sizeof(bit) * picheight * width * 3);
	//���к�����м��α任
	shear(rgbpic, change, picheight, picwidth, width, 'x', tan);
	//���´��к�ͼ���������ؿ�Ⱥ�ͼ�� bisizeimage ��С
	information2.biwidth = width;
	information2.bisizeimage = width * picheight / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file2.bfsize = width * picheight / 8;
	//д��ͼ���ļ�
	write(file2, information2, change, picheight * width, "out4.bmp");
	free(change);

	//�����������
	//����ָ���������������ϣ�x ����ƫ����
	long int height;
	height = picheight + tan * picwidth;
	//ת��Ϊ 8 �ı���
	height = ochange(height);
	bit* longch = (bit*)malloc(sizeof(bit) * height * picwidth * 3);
	//���к�����м��α任
	shear(rgbpic, longch, picheight, picwidth, height, 'y', tan);
	//���ô��к�ͼ���������ؿ�Ȳ�����ͼ�����س��Ⱥ�ͼ�� bisizeimage ��С
	information2.biwidth = picwidth;
	information2.biheight = height;
	information2.bisizeimage = height * picwidth / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file2.bfsize = picwidth * height / 8;
	//д��ͼ���ļ�
	write(file2, information2, longch, height* picwidth, "out5.bmp");
	free(longch);


	//�������ŵļ��α任
	//����λ�Ʊ任��bmp�ļ�������ͷ��Ϣ
	tagBITMAPFILEHEADER file3 = file;
	tagBITMAPINFOHEADER information3 = information;

	//�������ű�����x��y ���Ŵ�
	double changex = 2;
	double changey = 2;
	//������ͼ�������ظ����������ظ���
	height = picheight * changey;
	width = picwidth * changex;
	//���¶������س��� 8 ������
	height = ochange(height);
	width = ochange(width);
	bit* scalech = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scalech, picheight, picwidth, height, width, changex, changey);
	//����ͼ�����س������Ⱥ�ͼ�� bisizeimage ��С
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file3.bfsize = width * height / 8;
	//д��ͼ���ļ�
	write(file3, information3, scalech, height* width, "out6.bmp");
	free(scalech);

	//�������ű�����x���Ŵ�y ��С
	changex = 2;
	changey = 0.5;
	//������ͼ�������ظ����������ظ���
	height = picheight * changey;
	width = picwidth * changex;
	//���¶������س��� 8 ������
	height = ochange(height);
	width = ochange(width);
	bit* scalebs = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scalebs, picheight, picwidth, height, width, changex, changey);
	//����ͼ�����س������Ⱥ�ͼ�� bisizeimage ��С
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file3.bfsize = width * height / 8;
	//д��ͼ���ļ�
	write(file3, information3, scalebs, height* width, "out7.bmp");
	free(scalebs);

	//�������ű�����x ��С��y �Ŵ�
	changex = 0.5;
	changey = 2;
	//������ͼ�������ظ����������ظ���
	height = picheight * changey;
	width = picwidth * changex;
	//���¶������س��� 8 ������
	height = ochange(height);
	width = ochange(width);
	bit* scalesb = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scalesb, picheight, picwidth, height, width, changex, changey);
	//����ͼ�����س������Ⱥ�ͼ�� bisizeimage ��С
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file3.bfsize = width * height / 8;
	//д��ͼ���ļ�
	write(file3, information3, scalesb, height* width, "out8.bmp");
	free(scalesb);

	//�������ű�����x��y�� ��С
	changex = 0.5;
	changey = 0.5;
	//������ͼ�������ظ����������ظ���
	height = picheight * changey;
	width = picwidth * changex;
	//���¶������س��� 8 ������
	height = ochange(height);
	width = ochange(width);
	bit* scaless = (bit*)malloc(sizeof(bit) * height * width * 3);
	scale(rgbpic, scaless, picheight, picwidth, height, width, changex, changey);
	//����ͼ�����س������Ⱥ�ͼ�� bisizeimage ��С
	information3.biwidth = width;
	information3.biheight = height;
	information3.bisizeimage = height * width / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file3.bfsize = width * height / 8;
	//д��ͼ���ļ�
	write(file3, information3, scaless, height* width, "out9.bmp");
	free(scaless);


	//������ת�ļ��α任
	//����λ�Ʊ任��bmp�ļ�������ͷ��Ϣ
	tagBITMAPFILEHEADER file4 = file;
	tagBITMAPINFOHEADER information4 = information;
	//������Ҫ��ת�ĽǶ�
	double rota = pi / 24;
	//������ͼ�������ظ����������ظ���
	height = picheight * (sin(rota) + cos(rota));
	width = picwidth * (sin(rota) + cos(rota));
	//���¶������س��� 8 ������
	height = ochange(height);
	width = ochange(width);
	bit* rotatry = (bit*)malloc(sizeof(bit) * height * width * 3);
	rotation(rgbpic, rotatry, picheight, picwidth, height, width, rota);
	//����ͼ�����س������Ⱥ�ͼ�� bisizeimage ��С
	information4.biwidth = width;
	information4.biheight = height;
	information4.bisizeimage = height * width / 8 + 2;
	//���´��к�ͼ�����ֽڴ�С
	file4.bfsize = width * height / 8;
	//д��ͼ���ļ�
	write(file4, information4, rotatry, height* width, "out10.bmp");
	free(rotatry);

	return 0;
}

//��ת���α任����
void rotation(bit* rgbpic, bit* rotatry, long int picheight, long int picwidth, long int height, long int width, double rota)
{
	int i, j;
	int start;
	int begin;
	int hei, wid;
	int record;
	//�Ƚ��������ں�ɫ��������ʾ��ת��Ч��
	for (i = 0;i < height * width * 3;i++)
		rotatry[i] = 0;
	//Ѱ����ת��ͼ�����ص���ʼ���λ��
	start = picheight * sin(rota);
	//���� RGB ��ֵ�ֲ�
	start = start * 3;
	begin = start;
	for (i = 0;i < picheight;i++)
	{
		if (i != 0)
		{
			//ͨ����ʼλ��Ѱ��ԭʼͼ��ÿ���������ʼλ��
			hei = i * cos(rota);
			wid = i * sin(rota);
			//����ƫ�� RGB ��ֵ��λ��
			begin = start + hei * width * 3 - wid * 3;
		}
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			//��ÿһ����ʼλ�ÿ�ʼ��ͨ��ƫ��Ѱ��ԭʼͼ��ÿ�е�������λ��
			hei = j * sin(rota) / 3;
			wid = j * cos(rota) / 3;
			record = width * 3 * hei + begin + wid * 3;
			//��¼��Ӧ�� RGB ��ֵ
			copy(rotatry, rgbpic, record, picwidth, i, j);
		}
	}
	//��ת�󲿷ֵ���������ȡȡ��ԭ��δ���ܵ����� RGB ��ֵ��ͨ���ٽ�ֵ���������
	for (i = 0;i < height;i++)
		for (j = 3;j < width * 3 - 3;j = j + 3)
		{
			//�жϿ�ȱ����λ�ã��Ǻ�ɫ��������
			if (rotatry[i * width * 3 + j] == 0 && rotatry[i * width * 3 + j + 1] == 0 && rotatry[i * width * 3 + j + 2] == 0)
				if (rotatry[i * width * 3 + j - 3] != 0 && rotatry[i * width * 3 + j - 2] != 0 && rotatry[i * width * 3 + j - 1] != 0
					&& rotatry[i * width * 3 + j + 3] != 0 && rotatry[i * width * 3 + j + 4] != 0 && rotatry[i * width * 3 + j + 5] != 0)
				{
					//ʹ�����ٽ�ֵ�����
					rotatry[i * width * 3 + j] = rotatry[i * width * 3 + j + 3];
					rotatry[i * width * 3 + j + 1] = rotatry[i * width * 3 + j + 4];
					rotatry[i * width * 3 + j + 2] = rotatry[i * width * 3 + j + 5];
				}
		}
}

//���ż��α任����
void scale(bit* rgbpic, bit* scalech, long int picheight, long int picwidth, long int height, long int width, double changex, double changey)
{
	int x, y;
	int i, j, record;
	int que, pue;
	int flag1 = 0, flag2 = 0;
	//�Ƚ��������ڰ�ɫ�����������հ�
	for (i = 0;i < height * width * 3;i++)
		scalech[i] = 255;
	//�� x��y ������Ŵ�ʱ
	if (changex >= 1 && changey >= 1)
	{
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
			{
				//���շŴ���У�ÿ��ָ�����ظ�һԭͼ����ֵ
				record = changey * i * width * 3 + j * changex;
				scalech[record] = rgbpic[i * picwidth * 3 + j];
				scalech[record + 1] = rgbpic[i * picwidth * 3 + j + 1];
				scalech[record + 2] = rgbpic[i * picwidth * 3 + j + 2];
			}
	}
	//�� x �Ŵ�y ��Сʱ
	else if (changex >= 1 && changey < 1)
	{
		//�����С��
		que = round(1 / changey);
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
				if (i % que == 0)
				{
					//���ձ������� x ������ÿ��ָ�����ظ���ԭͼ����ֵ���� y ������ÿ��һ��ԭͼ����ȡһ��ͼ����
					record = round(i / que);
					record = record * width * 3 + j * changex;
					scalech[record] = rgbpic[i * picwidth * 3 + j];
					scalech[record + 1] = rgbpic[i * picwidth * 3 + j + 1];
					scalech[record + 2] = rgbpic[i * picwidth * 3 + j + 2];
				}
		//�����С����������������Ϳհ�
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			record = (height - 1) * width * 3 + j * changex;
			copy(scalech, rgbpic, record, picwidth, picheight - 1, j);
		}
	}
	//�� x ��С��y �Ŵ�ʱ
	else if (changex < 1 && changey >= 1)
	{
		//�����С��
		que = round(1 / changex);
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
				if (j % que == 0)
				{
					//���ձ������� y ������ÿ��ָ�����ظ���ԭͼ����ֵ���� x ������ÿ��һ��ԭͼ����ȡһ��ͼ����
					record = changey * i;
					record = record * width * 3 + j / que;
					copy(scalech, rgbpic, record, picwidth, i, j);
				}
		//�����С����������������Ϳհ�
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			record = (height - 1) * width * 3 + j * changex;
			copy(scalech, rgbpic, record, picwidth, picheight - 1, j);
		}
	}
	//�� x��y �������Сʱ
	else if (changex < 1 && changey < 1)
	{
		pue = round(1 / changey);
		que = round(1 / changex);
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
				if (j % que == 0 && i % pue == 0)
				{
					//���ձ������� x��y ������ÿ��һ��ԭͼ����ȡһ��ͼ����
					record = i / pue;
					record = record * width * 3 + j / que;
					copy(scalech, rgbpic, record, picwidth, i, j);
				}
		//�����С����������������Ϳհ�
		for (j = 0;j < picwidth * 3;j = j + 3)
		{
			record = (height - 1) * width * 3 + j * changex;
			copy(scalech, rgbpic, record, picwidth, picheight - 1, j);
		}

	}
	//����ȡֵ����Ŵ�����д����Ŀհ����ؼ�϶
	for (i = 0;i < height;i++)
		for (j = 0;j < width * 3;j = j + 3)
			//�ų���ĩβ���Զ���չ�������ֹ�����Ӱ�������걸��
			if (scalech[i * width * 3 + j] == 255 && j % (width * 3) != 0)
			{
				scalech[i * width * 3 + j] = scalech[i * width * 3 + j - 3];
				scalech[i * width * 3 + j + 1] = scalech[i * width * 3 + j - 2];
				scalech[i * width * 3 + j + 2] = scalech[i * width * 3 + j - 1];
			}
	//����ȡֵ����Ŵ�����д����Ŀհ����ؼ�϶
	for (j = 0;j < width * 3;j = j + 3)
	{
		for (i = height - 1;i >= 0;i--)
		{
			//�ų�������������ֹȡֵ�������
			if (scalech[i * width * 3 + j] == 255 && i != height - 1)
			{
				scalech[i * width * 3 + j] = scalech[(i + 1) * width * 3 + j];
				scalech[i * width * 3 + j + 1] = scalech[(i + 1) * width * 3 + j + 1];
				scalech[i * width * 3 + j + 2] = scalech[(i + 1) * width * 3 + j + 2];
			}
		}
	}
}

//���� RGB λ�÷ֲ����ض�����λ�������Ӧ RGB ��ֵ
void copy(bit* change, bit* rgbpic, int record, long int picwidth, int i, int j)
{
	//�����Ϊ 3 �ı�����ֱ�Ӹ����ӳ���ص� RGB ��ֵ 
	if (record % 3 == 0)
	{
		change[record] = rgbpic[i * picwidth * 3 + j];
		change[record + 1] = rgbpic[i * picwidth * 3 + j + 1];
		change[record + 2] = rgbpic[i * picwidth * 3 + j + 2];
	}
	//�� mod 3 �����Ϊ 0 ����������������������ʵ�������ʹ������ RGB ��ֵ��ԭͼһһ��Ӧ
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

//���м��α任��������
void shear(bit* rgbpic, bit* change, long int picheight, long int picwidth, long int width, char ch, double tan)
{
	int i, j, k = 0;
	int record;
	int height;
	//��������ɫ�����ɫ�������жϴ��к�ͼ�����״��
	for (i = 0;i < picheight * width * 3;i++)
		change[i] = 0;
	//������Ϊ x �����к������
	if (ch == 'x')
	{
		//�Դ��к�ͼ��ÿһ���� RGB ��ֵ�ֱ�ֵ
		for (i = 0;i < picheight;i++)
			for (j = 0;j < picwidth * 3;j = j + 3)
			{
				//�����ض��д���ƫ�����������������
				record = i * width * 3 + j + round(tan * (i + 1)) * 3;
				//���ض�����λ�������Ӧ�� RGB ��ֵ
				copy(change, rgbpic, record, picwidth, i, j);
			}
	}
	//������Ϊ'y', �����������
	else if (ch == 'y')
	{
		//���������ͼ������ RGB ��ֵ
		for (i = 0;i < picwidth * 3;i = i + 3)
			for (j = 0;j < picheight;j++)
			{
				//�����ض��д���ƫ�����������������
				record = j * picwidth * 3 + i + round(i / 3 * tan + 0.5) * picwidth * 3;
				//�ֱ����������ض�ӳ RGB ��ֵ
				change[record] = rgbpic[j * picwidth * 3 + i];
				change[record + 1] = rgbpic[j * picwidth * 3 + i + 1];
				change[record + 2] = rgbpic[j * picwidth * 3 + i + 2];
			}
	}
}

//���񼸺α任��������
void mirror(bit* rgbpic, bit* trans, long int height, long int width, char ch)
{
	int i, j;
	//���ѡ���y����о���任
	if (ch == 'y')
	{
		//����ͼ��ÿһ���� RGB ��ֵ�ֱ�ֵ
		for (i = 0;i < height;i++)
			for (j = 0;j < width * 3;j = j + 3)
			{
				trans[(i + 1) * width * 3 - j - 1] = rgbpic[i * width * 3 + j + 2];
				trans[(i + 1) * width * 3 - j - 2] = rgbpic[i * width * 3 + j + 1];
				trans[(i + 1) * width * 3 - j - 3] = rgbpic[i * width * 3 + j ];
			}
	}
	//���ѡ���x����о���任
	else if (ch == 'x')
	{
		//�ӵ����϶���ͼ������ؽ��� RGB ��ֵ
		for (i = 0;i < height;i++)
			for (j = 0;j < width * 3;j++)
				trans[(height - 1 - i) * width * 3 + j] = rgbpic[i * width * 3 + j];
	}
}

//λ�Ƽ��α任��������
void translation(bit* rgbpic, bit* copy, long int height, long int width, int x, int y)
{
	int i,j;
	//����λ�ƺ���ʼ��
	int startx, starty;
	//int widthbit = (fabs(x) + width) / 8;
	//�������ر���ֵ����Ϊ��ɫ�����ڹ۲�Ч����
	for (i = 0;i < (height + fabs(y)) * (fabs(x) + width) * 3;i++)
		copy[i] = 0;
	//��λ����Ϊ��������ͼ���ԭ�㿪ʼ���
	if (x < 0)
		startx = 0;
	//��λ����Ϊ��������ͼ��ʼ���λ��
	else
		startx = x;
	if (y < 0)
		starty = 0;
	else
		starty = y;
	//��ԭʼ������ֵ�����ӳ������λ����
	for (i = 0;i < height;i++)
		for (j = 0;j < width * 3;j++)
			copy[(i + starty) * (width + startx) * 3 + j + startx * 3] = rgbpic[i * width * 3 + j];
}

//����ֵ����ת��Ϊ 8 �ı���
int ochange(int number)
{
	double end;
	//�����������Ӧ����
	end = number % 8;
	number = number / 8;
	//������Ϊ0������ԭ����С����Ȼ������ȡΪ 8 �ı���
	if (end != 0)
		number = number * 8 + 8;
	else
		number = number * 8;
	return number;
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