//======================================================
//	page.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================

#ifndef __PAGE_H__
#define __PAGE_H__

#include"linkedlist.h"
#include"strlib.h"

//自定义一个page结构，用于记录文件名（网址）、网页名、文件的内容、前后页、 
typedef struct page{
	char location[30];						//文件名（网址） 
	char webPageName[30];					//网页名 
	linkedlistADT text;						//存储一份文件所有内容 
	linkedlistADT textPageContent[20];		//存储文件的每一页输入内容 
	linkedlistADT outputPageContent[20];	//存储文件的每一页输出内容 
	char formalPage[30];					//上一页文件 
	char nextPage[30];						//下一页文件 
	int textPageNum;						//HTML文本总页数  
	int outputPageNum;						//解析内容总页数 
	int ithTextPage;						//当前HTML文本页码 
	int ithOutputPage;						//当前解析内容页码 
}page;

/*
 *	函数名: loadingPage
 *
 *	功能：加载文件并读取文件内容 
 *
 *	接受形参: page结构指针 nowpage, 文件名(网址）pageLocation 返回值: 无 
 */
void loadingPage(page *nowPage, string pageLocation);

/*
 *	函数名: writrInPageName
 *
 *	功能：写入网页名字 
 *
 *	接受形参: page结构指针 nowpage	字符串a	返回值: 无 
 */
void writeInPageName(page *nowPage, string a);

/*
 *	函数名: cancelPage
 *
 *	功能：关闭网页 
 *
 *	接受形参: 关闭的网页位序 now	返回值: 无 
 */
void cancelPage(int ithWebPage);

#endif 
