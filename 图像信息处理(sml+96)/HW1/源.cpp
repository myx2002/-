#include <stdio.h>
#include <stdlib.h>
#include "bmppart.h"

#pragma warning(disable:4996)					//����vs��fopen�İ�ȫ�Դ���

int main()
{
	//����ͼƬ��Ϣ���ձ���
	tagBITMAPFILEHEADER file;					
	tagBITMAPINFOHEADER information;
	//����ѭ��ָ����ͼ�񳤿�
	long int picwidth;
	long int picheight;
	long int i;

	//��ȡbmp�ļ�ͷ����Ϣ
	FILE* fp;
	fp = fopen("5.bmp", "rb");
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
	short* yuvpic = (short*)malloc(picwidth * picheight * sizeof(short) * 3);
	//���Ի�õ�rgb�Ƿ�Ϊ0-255������
	//printf("%d", 100 *rgbpic[89]);
	
	//���ù�ʽ��RGB����ת��ΪYUV����
	for (i = 0;i < picwidth * picheight;i++)
	{
		yuvpic[3 * i] = 0.11 * rgbpic[3 * i] + 0.59 * rgbpic[3 * i + 1] + 0.3 * rgbpic[3 * i + 2];
		yuvpic[3 * i + 1] = 0.493 * (rgbpic[3 * i] - yuvpic[3 * i]);
		yuvpic[3 * i + 2] = 0.877*(rgbpic[3 * i + 2] - yuvpic[3 * i]);
	}
	//���󲿷֣����Ե��ڱ���ɫ
	/*for (i = 0;i < picwidth * picheight * 3;i++)
	{
		if (yuvpic[i] < 0)
			yuvpic[i] = 0;	
		else if (yuvpic[i] >255)
			yuvpic[i] = 255;
	}*/

	//YUV�Ҷȵ���������Ҷ�bmpͼ��
	bit* rgbpic2 = (bit*)malloc(picwidth * picheight * 3 * sizeof(bit));
	for (long i = 0; i < picheight * picwidth; i++)
	{
		rgbpic2[3 * i] = yuvpic[3 * i];
		rgbpic2[3 * i + 1] = yuvpic[3 * i];
		rgbpic2[3 * i + 2] = yuvpic[3 * i];
	}

	//д�����ӡҪ��ͼƬ
	FILE* fp2;
	fp2 = fopen("output2.bmp", "wb");

	fwrite(&(file.bftype), 1, sizeof(file.bftype), fp2);
	fwrite(&(file.bfsize), 1, sizeof(file.bfsize), fp2);
	fwrite(&(file.bfreserved1), 1, sizeof(file.bfreserved1), fp2);
	fwrite(&(file.bfreserved2), 1, sizeof(file.bfreserved2), fp2);
	fwrite(&(file.bfoffbits), 1, sizeof(file.bfoffbits), fp2);
	fwrite(&information, 1, sizeof(information), fp2);
	fwrite(rgbpic2, sizeof(bit), picwidth * picheight * 3, fp2);

	free(rgbpic2);
	fclose(fp2);

	//�ı�bmpͼ������
	//��С����
	for (i = 0;i < picheight * picwidth;i++)
		yuvpic[3 * i] = yuvpic[3 * i] / 1.2;

	//��YUV��ʽת��ΪRGB��ʽ
	//��YUV��Ӧת��ֵ����255������255
	//��YUV��Ӧת��ֵС��0������0
	bit* rgbpic3 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	for ( i = 0; i < picwidth * picheight; i++) 
	{
		if (yuvpic[3 * i] + 2.0284 * yuvpic[3 * i + 1] < 0)
			rgbpic3[3 * i] = 0;
		else if (yuvpic[3 * i] + 2.0284 * yuvpic[3 * i + 1] > 255)
			rgbpic3[3 * i] = 255;
		else
			rgbpic3[3 * i] = yuvpic[3 * i] + 2.0284 * yuvpic[3 * i + 1];

		if (yuvpic[3 * i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] < 0)
			rgbpic3[3 * i + 1] = 0;
		else if (yuvpic[3 * i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] > 255)
			rgbpic3[3 * i + 1] = 255;
		else
			rgbpic3[3 * i + 1] = yuvpic[3 * i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2];

		if (yuvpic[3 * i] + 1.1403 * yuvpic[3 * i + 2] < 0)
			rgbpic3[3 * i + 2] = 0;
		else if (yuvpic[3 * i] + 1.1403 * yuvpic[3 * i + 2] > 255)
			rgbpic3[3 * i + 2] = 255;
		else
			rgbpic3[3 * i + 2] = yuvpic[3 * i] + 1.1403 * yuvpic[3 * i + 2];
	}

	//д�����ӡҪ��ͼƬ
	FILE* fp3;
	fp3 = fopen("output3.bmp", "wb");
	fwrite(&(file.bftype), 1, sizeof(file.bftype), fp3);
	fwrite(&(file.bfsize), 1, sizeof(file.bfsize), fp3);
	fwrite(&(file.bfreserved1), 1, sizeof(file.bfreserved1), fp3);
	fwrite(&(file.bfreserved2), 1, sizeof(file.bfreserved2), fp3);
	fwrite(&(file.bfoffbits), 1, sizeof(file.bfoffbits), fp3);
	fwrite(&information, 1, sizeof(information), fp3);
	fwrite(rgbpic3, sizeof(bit), picwidth * picheight * 3, fp3);

	free(rgbpic3);
	fclose(fp3);

	//��������
	//��Y�Ŵ�ֵ����255������255
	for (i = 0;i < picheight * picwidth;i++)
	{
		if (yuvpic[3 * i] * 1.5 > 255)
			yuvpic[3 * i] = 255;
		else 
			yuvpic[3 * i] = yuvpic[3 * i] * 1.5;
	}

	//��YUV��ʽת��ΪRGB��ʽ
	//��YUV��Ӧת��ֵ����255������255
	//��YUV��Ӧת��ֵС��0������0
	bit* rgbpic4 = (bit*)malloc(picheight * picwidth * sizeof(bit) * 3);
	for (i = 0; i < picwidth * picheight; i++)
	{
		if (yuvpic[3 * i] + 2.0284 * yuvpic[3 * i + 1] < 0)
			rgbpic4[3 * i] = 0;
		else if (yuvpic[3 * i] + 2.0284 * yuvpic[3 * i + 1] > 255)
			rgbpic4[3 * i] = 255;
		else
			rgbpic4[3 * i] = yuvpic[3 * i] + 2.0284 * yuvpic[3 * i + 1];

		if (yuvpic[3 * i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] < 0)
			rgbpic4[3 * i + 1] = 0;
		else if (yuvpic[3 * i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2] > 255)
			rgbpic4[3 * i + 1] = 255;
		else
			rgbpic4[3 * i + 1] = yuvpic[3 * i] - 0.3781 * yuvpic[3 * i + 1] - 0.5798 * yuvpic[3 * i + 2];

		if (yuvpic[3 * i] + 1.1403 * yuvpic[3 * i + 2] < 0)
			rgbpic4[3 * i + 2] = 0;
		else if (yuvpic[3 * i] + 1.1403 * yuvpic[3 * i + 2] > 255)
			rgbpic4[3 * i + 2] = 255;
		else
			rgbpic4[3 * i + 2] = yuvpic[3 * i] + 1.1403 * yuvpic[3 * i + 2];
	}

	//д�����ӡҪ��ͼƬ
	FILE* fp4;
	fp4 = fopen("output4.bmp", "wb");
	fwrite(&(file.bftype), 1, sizeof(file.bftype), fp4);
	fwrite(&(file.bfsize), 1, sizeof(file.bfsize), fp4);
	fwrite(&(file.bfreserved1), 1, sizeof(file.bfreserved1), fp4);
	fwrite(&(file.bfreserved2), 1, sizeof(file.bfreserved2), fp4);
	fwrite(&(file.bfoffbits), 1, sizeof(file.bfoffbits), fp4);
	fwrite(&information, 1, sizeof(information), fp4);
	fwrite(rgbpic4, sizeof(bit), picwidth * picheight * 3, fp4);

	free(rgbpic4);
	fclose(fp4);

	//�ͷ�ԭʼbmp�ļ�����
	free(rgbpic);
	free(yuvpic);
	fclose(fp);

	return 0;
}