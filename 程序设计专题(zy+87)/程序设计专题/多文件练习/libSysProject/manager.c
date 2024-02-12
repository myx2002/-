#include "manager.h" 
extern int loginFlag;

//******************管理员相关操作************************ 
//创建管理员 
ManagerInfo* CreateManagersList()
{
    ManagerInfo* headManager;
    headManager = (ManagerInfo*)malloc(sizeof(ManagerInfo));  //为头结点分配空间
	headManager->nextM = NULL;                                //初始化头指针
    return headManager;
}
//插入记录
void InsertM(ManagerInfo* headManager)
{
    ManagerInfo *b, *p;
    char flag = 'Y';
    p = headManager;
    while (p->nextM != NULL)
	{
    	p = p->nextM;	
    }
    //开辟新空间，存储管理员信息，并加入链表
    while (flag == 'Y' || flag == 'y')
    {
        b = (ManagerInfo*)malloc(sizeof(ManagerInfo));     //开辟新空间
        printf("请输入姓名：");                            //获取信息
        fflush(stdin);                                     //清空缓冲区
        scanf("%s", b->name);
        printf("请密码：");
        fflush(stdin);
        scanf("%s", b->password);
        p->nextM = b;                                      //将新增加的结点加入链表
        p = b;                                             //指针p向后移动，指向尾结点
        b->nextM = NULL;
        printf("添加成功！\n继续添加？(Y/N):");
        fflush(stdin);
        scanf("%c", &flag);
        if (flag == 'N' || flag == 'n')break;
        else if (flag == 'Y' || flag == 'y')continue;
    }
    return;
}
//删除记录
void DeleteM(ManagerInfo* head)
{
    ManagerInfo *b, *p;
    char tmp[30];
    int flag;                                             //标志位，判断是否找到了要删除的书籍
    flag = 0;
    b = head;
    p = head;
    printf("请输入要删除的管理员姓名：");
    fflush(stdin);
    scanf("%s", tmp);
    //遍历链表
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
        printf("删除成功！\n");
    }
    else
        printf("该管理员不存在！");
    return;
}
//浏览
void PrintM(ManagerInfo* head)
{
    ManagerInfo *p;
    if (head == NULL || head->nextM == NULL)                //判断链表是否为空
    {
        printf("无记录！\n");
        return;
    }
    p = head;
//    printf("┏━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━┓\n");
    printf("┃ 姓名 ┃   密码   ┃\n");
//    printf("┣━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━┫\n");
    //遍历链表，输出书籍信息
    while (p->nextM != NULL)
    {
        p = p->nextM;
        printf("┃ %-5s┃ %-9s┃\n", 
            p->name, p->password);
//      printf("┗━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━┛\n");
    }
}

void login(ManagerInfo* head)
{
	ManagerInfo *p;
    char tmp[30];
    char t2[50]; 
    p = head;
    
    printf("请输入姓名：");
    fflush(stdin);
    scanf("%s", tmp);
    printf("请输入密码：");
    fflush(stdin);
    scanf("%s", t2);
    if(strcmp("admin", tmp) == 0 && strcmp("123", t2) == 0)
	{
    	loginFlag = 1;                                        //已找到
        printf("登录成功\n");
        return;
    }
    if (head == NULL)
	{
    	printf("请输入默认管理员姓名和密码\n");
    	return;
    }
    if (head == NULL && head->nextM == NULL)
	{
    	printf("请输入默认管理员姓名和密码\n");
    }
    else
	{
        while (p->nextM != NULL)
        {
            p = p->nextM;
            if (strcmp(p->name, tmp) == 0 && strcmp(p->password, t2) == 0)
            {
                loginFlag = 1;                                //已找到
                printf("登录成功\n");
                return;
            }
        }
    }
    if (loginFlag == 0)
        printf("没有找到管理员：%s！\n", tmp);
    return;
}

