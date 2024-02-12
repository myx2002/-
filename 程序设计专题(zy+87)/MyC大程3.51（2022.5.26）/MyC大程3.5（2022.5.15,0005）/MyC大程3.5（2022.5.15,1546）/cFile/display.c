//======================================================
//	display.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================

#include"display.h"
#include"string.h"
#include"stdio.h"
#include"linkedlist.h"
#include"displaybox.h"
#include"analysis.h"
#include"widget.h"
#include"strlib.h"
#include"imgui.h"
#include"page.h"
#include "graphics.h"
#include "extgraph.h"
#include "icon.h"


extern double windowHeight;					//界面高度 
extern double windowWidth;					//界面宽度 
char fileName[100] = "source/homepage.txt";	//文件名 
extern displayBox textBox, outputBox;		//展示框属性 
extern page PageList[20];					//记录最多10个页面 
extern int ithWebPage; 						//当前页数 
extern int webPageNum;						//总页数

//函数声明：将文件内容通过链表打在输入框中 
static void printNodeIntoTextBox(void *obj); 

/*
 *	函数名: display
 *
 *	功能：绘制浏览器
 */
void display()
{
	DisplayClear();		//清空

	drawTextBox();		//绘制地址栏 
	drawInputBox(); 	//输入框 
	drawOutputBox();	//输出框 
	drawButton();		//绘制按钮
	drawPageBar();		//绘制标签页栏 
	drawMenu();			//绘制菜单栏 
}

/*
 *	函数名: drawMenu
 *
 *	功能：绘制菜单栏，实现菜单和快捷键功能 
 */
void drawMenu()
{
	double menuBarHeight = windowHeight / 20;
	drawMenuBar(0, windowHeight - menuBarHeight, windowWidth, menuBarHeight);	//绘制长栏 
	
	static char * menuListMenu[] = {"菜单",
		"主页 | Ctrl-H",
		"刷新 | Ctrl-R",
		"后退 | Ctrl-B",
		"前进 | Ctrl-A",
		"关闭 | Ctrl-Q", 
		"新建 | Ctrl-T"};
	int selection;
	//绘制菜单 
	double menuWidth = windowHeight / 15;
	double wlist = TextStringWidth(menuListMenu[1]) * 1.2;
	selection = menuList(GenUIID(0), menuWidth / 50 + windowHeight * 0.18, windowHeight - menuBarHeight, menuWidth,
						wlist, menuBarHeight, menuListMenu, sizeof(menuListMenu) / sizeof(menuListMenu[0]));
	//菜单响应
	switch (selection)
	{
		case 1:
			reactHome();				//主页
			break;
		case 2:
			reactCircleReflesh();		//刷新
			break;
		case 3:
			reactLeftArrow();			//后退
			break;
		case 4:
			reactRightArrow();			//前进
			break;
		case 5:
			cancelPage(ithWebPage);		//关闭
			break; 
		case 6:
			reactPlus();				//新建
			break;
		default:
			break;
	}	//因为菜单栏的区域和标签页栏的区域重合，所以需要在打开菜单栏期间让标签栏失效，通过库里面的变量unfoldMenu控制 
}

/*
 *	函数名: drawButton
 *
 *	功能：绘制按钮
 */
void drawButton()
{
	double buttonWidth = windowHeight / 10;		//按钮宽度 
	double buttonHeight = windowHeight / 20;	//按钮高度 
	SetPointSize(20);
	//绘制"跳转"按钮并判断按下 
	if(button(GenUIID(0), windowWidth / 2 - buttonWidth,
			  windowHeight - buttonHeight, buttonWidth, buttonHeight, "跳转"))
	{
		if(fopen(fileName, "r")==NULL)				//如果输入的网址不可以打开 
		{
			return;
		 } 
		loadingPage(&PageList[ithWebPage], fileName);	//加载文件（网页） 
	}
	if(button(GenUIID(0), 0, 
			  0, 1.5, 0.6, "前翻"))					//当前输入页码 -1 
	{
		if(PageList[ithWebPage].ithTextPage - 1>=0)
		PageList[ithWebPage].ithTextPage--;	
		display();
	}
	if(button(GenUIID(0), 1.5,
			  0, 1.5, 0.6, "后翻"))					//当前输出页码 +1
	{	
		if(PageList[ithWebPage].ithTextPage + 1<=PageList[ithWebPage].textPageNum)
		PageList[ithWebPage].ithTextPage++;
		display();		 
	}
	if(button(GenUIID(0), 11,
			  0, 1.5, 0.6, "前翻"))					//当前输入页面 -1 
	{
		if(PageList[ithWebPage].ithOutputPage - 1>=0)
		PageList[ithWebPage].ithOutputPage--;
		display();	
	}
	if(button(GenUIID(0), 12.5,
			  0, 1.5, 0.6, "后翻"))					//当前输出页码 +1
	{
		if(PageList[ithWebPage].ithOutputPage + 1<=PageList[ithWebPage].outputPageNum)
		PageList[ithWebPage].ithOutputPage++;
		display();	
	}
	
	//当鼠标移动到刷新、后退、前进、主页图标时绘制底圆 
	drawRoundButton(windowWidth / 50, windowHeight - windowHeight / 40, 
			windowHeight * 0.05 * 0.4, drawCircleReflesh, reactCircleReflesh, NULL);
	drawRoundButton(windowWidth / 50 + windowHeight * 0.04, windowHeight - windowHeight / 40,
			windowHeight * 0.05 * 0.4, drawLeftArrow, reactLeftArrow, permitLeftArrow);
	drawRoundButton(windowWidth / 50 + windowHeight * 0.08, windowHeight - windowHeight / 40, 
			windowHeight * 0.05 * 0.4, drawRightArrow, reactRightArrow, permitRightArrow);
	drawRoundButton(windowWidth / 50 + windowHeight * 0.12, windowHeight - windowHeight / 40,
			windowHeight * 0.05 * 0.4, drawHome, reactHome, NULL);
}

/*
 *	函数名: drawTextBox
 *
 *	功能：绘制地址栏 
 */
void drawTextBox()
{
	double menuBarHeight = windowHeight / 20;
	textbox(GenUIID(0), windowWidth / 2, windowHeight - menuBarHeight,
			windowWidth, menuBarHeight, fileName, 100); 
}

/*
 *	函数名: drawOutputBox
 *
 *	功能：打印输出框内容并在需要翻页时将该处节点作为新一页的表头 
 */
void drawOutputBox()
{
	drawDisplayBox(&outputBox);
	reset(&outputBox);
	//将输出框文本打印起点重新定位到开头 
	if(PageList[ithWebPage].text!=NULL)													
	{
		linkedlistADT nodeptr=NULL;
		if(!PageList[ithWebPage].ithOutputPage)															//当输出框为第0页时（实际上为第一页） 
		{
			nodeptr = PageList[ithWebPage].outputPageContent[PageList[ithWebPage].ithOutputPage]->next;	//表头不存放数据，所以第零页与其他页区分	
		}
		else
		{
			nodeptr = PageList[ithWebPage].outputPageContent[PageList[ithWebPage].ithOutputPage];		//获得对应页码的链表首节点
		}
		//开始输出内容 
		while (nodeptr!=NULL && nodeptr!=PageList[ithWebPage].outputPageContent[PageList[ithWebPage].ithOutputPage + 1]) 
		{																								//第二个条件是为了控制不继续打印下一页的内容 
			if(dealInput(nodeptr->dataptr)==1)															//当点击超链接时 
			{
				return;
			}
			if(outputBox.nowPrintY<outputBox.positionY)													//当打印至展示框底部时，增加新的输入页
			{
				PageList[ithWebPage].outputPageNum++;											 		//输入总页数加1
				PageList[ithWebPage].outputPageContent[PageList[ithWebPage].outputPageNum] = nodeptr;	//此处节点赋给新一页的链表头 
				display();
				break; 
			}
			nodeptr = nodeptr->next;																	//进行下一个节点操作
		}
	}
	//绘制页码 
	SetPenColor("blue");
	MovePen(outputBox.positionX + outputBox.boxWdith - TextStringWidth("page:") -
			TextStringWidth(IntegerToString(PageList[ithWebPage].ithOutputPage + 1)), outputBox.positionY-GetFontHeight());
	DrawTextString("page:");
	DrawTextString(IntegerToString(PageList[ithWebPage].ithOutputPage + 1));
}

/*
 *	函数名: drawInputBox
 *
 *	功能：打印输入框内容并在需要翻页时将该处节点作为新一页的表头  
 */
void drawInputBox()
{
	reset(&textBox);
	drawDisplayBox(&textBox);
	if(PageList[ithWebPage].text!=NULL)
	{
		linkedlistADT nodeptr;
		//根据按钮"前翻 ""后翻 "来改变当前页码（ithTextPage) 
		if(!PageList[ithWebPage].ithTextPage)														//当前页为第零页时（实际上为第一页） 
		{
			nodeptr = PageList[ithWebPage].textPageContent[PageList[ithWebPage].ithTextPage]->next;	//表头不存放数据，所以第零页与其他页区分		
		}
		else
		{
			nodeptr = PageList[ithWebPage].textPageContent[PageList[ithWebPage].ithTextPage];		//获得对应页码的链表首节点	
		}
		
		while (nodeptr != NULL&&nodeptr!=PageList[ithWebPage].textPageContent[PageList[ithWebPage].ithTextPage+1]) 
																									//第二个条件是为了控制不继续打印下一页的内容 
		{
			printNodeIntoTextBox(nodeptr->dataptr);													//打印出每个节点存储的数据 
			if(textBox.nowPrintY<textBox.positionY)													//当打印至展示框底部时，增加新的输入页 
			{
				PageList[ithWebPage].textPageNum++;													//输入总页数加1 
				PageList[ithWebPage].textPageContent[PageList[ithWebPage].textPageNum] = nodeptr;	//此处节点赋给新一页的链表头 
				display();								
				break; 
			}
			nodeptr = nodeptr->next;																//进行下一个节点操作 
		}	
	}
	else
	{
		writeInBox("找不到该文件", &textBox);
	}
	//绘制页码 
	MovePen(textBox.positionX + textBox.boxWdith - TextStringWidth("page:") -
			TextStringWidth(IntegerToString(PageList[ithWebPage].ithTextPage + 1)),textBox.positionY - GetFontHeight());
	DrawTextString("page:");
	DrawTextString(IntegerToString(PageList[ithWebPage].ithTextPage + 1));	
}

/*
 *	函数名: printNodeIntoTextBox
 *
 *	功能：将文件内容输出在输入框中
 *
 *	接受形参： 指向输出内容的指针 obj  返回值：无 
 */
void printNodeIntoTextBox(void *obj)
{
	writeInBox(obj, &textBox);
	shift(&textBox);
}

/*
 *	函数名: drawPageBar
 *
 *	功能：绘制网页标签栏
 *		  当鼠标点击某标签栏时将网址赋给"当前 
 */
void drawPageBar()
{
 	double menuBarHeight = windowHeight / 20;
 	double pageTagLength = webPageNum>=4?windowWidth/(webPageNum + 2):windowWidth / 5;		//当标签页过多时标签页栏缩短 
 	SetFont("黑体"); 
 	SetPointSize(17);
		
	 //绘制标签页长栏
	 int i; 
 	for(i = 0; i<=webPageNum; i++)
 	{
 		int tmp = drawPage(pageTagLength * i, windowHeight - 2 * menuBarHeight,
		 pageTagLength, menuBarHeight, PageList[i].webPageName, i);
 		//返回值=1说明鼠标点击了该标签，返回2说明关闭网页 
  		if(tmp==1)
  		{
   			ithWebPage = i;
   			strcpy(fileName, PageList[ithWebPage].location);								//将点击的网页地址赋给fileName 
  		}
  		else if(tmp==2)
  		{
  			cancelPage(i);
		}
 	}
 	drawRoundButton(pageTagLength * (webPageNum + 1) + 0.5, windowHeight - 1.5 * menuBarHeight,	//绘制新建按钮图标 
	 							menuBarHeight * 0.35, drawPlus, reactPlus, permitPlus);
 	drawMenuBar(0, windowHeight - 2 * menuBarHeight, windowWidth, menuBarHeight); 					//绘制标签页长栏
	//通过白色线条形成标签页栏与网页内容连通的效果 
	if(ithWebPage > webPageNum)
	{
		ithWebPage--;
	}
	SetPenColor("white");
	MovePen(pageTagLength * ithWebPage, windowHeight - 2 * menuBarHeight);
	DrawLine(pageTagLength, 0);
	SetPenColor("blue"); 
}
