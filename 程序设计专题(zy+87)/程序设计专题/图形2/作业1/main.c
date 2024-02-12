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
int index=0;					//���ƻ�ͼ״̬��� 
double mouse_x=0;
double mouse_y=0;				//��¼��굱ǰλ�� 
double last_x=0;
double last_y=0;				//��¼����ϴ��ƶ�ǰλ�� 
void display()
{
	SetPenColor("Blue");		//����������ɫ 
	if(index==1)
	{
		if(last_x==0&&last_y==0)	//�������һ�����£���¼��ǰ���λ�� 
		{
			MovePen(mouse_x,mouse_y);
			last_x=mouse_x;
			last_y=mouse_y;
		}else					   //������ƶ������ϸ������λ�ã��������߶� 
		{
			DrawLine(mouse_x-last_x,mouse_y-last_y);
			last_x=mouse_x;
			last_y=mouse_y;
		}
	}
	if(index==0)				  //����ſ�������λ�ó�ʼ״̬�������� 
	{
		last_x=0;
		last_y=0;
		DisplayClear();
	}	
}
void myMouseEvent(int x, int y,int button, int event)
{
	mouse_x=ScaleXInches(x);
	mouse_y=ScaleYInches(y);			//�������λ��(x,y) 
	switch(event)						 
	{
		case BUTTON_DOWN:			   //���������ʱ�������ͼ״̬ 
			if(button=LEFT_BUTTON)
				index=1;
			break;
		case BUTTON_UP:				  //������ɿ�ʱ��������ͼ״̬ 
			if(button=LEFT_BUTTON)
				index=0;
			break;
	} 
	display();
}
void Main()
{
	SetWindowTitle("��ҵ1");			
	InitGraphics();						//���ÿհ�ͼ�ο����� 
	registerMouseEvent(myMouseEvent);	//��ÿ������ƶ�������Ӧ��Ӧ 
	display();
}
