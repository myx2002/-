#include "menu.h"
int xianshi()
{
	//����˵�������ȡѡ��Ĺ���
	int sel;
	printf("\n           ͼ�����ϵͳ              \n");
	printf("������������������������������������ \n");
	printf("          1-ͼ����Ϣ¼��\n");
	printf("          2-ͼ����Ϣ���\n");
	printf("          3-ͼ����Ϣ��ѯ\n");
	printf("          4-ͼ����Ϣ�޸�\n");
	printf("          5-ͼ����Ϣɾ��\n");
	printf("          6-ͼ����Ϣ����\n");
	printf("          7-�����¹���Ա\n");
	printf("          8-ɾ������Ա\n");
	printf("          9-����Ա��Ϣ���\n");
	printf("          10-�˳�\n");
	printf("������������������������������������ \n");
	printf("��ѡ��");
	fflush(stdin);
	scanf("%d", &sel);
	return sel;
}

