#include "library.h"

//******************管理员相关操作************************ 
//创建书单
BookInfo* CreateBooksList()
{
    BookInfo* head;
    head = (BookInfo*)malloc(sizeof(BookInfo));        //为头结点分配空间
    head->next = NULL;                                 //初始化头指针
    return head;
}
//插入记录
void Insert(BookInfo* head)
{
    BookInfo *b, *p;
    char flag = 'Y';
    p = head;
    while (p->next != NULL)
        p = p->next;
    //开辟新空间，存储书籍信息，并加入链表
    while (flag == 'Y' || flag == 'y')
    {
        b = (BookInfo*)malloc(sizeof(BookInfo));       //开辟新空间
        printf("请输入图书编号：");                    //获取书籍信息
        fflush(stdin);                                 //清空缓冲区
        scanf("%s", b->bnum);
        printf("请输入书名：");
        fflush(stdin);
        scanf("%s", b->bname);
        printf("请输入作者：");
        fflush(stdin);
        scanf("%s", b->bauthor);
        printf("请输入类别编号：");
        fflush(stdin);
        scanf("%s", b->bclassfy);
        printf("请输入图书价格：");
        fflush(stdin);
        scanf("%f", &b->bprice);
        p->next = b;                                  //将新增加的结点加入链表
        p = b;                                        //指针p向后移动，指向尾结点
        b->next = NULL;
        printf("添加成功！\n继续添加？(Y/N):");
        fflush(stdin);
        scanf("%c", &flag);
        if (flag == 'N' || flag == 'n')break;
        else if (flag == 'Y' || flag == 'y')continue;
	}
}
//删除记录
void Delete(BookInfo* head)
{
    BookInfo *b, *p;
    char tmp[30];
    int flag;                                        //标志位，判断是否找到了要删除的书籍
    flag = 0;
    b = head;
    p = head;
    printf("请输入要删除的书籍名：");
    fflush(stdin);
    scanf("%s", tmp);
    //遍历链表
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
        printf("删除成功！\n");
    }
    else
        printf("该书不存在！");
    return;
}
//浏览书单
void Print(BookInfo* head)
{
    BookInfo *p;
    if (head == NULL || head->next == NULL)                //判断链表是否为空
    {
        printf("无记录！\n");
        return;
    }
    p = head;
//    printf("┏━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━┓\n");
    printf("┃ 编号 ┃   书名   ┃   作者   ┃ 类别编号 ┃  价格  ┃\n");
//    printf("┣━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━┫\n");
    //遍历链表，输出书籍信息
    while (p->next != NULL)
    {
        p = p->next;
        printf("┃ %-5s┃ %-9s┃ %-9s┃ %-9s┃ %.2lf  ┃\n", p->bnum,
            p->bname, p->bauthor, p->bclassfy, p->bprice);
//      printf("┗━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━┛\n");
    }
}
//查找书籍
void Search(BookInfo* head)
{
    BookInfo *p;
    char tmp[30];
    int flag = 0;
    p = head;
    if (head == NULL || head->next == NULL)
        printf("清单为空！\n");
    else
    {
        printf("请输入书名：");
        fflush(stdin);
        scanf("%s", tmp);
        while (p->next != NULL)
        {
            p = p->next;
            if (strcmp(p->bname, tmp) == 0)
            {
                flag = 1;                                //书籍已找到
                printf("编号：%s\n书名：《%s》\n作者：%s\n分类：%s\n价格：%.2f\n",
                    p->bnum, p->bname, p->bauthor, p->bclassfy, p->bprice);
                return;
            }
            if (p->next == NULL)
                printf("\n查询完毕！");
        }
        if (flag == 0)
            printf("没有找到《%s》！\n", tmp);
    }
    return;
}
//修改信息
void Update(BookInfo* head)
{
    BookInfo *p;
    int flag = 0;
    char tmp[30];
    p = head;
    printf("请输入书名：");
    fflush(stdin);
    scanf("%s", tmp);
    while (p->next != NULL)
    {
        p = p->next;
        if (strcmp(p->bname, tmp) == 0)
        {
            flag = 1;                                //标志找到所要修改的书籍
            printf("请输入编号：");
            fflush(stdin);
            scanf("%s", p->bnum);
            printf("请输入书名：");
            fflush(stdin);
            scanf("%s", p->bname);
            printf("请输入作者：");
            fflush(stdin);
            scanf("%s", p->bauthor);
            printf("请输入类别编号：");
            fflush(stdin);
            scanf("%s", p->bclassfy);
            printf("请输入价格：");
            fflush(stdin);
            scanf("%f", &p->bprice);
        }
    }
    if (flag == 0)
        printf("没有找到《%s》！\n", tmp);
    return;
}
//保存书单到文件
void Save(BookInfo* head)
{
    BookInfo *p;
    FILE *fp;
    p = head;
    //以只写的方式打开文件
    fp = fopen("C:\\Users\\admin\\Desktop\\bookslist.txt", "w");
    fprintf(fp, "┏━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━┓\n");
    fprintf(fp, "┃ 编号 ┃   书名   ┃   作者   ┃ 类别编号 ┃  价格  ┃\n");
    fprintf(fp, "┣━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━┫\n");
    while (p->next != NULL)
    {
        p = p->next;
        fprintf(fp, "┃%-6s┃%-10s┃%-10s┃%-10s┃%.2lf   ┃\n",
            p->bnum, p->bname, p->bauthor, p->bclassfy, p->bprice);
        fprintf(fp, "┗━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━┛\n");
    }
    fclose(fp);
    printf("保存成功！\n");
    printf("数据已成功保存到C:\\Users\\admin\\Desktop\\bookslist.txt\n");
}
