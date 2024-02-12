#ifndef _MANAGER_H
#define _MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//*********************管理员有关操作********************************** 
typedef struct man
{
    char   name[10];                           //姓名 
    char   password[30];                       //密码                     
    struct man *nextM;                         //链表指针
}ManagerInfo;

ManagerInfo* CreateManagersList();             //创建链表
void InsertM(ManagerInfo* headManager);        //插入
void DeleteM(ManagerInfo* headManager);        //删除
void PrintM(ManagerInfo* headManager);         //浏览
void login(ManagerInfo* head);

#endif
