//======================================================
//	analysisText.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================
#include "analysisText.h"
#include "strlib.h"
#include "string.h" 
#include "display.h"
#include "displaybox.h"
#include "widget.h"
#include "enumType.h"
#include "page.h"

extern displayBox outputBox;		//输出框
extern page PageList[10];			//最多保存10个网页 
extern int ithWebPage; 				//当前网页 
extern char buffer[100];			//存储文本内容 
extern textType;					//枚举文本类型 
extern lineLocation;				//枚举划线位置 
extern lineType;					//枚举线条类型 

/*
 *	函数名: deleteChar
 *
 *	功能：删除指定区域的字符 
 *
 *	接受形参: 待处理字符串 a, 起始 i, j	返回值: 无 
 */
static void deleteChar(string a, int i, int j)
{
	int length = StringLength(a);
	if(i > length)
	{
		return;						//当指定起点大于字符串有效长度时返回 
	}
	else if(j > length)			
	{
		a[i] = 0;					//当指定结尾大于有效长度时直接将起点设为'\0' 
	}
	else
	{
		int p;
		for(p = j + 1; p<=strlen(a); p++)
		{
			a[i + p - j - 1] = a[p];//正常删除时通过后面的字符向前平移实现 
		}
	}
}

/*
 *	函数名: dealText
 *
 *	功能：输出不同类型的文本 
 *
 *	接受形参: 输出字符串 a	返回值: 无 
 */
int dealText(string a)
{
	if(textType==titleText)		//标题文本 
	{
		writeInPageName(&PageList[ithWebPage], a);
		textType = normalText;
		return 0;
	}
	int blank = 0;	//判断空格 
	int select = 0;	//判断保留字符 
	//删除多余空格并寻找保留字符 
	int i;
	for(i = 0;i < StringLength(a); i++)
	{
		if(blank&&a[i]==' ')
		{
			deleteChar(a, i, i);
			i--; 
		}
		else if(a[i]==' ')
		{
			blank = 1;			//删除多余的空格 
		}
		else if(a[i]!=' ')
		{
			blank = 0;			//恢复blank状态 
		}
		
		if(a[i]=='&')			//保留字符均以&开头且&为保留字符 
		{
			select = 1;			//进入保留字符状态 
		}
		if(select==1)
		{
			//判断保留字符并删除实体字符 
			if(a[i+1]=='l' && a[i+2]=='t')						//判断lt输出'<'
			{
				a[i] = '<';
				deleteChar(a, i + 1, i + 2);						
			}
			else if(a[i+1]=='#' && a[i+2]=='6' && a[i+3]=='0')  //判断#60输出'<'
			{
				a[i] = '<';
				deleteChar(a, i + 1, i + 3);
			}
			else if(a[i+1]=='g' && a[i+2]=='t')					//判断gt输出'>'
			{
				a[i] = '>';
				deleteChar(a,i+1,i+2);						
			}
			else if(a[i + 1]=='#' && a[i + 2]=='6' && a[i + 3]=='2')	//判断#62输出'>'
			{
				a[i] = '>';
				deleteChar(a, i + 1, i + 3);	
			}
			else if(a[i + 1]=='n' && a[i + 2]=='b' && 
					a[i + 3]=='s' && a[i + 4]=='p')  					//判断nbsp输出' ' 不收到空格消除影响 
			{
				a[i] = ' ';
				deleteChar(a, i + 1, i + 4);
			}
			else if(a[i + 1]=='#' && a[i + 2]=='1' && 
					a[i + 3]=='6' && a[i + 4]=='0')						//判断#160输出' ' 不收到空格消除影响 
			{
				a[i] = ' ';
				deleteChar(a, i + 1, i + 4);	
			}
			else if(a[i + 1]=='a' && a[i + 2]=='m' && a[i + 3]=='p')	//判断amp，输出'&' 
			{
				a[i] = '&';
				deleteChar(a, i + 1, i + 3);								
			}
			else if(a[i + 1]=='#' && a[i + 2]=='3' && a[i + 3]=='8')	//判断#38，输出'&' 
			{
				a[i] = '&';
				deleteChar(a, i + 1, i + 3);
				}
			else if(a[i + 1]=='t' && a[i + 2]=='i' &&
					a[i + 3]=='m' && a[i + 4]=='e' && a[i + 5]=='s')	//判断times,输出'*' 
			{
				a[i] = '*';
				deleteChar(a, i + 1, i + 5);
			}
			else if(a[i + 1]=='#' && a[i + 2]=='2' &&
					a[i + 3]=='1' && a[i + 4]=='5')						//判断#215，输出'*' 
			{
				a[i] = '*';
				deleteChar(a, i + 1, i + 4);	
			}
			else if(a[i + 1]=='d' && a[i + 2]=='i'&& 
					a[i + 3]=='v' && a[i + 4]=='i'&&
					a[i + 5]=='d' && a[i + 6]=='e')						//判断divide,输出'/' 
			{
				a[i] = '/';
				deleteChar(a, i + 1, i + 6);	
			}
			else if(a[i + 1]=='#' && a[i + 2]=='2' && 
					a[i + 3]=='4' && a[i + 4]=='7')						//判断#247，输出'/' 
			{
				a[i] = '/';
				deleteChar(a, i + 1, i + 4);
			}
		}
		select = 0;					//重置状态 
	}
	if(textType==normalText)		//正常输出非标签文本 
	{
		writeInBox(a, &outputBox);	
	}
	else if(textType==insText)		//下划线 
	{
		drawLineWithText(&outputBox, a, 0, 0, NULL);	 
		textType = normalText;
	}
	else if(textType==delText)		//删除线 
	{
		drawLineWithText(&outputBox, a, 1, 0, NULL);
		textType = normalText;
	}
	else if(textType==abbrText)		//缩写 
	{
		createTextWidget(a, abbrText, buffer);
		textType = normalText;
	}
	else if(textType==hrefText)		//超链接 
	{
		textType = normalText;
		string tmp = buffer;
		if(createTextWidget(a, hrefText, tmp))	//当点击了创建的超链接的区域后返回1 
		{
			return 1;
		}
	}
	return 0;
}
