#ifndef _LIBRARY_H
#define _LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//*********************图书有关操作********************************** 
typedef struct book
{
    char  bnum[10];                        //书籍编号
    char  bname[30];                       //书名
    char  bauthor[20];                     //作者
    char  bclassfy[10];                    //类别编号
    float bprice;                          //价格
    struct book *next;                     //链表指针
}BookInfo;

BookInfo* CreateBooksList();               //创建链表
void Insert(BookInfo* head);               //插入
void Delete(BookInfo* head);               //删除
void Print(BookInfo* head);                //浏览
void Search(BookInfo* head);               //查询
void Update(BookInfo* head);               //修改
void Save(BookInfo* head);                 //保存

#endif
