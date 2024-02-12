#include "library.h"

//******************����Ա��ز���************************ 
//�����鵥
BookInfo* CreateBooksList()
{
    BookInfo* head;
    head = (BookInfo*)malloc(sizeof(BookInfo));        //Ϊͷ������ռ�
    head->next = NULL;                                 //��ʼ��ͷָ��
    return head;
}
//�����¼
void Insert(BookInfo* head)
{
    BookInfo *b, *p;
    char flag = 'Y';
    p = head;
    while (p->next != NULL)
        p = p->next;
    //�����¿ռ䣬�洢�鼮��Ϣ������������
    while (flag == 'Y' || flag == 'y')
    {
        b = (BookInfo*)malloc(sizeof(BookInfo));       //�����¿ռ�
        printf("������ͼ���ţ�");                    //��ȡ�鼮��Ϣ
        fflush(stdin);                                 //��ջ�����
        scanf("%s", b->bnum);
        printf("������������");
        fflush(stdin);
        scanf("%s", b->bname);
        printf("���������ߣ�");
        fflush(stdin);
        scanf("%s", b->bauthor);
        printf("����������ţ�");
        fflush(stdin);
        scanf("%s", b->bclassfy);
        printf("������ͼ��۸�");
        fflush(stdin);
        scanf("%f", &b->bprice);
        p->next = b;                                  //�������ӵĽ���������
        p = b;                                        //ָ��p����ƶ���ָ��β���
        b->next = NULL;
        printf("��ӳɹ���\n������ӣ�(Y/N):");
        fflush(stdin);
        scanf("%c", &flag);
        if (flag == 'N' || flag == 'n')break;
        else if (flag == 'Y' || flag == 'y')continue;
	}
}
//ɾ����¼
void Delete(BookInfo* head)
{
    BookInfo *b, *p;
    char tmp[30];
    int flag;                                        //��־λ���ж��Ƿ��ҵ���Ҫɾ�����鼮
    flag = 0;
    b = head;
    p = head;
    printf("������Ҫɾ�����鼮����");
    fflush(stdin);
    scanf("%s", tmp);
    //��������
    while (p != NULL)
    {
        if (strcmp(p->bname, tmp) == 0)
        {
            flag = 1;
            break;
        }
        p = p->next;
    }
    if (flag == 1)
    {
        for (; b->next != p;)
            b = b->next;
        b->next = p->next;
        free(p);
        printf("ɾ���ɹ���\n");
    }
    else
        printf("���鲻���ڣ�");
    return;
}
//����鵥
void Print(BookInfo* head)
{
    BookInfo *p;
    if (head == NULL || head->next == NULL)                //�ж������Ƿ�Ϊ��
    {
        printf("�޼�¼��\n");
        return;
    }
    p = head;
//    printf("���������ש����������ש����������ש����������ש���������\n");
    printf("�� ��� ��   ����   ��   ����   �� ����� ��  �۸�  ��\n");
//    printf("�ǩ������贈���������贈���������贈���������贈��������\n");
    //������������鼮��Ϣ
    while (p->next != NULL)
    {
        p = p->next;
        printf("�� %-5s�� %-9s�� %-9s�� %-9s�� %.2lf  ��\n", p->bnum,
            p->bname, p->bauthor, p->bclassfy, p->bprice);
//      printf("���������ߩ����������ߩ����������ߩ����������ߩ���������\n");
    }
}
//�����鼮
void Search(BookInfo* head)
{
    BookInfo *p;
    char tmp[30];
    int flag = 0;
    p = head;
    if (head == NULL || head->next == NULL)
        printf("�嵥Ϊ�գ�\n");
    else
    {
        printf("������������");
        fflush(stdin);
        scanf("%s", tmp);
        while (p->next != NULL)
        {
            p = p->next;
            if (strcmp(p->bname, tmp) == 0)
            {
                flag = 1;                                //�鼮���ҵ�
                printf("��ţ�%s\n��������%s��\n���ߣ�%s\n���ࣺ%s\n�۸�%.2f\n",
                    p->bnum, p->bname, p->bauthor, p->bclassfy, p->bprice);
                return;
            }
            if (p->next == NULL)
                printf("\n��ѯ��ϣ�");
        }
        if (flag == 0)
            printf("û���ҵ���%s����\n", tmp);
    }
    return;
}
//�޸���Ϣ
void Update(BookInfo* head)
{
    BookInfo *p;
    int flag = 0;
    char tmp[30];
    p = head;
    printf("������������");
    fflush(stdin);
    scanf("%s", tmp);
    while (p->next != NULL)
    {
        p = p->next;
        if (strcmp(p->bname, tmp) == 0)
        {
            flag = 1;                                //��־�ҵ���Ҫ�޸ĵ��鼮
            printf("�������ţ�");
            fflush(stdin);
            scanf("%s", p->bnum);
            printf("������������");
            fflush(stdin);
            scanf("%s", p->bname);
            printf("���������ߣ�");
            fflush(stdin);
            scanf("%s", p->bauthor);
            printf("����������ţ�");
            fflush(stdin);
            scanf("%s", p->bclassfy);
            printf("������۸�");
            fflush(stdin);
            scanf("%f", &p->bprice);
        }
    }
    if (flag == 0)
        printf("û���ҵ���%s����\n", tmp);
    return;
}
//�����鵥���ļ�
void Save(BookInfo* head)
{
    BookInfo *p;
    FILE *fp;
    p = head;
    //��ֻд�ķ�ʽ���ļ�
    fp = fopen("C:\\Users\\admin\\Desktop\\bookslist.txt", "w");
    fprintf(fp, "���������ש����������ש����������ש����������ש���������\n");
    fprintf(fp, "�� ��� ��   ����   ��   ����   �� ����� ��  �۸�  ��\n");
    fprintf(fp, "�ǩ������贈���������贈���������贈���������贈��������\n");
    while (p->next != NULL)
    {
        p = p->next;
        fprintf(fp, "��%-6s��%-10s��%-10s��%-10s��%.2lf   ��\n",
            p->bnum, p->bname, p->bauthor, p->bclassfy, p->bprice);
        fprintf(fp, "���������ߩ����������ߩ����������ߩ����������ߩ���������\n");
    }
    fclose(fp);
    printf("����ɹ���\n");
    printf("�����ѳɹ����浽C:\\Users\\admin\\Desktop\\bookslist.txt\n");
}
