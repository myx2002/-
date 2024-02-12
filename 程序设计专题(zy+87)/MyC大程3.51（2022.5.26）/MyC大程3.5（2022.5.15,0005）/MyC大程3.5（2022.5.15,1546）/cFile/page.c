//======================================================
//	page.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/5
//
//======================================================
#include "page.h"
#include"linkedlist.h"
#include"stdio.h"
#include"strlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "string.h"

extern char fileName[100];									//文件地址（网址） 
extern page PageList[10];									//最多同时存在10个网页 
extern int ithWebPage; 										//当前网页
extern int webPageNum;									    //当前总网页数 

/*
 *	函数名: loadingPage
 *
 *	功能：加载文件并读取文件内容  
 *
 *	接受形参: page结构指针 nowpage, 文件名(网址）pageLocation 返回值: 无 
 */
void loadingPage(page *nowPage, string pageLocation)
{
	if(strcmp(nowPage->location,pageLocation))				//如果加载的不是当前页面 
	{
		if(nowPage->location[0]!=0)							//如果原先不是空地址（针对于"后退"功能的实现） 
		{
			strcpy(nowPage->formalPage, nowPage->location);	//把原先页面地址赋值给 "上一页面 " 
		}		
	}

	FILE* fp;
	fp = fopen(pageLocation, "r+");				//打开对应的实际文件 
	strcpy(fileName, pageLocation);				//将加载的文件地址（网址）赋给fileName 
	strcpy(nowPage->location, pageLocation);		//将加载的文件地址（网址）赋给当前页面的location 
	//如果打开失败
	if(fp==NULL)
	{
		nowPage->location[0] = '\0';
		return;
	}
	//读取文件，一行一个节点创建链表 
	char buffer[200];					
	FreeLinkedList(nowPage->text);				//释放原先页面的文本内容链表 
	nowPage->text = NULL;
	nowPage->text = NewLinkedList();				//创建新页面的文本内容链表 
	//对每一行文本创建一个链表节点 
	while(!feof(fp))
	{
		fgets(buffer,200,fp);
		char *tmp = (char *)malloc(sizeof(char)*(strlen(buffer) + 1));	//+1是为了给'\0'留下位置 
		strcpy(tmp, buffer);
		InsertNode(nowPage->text, NULL, tmp);
	}
	fclose(fp);
	//初始化页面属性 
	nowPage->outputPageNum = 0;
	nowPage->textPageNum = 0;
	nowPage->ithOutputPage = 0;
	nowPage->ithTextPage = 0;
	int i;
	for(i = 0; i < 20; i++)
	{
		nowPage->textPageContent[i] = 0;
		nowPage->outputPageContent[i] = 0;		
	}
	nowPage->textPageContent[nowPage->ithTextPage] = nowPage->text;
	nowPage->outputPageContent[nowPage->ithOutputPage] = nowPage->text;
	nowPage->webPageName[0] = 0;
}

/*
 *	函数名: writrInPageName
 *
 *	功能：写入网页名字 
 *
 *	接受形参: page结构指针 nowpage	字符串a	返回值: 无 
 */
void writeInPageName(page *nowPage,string a)
{
	strcpy(nowPage->webPageName, a);
}

/*
 *	函数名: cancelPage
 *
 *	功能：关闭网页 
 *
 *	接受形参: 关闭的网页位序 now	返回值: 无 
 */
void cancelPage(int canceledWebPage)
{

	//当删除的是最后一个网页 
 	if(canceledWebPage==webPageNum)
 	{
  		memset(PageList + webPageNum, 0, sizeof(page));			//将最后一个网页清零 
		webPageNum--;
		ithWebPage = webPageNum;
  		return;
 	}
 	//当删除的是中间的网页 
 	int i;
	for(i = canceledWebPage; i < webPageNum; i++)
 	{
  		memcpy(PageList + i, PageList + i + 1, sizeof(page));	//依次将下一个网页的内容复制给上一个网页 
 	}
 	memset(PageList + webPageNum, 0, sizeof(page));				//将原先最后一个网页清零 
  	webPageNum--;

}
