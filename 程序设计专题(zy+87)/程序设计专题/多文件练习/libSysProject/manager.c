#include "manager.h" 
extern int loginFlag;

//******************����Ա��ز���************************ 
//��������Ա 
ManagerInfo* CreateManagersList()
{
    ManagerInfo* headManager;
    headManager = (ManagerInfo*)malloc(sizeof(ManagerInfo));  //Ϊͷ������ռ�
	headManager->nextM = NULL;                                //��ʼ��ͷָ��
    return headManager;
}
//�����¼
void InsertM(ManagerInfo* headManager)
{
    ManagerInfo *b, *p;
    char flag = 'Y';
    p = headManager;
    while (p->nextM != NULL)
	{
    	p = p->nextM;	
    }
    //�����¿ռ䣬�洢����Ա��Ϣ������������
    while (flag == 'Y' || flag == 'y')
    {
        b = (ManagerInfo*)malloc(sizeof(ManagerInfo));     //�����¿ռ�
        printf("������������");                            //��ȡ��Ϣ
        fflush(stdin);                                     //��ջ�����
        scanf("%s", b->name);
        printf("�����룺");
        fflush(stdin);
        scanf("%s", b->password);
        p->nextM = b;                                      //�������ӵĽ���������
        p = b;                                             //ָ��p����ƶ���ָ��β���
        b->nextM = NULL;
        printf("��ӳɹ���\n������ӣ�(Y/N):");
        fflush(stdin);
        scanf("%c", &flag);
        if (flag == 'N' || flag == 'n')break;
        else if (flag == 'Y' || flag == 'y')continue;
    }
    return;
}
//ɾ����¼
void DeleteM(ManagerInfo* head)
{
    ManagerInfo *b, *p;
    char tmp[30];
    int flag;                                             //��־λ���ж��Ƿ��ҵ���Ҫɾ�����鼮
    flag = 0;
    b = head;
    p = head;
    printf("������Ҫɾ���Ĺ���Ա������");
    fflush(stdin);
    scanf("%s", tmp);
    //��������
    while (p != NULL)
    {
        if (strcmp(p->name, tmp) == 0)
        {
            flag = 1;
            break;
        }
        p = p->nextM;
    }
    if (flag == 1)
    {
        for (; b->nextM != p;)
            b = b->nextM;
        b->nextM = p->nextM;
        free(p);
        printf("ɾ���ɹ���\n");
    }
    else
        printf("�ù���Ա�����ڣ�");
    return;
}
//���
void PrintM(ManagerInfo* head)
{
    ManagerInfo *p;
    if (head == NULL || head->nextM == NULL)                //�ж������Ƿ�Ϊ��
    {
        printf("�޼�¼��\n");
        return;
    }
    p = head;
//    printf("���������ש����������ש����������ש����������ש���������\n");
    printf("�� ���� ��   ����   ��\n");
//    printf("�ǩ������贈���������贈���������贈���������贈��������\n");
    //������������鼮��Ϣ
    while (p->nextM != NULL)
    {
        p = p->nextM;
        printf("�� %-5s�� %-9s��\n", 
            p->name, p->password);
//      printf("���������ߩ����������ߩ����������ߩ����������ߩ���������\n");
    }
}

void login(ManagerInfo* head)
{
	ManagerInfo *p;
    char tmp[30];
    char t2[50]; 
    p = head;
    
    printf("������������");
    fflush(stdin);
    scanf("%s", tmp);
    printf("���������룺");
    fflush(stdin);
    scanf("%s", t2);
    if(strcmp("admin", tmp) == 0 && strcmp("123", t2) == 0)
	{
    	loginFlag = 1;                                        //���ҵ�
        printf("��¼�ɹ�\n");
        return;
    }
    if (head == NULL)
	{
    	printf("������Ĭ�Ϲ���Ա����������\n");
    	return;
    }
    if (head == NULL && head->nextM == NULL)
	{
    	printf("������Ĭ�Ϲ���Ա����������\n");
    }
    else
	{
        while (p->nextM != NULL)
        {
            p = p->nextM;
            if (strcmp(p->name, tmp) == 0 && strcmp(p->password, t2) == 0)
            {
                loginFlag = 1;                                //���ҵ�
                printf("��¼�ɹ�\n");
                return;
            }
        }
    }
    if (loginFlag == 0)
        printf("û���ҵ�����Ա��%s��\n", tmp);
    return;
}

