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
void drawHomePage(double x,double y,double r)		//������ҳ��ťͼ����(x,y)ΪԲ��λ�ã�rΪ��԰뾶 
{
	/*MovePen(x+r,y);
	DrawArc(r,0,359);*/			//�����Ƿ��ڰ�ťԲȦ�� 
	MovePen(x+0.5*r,y+0.15*r);	//���Ʒ��ݵײ�		
	DrawLine(0,-0.75*r);
	DrawLine(-1*r,0);
	DrawLine(0,0.75*r);
	
	MovePen(x-0.75*r,y+0.15*r);	//���Ʒ��ݶ���
	DrawLine(1.5*r,0);
	DrawLine(-0.75*r,0.5*r);
	DrawLine(-0.75*r,-0.5*r);
	
	MovePen(x,y-0.6*r);			//���Ʒ����ڲ���Ϣ 
	DrawLine(0,0.5*r);
	DrawLine(0.25*r,0);
	DrawLine(0,-0.5*r);
}
void Main(){
	InitGraphics();
	SetPenSize(2);
	double width=5*TextStringWidth("a");
	drawHomePage(1,1,width);
} 
