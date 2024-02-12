#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "manager.h" 
#include "menu.h"
int loginFlag = 0;  //管理员是否登录

int main()
{
    int sel;
	BookInfo *head;
    ManagerInfo *headManager;
    head = NULL;
    headManager = NULL;
    
    for (;;)
    {
        xianshi();
	    while (sel > 10 || sel < 0)
	    {
	        printf("选择有误！\n请重新输入：");
	        scanf("%d", &sel);
	    }
        if ((sel != 2 && (sel != 3 && sel != 10)) && loginFlag == 0)
		{   
			//除了浏览图书和搜索图书，其他功能需要管理员登录 
			printf("******请登录******\n");
			login(headManager);  //初始管理员：admin, 密码：123 
			if(loginFlag == 0)  continue;
		}
		switch (sel)
        {
	        case 1:
	            if (head == NULL)
	                head = CreateBooksList();
	            Insert(head);
	            break;
	        case 2:Print(head); break;
	        case 3:Search(head); break;
	        case 4:Update(head); break;
	        case 5:Delete(head); break;
	        case 6:Save(head); break;
	        case 7:
				if (headManager == NULL)
	                headManager = CreateManagersList();
	            InsertM(headManager);
	            break; 
	        case 8:DeleteM(headManager); break;
	        case 9:PrintM(headManager); break;
	        case 10:exit(0); break;
	        default:break;
        }
    }
    return 0;
}
