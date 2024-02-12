//======================================================
//	analysisTag.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================
#include "analysisTag.h"
#include "string.h"
#include "strlib.h"
#include "linkedlist.h"
#include "displaybox.h"
#include "graphics.h"
#include "extgraph.h" 
#include "enumType.h" 

char buffer[100];				//存储文本控件对应内容 
extern displayBox outputBox;
//标签库(不包含shiftTag) 
char *TextInputOpenTag[] = {"p", "title", "h1", "h2", "h3", "h4", "h5", "h6",
						  "b", "/b", "i", "/i", "sub", "/sub", "sup", "/sup",
						  "br", "strong", "/strong", "em", "/em", "blockquote",
						  "/blockquote", "q", "/q", "cite", "/cite", "dfn", "/dfn",
						  "abbr", "/abbr", "ins", "/ins", "del", "/del", "a", "/a", "li"};
//需要换行的结束标签 
char *shiftTag[] = {"/p", "/h1", "/h2", "/h3", "/h4", "/h5", "/h6", "/blockquote", "/li"};

enum printType textType = normalText;		 
						  
//存储标签属性和对应内容的结构 
typedef struct attribute{
	string attributeName;
	string attributeValue;
}attribute;

int list = 0;			//列表数量 
int listNum[10];		//用数组记录有序列表的列表项个数用（于输出序号） 

/*
 *	函数名: dealTag
 *
 *	功能：进一步读取标签，并执行相应操作 
 *
 *	接受形参: 完整标签 fulltag	返回值: 无 
 */
void dealTag(string fullTag)
{
	if(fullTag[0]=='!')									//该标签为注释
	{
		return;
	}
	string tag;
	linkedlistADT attributeLink=NULL;
	if(FindChar(' ', fullTag, 0)!=-1)					//当标签里有空格时 
	{
		tag = fullTag;									//把空格前面部分赋给tag 
		fullTag+=FindChar(' ', fullTag, 0);				//把空格开始后面部分赋给fullTag 
		fullTag[0] = 0;									//把空格改为'\0'分隔,此时fullTag[0]='\0' 
		fullTag++;							
		if(FindChar('=', fullTag, 0)!=-1)				//当fullTag中有'='时 
		{
			attributeLink = NewLinkedList();
			while(FindChar('=', fullTag, 0)!=-1)		//标签中可能有不止一个属性 
			{
				attribute* nowAttribute = (attribute*)malloc(sizeof(attribute));
				nowAttribute->attributeName = fullTag;	//把等号前面部分赋给atrributeName 
				fullTag+=FindChar('=', fullTag, 0);
				fullTag[0] = 0;
				fullTag[1] = 0;							//把'='和'"'赋为0  
				fullTag+=2;	
				nowAttribute->attributeValue = fullTag;	//把引号后面部分赋给attributeValue 
				ConvertToLowerCase(nowAttribute->attributeName);
				fullTag+=FindChar('"', fullTag, 0);
				fullTag[0] = 0;
				fullTag[1] = 0;
				fullTag+=2;
				InsertNode(attributeLink, NULL, nowAttribute);
			}
		}
	}
	else
	{
		tag = fullTag;
	}
	ConvertToLowerCase(tag);							//因为对大小不敏感，统一转化成小写判断 
	int num = sizeof(shiftTag)/sizeof(char*);	
	int i;
	for(i = 0; i < num; i++)
	{
		if(strcmp(tag,shiftTag[i])==0)
		{
			shift(&outputBox);							//如果tag是需要换行的结束标签就换行 
		}
	}
	//对结束标签的相应操作 
	if(tag[0]=='/')	
	{
		SetStyle(0);
		outputBox.fontSize = outputBox.globalFontSize;	//恢复默认值 
		if(!strcmp(tag, "/sup"))						//上标结束 
		{
			SetPointSize(outputBox.fontSize);
			outputBox.nowPrintY-=GetFontHeight() * 0.4;
		}
		if(!strcmp(tag, "/blockquote"))					//大块引用结束 
		{
			outputBox.boxSideGap-=outputBox.boxWdith / 8;
			resetBoxSideGap(&outputBox);
		}
		if(!strcmp(tag, "/q"))							//短引用结束 
		{
			writeInBox(CharToString('"'), &outputBox);			
		}
		if(!strcmp(tag, "/ul"))							//无序列表 
		{
			list--;
			printf("%d\n", list);
		}
		if(!strcmp(tag, "/ol"))							//有序列表 
		{
			listNum[list] = 0;
			list--;
		}
	}
	//对其余标签的相应操作 
	if(!strcmp(tag, "h1"))			//一级标题 
	{
		outputBox.fontSize = 50;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h2"))		//二级标题 
	{
		outputBox.fontSize = 45;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h3"))		//三级标题 
	{
		outputBox.fontSize = 40;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h4"))		//四级标题 
	{
		outputBox.fontSize = 35;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h5"))		//五级标题 
	{
		outputBox.fontSize = 30;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h6"))		//六级标题 
	{
		outputBox.fontSize = 25;
		SetStyle(1);
	}
	else if(!strcmp(tag, "p"))		//段落 
	{
		outputBox.fontSize = 20;
		SetStyle(0);
	}
	else if(!strcmp(tag, "b") || !strcmp(tag, "strong"))//粗体 
	{
		SetStyle(1);
	}
	else if(!strcmp(tag, "i") || !strcmp(tag, "em"))	//斜体 
	{
		SetStyle(2);
	}
	else if(!strcmp(tag, "sub") || !strcmp(tag, "sup"))	//下标 or上标 
	{
		int tmpFontSize = outputBox.fontSize;
		double nowHeight = GetFontHeight();
		do
		{
			tmpFontSize--;
			SetPointSize(tmpFontSize);
		}while(GetFontHeight() > nowHeight * 0.5);
		outputBox.fontSize = tmpFontSize;
		if(!strcmp(tag, "sup"))
		{
			outputBox.nowPrintY+=nowHeight * 0.4;
		}
	}
	else if(!strcmp(tag, "br"))							//换行 
	{
		shift(&outputBox);
	}
	else if(!strcmp(tag, "hr"))							//分割线 
	{
		drawHorizentalLine(&outputBox);
		shift(&outputBox); 
	}
	else if(!strcmp(tag, "blockquote")) 				//大块引用 
	{
		outputBox.boxSideGap+=outputBox.boxWdith / 8;	//增大页面边距 
		resetBoxSideGap(&outputBox);					//根据页面边距重新设置输出位置 
	}
	else if(!strcmp(tag, "q"))							//短引用 
	{
		writeInBox(CharToString('"'), &outputBox);
	}
	else if(!strcmp(tag, "ins"))						//下划线 
	{
		textType = insText;								//将文本类型设置为下划线类型
	}
	else if(!strcmp(tag, "del"))						//删除线 
	{
		textType = delText;								//将文本类型设置为删除线类型
	}
	else if(!strcmp(tag, "title"))						//网页标题 
	{
		textType = titleText;							//将文本类型设置为标题类型
	}
	else if(!strcmp(tag, "ul"))							//无序列表 
	{
		list++;
	}
	else if(!strcmp(tag, "ol"))							//有序列表 
	{
		list++;
		listNum[list]++;
	}
	else if(!strcmp(tag, "li"))							//定义列表项 
	{
		//每嵌套一个列表就多打一次空格段 
		int i;
		for(i = 1; i<=list; i++)
		{
			writeInBox("  ", &outputBox);				
		}
		//当是无序列表时 
		if(!listNum[list])
		{
			drawCircle(&outputBox, list);
		}
		//当是有序列表时 
		else
		{
			writeInBox(IntegerToString(listNum[list]), &outputBox);	//输出序号 
			writeInBox(".", &outputBox);	
			listNum[list]++;										//序号+1 
		}
	}
	else if(!strcmp(tag, "abbr"))			//缩写 
	{
		textType = abbrText;				//将文本类型设置为缩写类型 
		if(attributeLink!=NULL)
		{
			linkedlistADT tmp = attributeLink->next;
			while(tmp!=NULL)
			{
				//判断不同的属性 
				if(!strcmp("title", ((attribute*)(tmp->dataptr))->attributeName))	//判断 attributeName是否为 title 
				{
					strcpy(buffer, ((attribute*)(tmp->dataptr))->attributeValue);	//将 attrbuteValue赋给buffer 
				}
				tmp = tmp->next;
			}
		}
	}
	else if(!strcmp(tag, "a"))			//超链接 
	{
		textType = hrefText;			//将文本类型设置为超链接类型		
		if(attributeLink!=NULL)
		{
			linkedlistADT tmp = attributeLink->next;
			while(tmp!=NULL)
			{
				if(!strcmp("href", ((attribute*)(tmp->dataptr))->attributeName))	//判断 attributeName是否为 href 
				{
					strcpy(buffer, ((attribute*)(tmp->dataptr))->attributeValue);	//将 attrbuteValue赋给buffer 
				}
				tmp = tmp->next;
			}
		}
	}
}

/*
 *	函数名: openTextInputMode
 *
 *	功能：判断是否是库中含有的标签以决定是否读取后面的文本
 *
 *	接受形参: 读取的标签 nowTag	 返回值: 是-1 否-0 
 */
int openTextInputMode(string nowTag)
{
	string a = (string)malloc(sizeof(char) * (strlen(nowTag) + 1));
	strcpy(a, nowTag);
	int num = sizeof(TextInputOpenTag) / sizeof(char*);		//库中标签类型的数量 
	char tmp;
	string tag=a;
	//将标签后的空格赋值为'\0' 
	if(FindChar(' ', a, 0)!=-1)
	{
		
		tag+=FindChar(' ', a, 0);
		tmp = tag[0];
		tag[0] = 0;
	}
	//判断标签 
	int i;
	for(i = 0; i < num; i++)
	{
		if(strcmp(a, TextInputOpenTag[i])==0)	//如果是库中的一个标签返回1
		{		
			return 1;							//否则返回0
		}
	}
	free(a);
	return 0;
}
