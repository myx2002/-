//======================================================
//	analysis.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================
#include "analysis.h"
#include "string.h"
#include "strlib.h"
#include "displaybox.h"
#include "graphics.h"
#include "extgraph.h"
#include "linkedlist.h" 

static int recordTag;		//标签记录模式标志 
static int recordText;		//文本记录模式标志 

/*
 *	函数名: apeendString
 *
 *	功能：读入字符加到字符串 
 *
 *	接受形参: char*, 读入字符char	返回值: 无 
 */
void appendString(string a, char i)
{
	int p = strlen(a);
	a[p] = i;
	a[p + 1] = '\0';
}

/*
 *	函数名: dealInput
 *
 *	功能：处理传入的原文本字符串
 *
 *	接受形参: 字符串 s	返回值:	点击超链接时返回 1 其余 0 
 */
int dealInput(char *s)
{
	char nowtag[200] = {};							//存储标签 
	int i;
	for(i = 0; i < strlen(s); i++)
	{
		if(s[i]=='<')
		{
			if(recordText)							//当之前处于文本记录模式时 
			{
				if(dealText(nowtag))				//处理文本内容，且当点击超链接时返回1 
				{
					return 1; 
				}
			}
			recordText = 0;							//文本记录模式结束 
			recordTag = 1;							//标签记录模式开始 
			nowtag[0] = 0;							//通过设置第一个元素为0,使得appendString在调用strlen时从头开始赋值
		}
		else if(s[i]=='>')
		{	
			recordText = openTextInputMode(nowtag);	//当<>里的是标签库中的标签时，文本记录模式开启，设为 1
			dealTag(nowtag);   						//分析标签 
			recordTag = 0;							//标签记录模式结束，设为0 
			nowtag[0] = 0;							//通过设置第一个元素为0,使得appendString在调用strlen时从头开始赋值 
		}
		else
		{
			if(s[i]!='\n')
			appendString(nowtag, s[i]);				//记录<>内或外的内容 
		}
	}
	return 0;
}
