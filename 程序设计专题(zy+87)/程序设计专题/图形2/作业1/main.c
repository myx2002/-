#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
int index=0;					//控制绘图状态与否 
double mouse_x=0;
double mouse_y=0;				//记录鼠标当前位置 
double last_x=0;
double last_y=0;				//记录鼠标上次移动前位置 
void display()
{
	SetPenColor("Blue");		//调整画笔颜色 
	if(index==1)
	{
		if(last_x==0&&last_y==0)	//若左键第一次摁下，记录当前鼠标位置 
		{
			MovePen(mouse_x,mouse_y);
			last_x=mouse_x;
			last_y=mouse_y;
		}else					   //随鼠标移动，不断更新鼠标位置，并绘制线段 
		{
			DrawLine(mouse_x-last_x,mouse_y-last_y);
			last_x=mouse_x;
			last_y=mouse_y;
		}
	}
	if(index==0)				  //左键放开后，重置位置初始状态，并清屏 
	{
		last_x=0;
		last_y=0;
		DisplayClear();
	}	
}
void myMouseEvent(int x, int y,int button, int event)
{
	mouse_x=ScaleXInches(x);
	mouse_y=ScaleYInches(y);			//或许鼠标位置(x,y) 
	switch(event)						 
	{
		case BUTTON_DOWN:			   //当左键按下时，进入绘图状态 
			if(button=LEFT_BUTTON)
				index=1;
			break;
		case BUTTON_UP:				  //当左键松开时，跳出绘图状态 
			if(button=LEFT_BUTTON)
				index=0;
			break;
	} 
	display();
}
void Main()
{
	SetWindowTitle("作业1");			
	InitGraphics();						//设置空白图形框及名称 
	registerMouseEvent(myMouseEvent);	//对每次鼠标移动做出相应响应 
	display();
}
