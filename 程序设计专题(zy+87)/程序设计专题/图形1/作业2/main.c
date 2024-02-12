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
#define pi 3.1415926
void forward(double distance);
void turn (double angle);
void move(double distance);
void liu();
void waiwei(double cx,double cy);
void dange();
void neiquan(double cx,double cy,int j);
double ang=0;

void Main()
{
	
	double cx,cy;
	int j=0;
	InitGraphics();
	SetPenColor("green");
	cx=GetWindowWidth()*1.0/2;
	cy=GetWindowHeight()*1.0/2;
	waiwei(cx,cy);
	for(j=0;j<3;j++)
	{
		neiquan(cx,cy,j);
	}
}
void forward(double distance)
{
	DrawLine(distance*cos(ang),distance*sin(ang));
}
void turn (double angle)
{
	ang=ang+angle;
}
void move(double distance)
{
	int x,y;
	x=GetCurrentX();
	y=GetCurrentY();
	MovePen(distance*cos(ang)+x,distance*sin(ang)+y);
}
void liu()
{
	int i;
	turn(pi*2/3);
	for(i=0;i<6;i++)
	{
		forward(1.5);
		turn(pi/3);
	}
	ang=0;
}
void waiwei(double cx,double cy)
{
	MovePen(cx,cy);
	move(1.5);
	liu();
	MovePen(cx,cy);
	turn(pi/9);
	move(1.5);
	liu();
	MovePen(cx,cy);
	turn(pi/9*2);
	move(1.5);
	liu();
}
void dange()
{
	turn(pi/3);
	forward(3*1.0/4);
	turn(pi/3);
	forward(3*1.0/4);
	turn(pi*2/3);
	forward(3*1.0/4);
	turn(pi/3);
	forward(3*1.0/4);
	ang=0;
}
void neiquan(double cx,double cy,int j)
{
	int i;
	for(i=0;i<6;i++)
	{
		MovePen(cx,cy-0.5);
		ang=j*pi*2/9;
		turn(i*pi/3);
		dange();
	}
}
