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
//���ƼӺ� 
void drawPlus(double x,double y,double r)			//(x,y)ΪԲ��λ�ã�rΪ��԰뾶 
{
	/*MovePen(x+r,y);
	DrawArc(r,0,359);*/				//Բ����Э�� 
	MovePen(x-0.6*r,y);
	DrawLine(1.2*r,0);
	MovePen(x,y-0.6*r);
	DrawLine(0,1.2*r);
}
//���Ƴ˺� 
void drawMultiplication(double x,double y,double r)	//(x,y)ΪԲ��λ�ã�rΪ��԰뾶 
{
	/*MovePen(x+r,y);
	DrawArc(r,0,359);*/			//Բ����Э�� 
	MovePen(x-0.5*r,y+0.5*r);
	DrawLine(r,-r);
	MovePen(x+0.5*r,y+0.5*r);
	DrawLine(-r,-r);
}

void Main(){
	InitGraphics();	
	SetPenSize(2);
	double width=2*TextStringWidth("a");				//��������ַ����� 
	drawPlus(1,2,width);
	drawMultiplication(2,2,width);
}
