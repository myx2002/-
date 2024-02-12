#ifndef _MANAGER_H
#define _MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//*********************����Ա�йز���********************************** 
typedef struct man
{
    char   name[10];                           //���� 
    char   password[30];                       //����                     
    struct man *nextM;                         //����ָ��
}ManagerInfo;

ManagerInfo* CreateManagersList();             //��������
void InsertM(ManagerInfo* headManager);        //����
void DeleteM(ManagerInfo* headManager);        //ɾ��
void PrintM(ManagerInfo* headManager);         //���
void login(ManagerInfo* head);

#endif
