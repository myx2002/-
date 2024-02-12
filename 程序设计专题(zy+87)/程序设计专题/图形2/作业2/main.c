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
#include "imgui.h"
#include <time.h>
double winheight;
double winwidth; 		//�����Ļ״̬��ȫ������  
double xz;
double yz;				//������ĸλ�� 
double speed = 0.06;	//�����ַ������ٶ� 
double currenttime = 0; //��¼��ʱ 
int mem = -1;			//��¼��������λ��
int gamestart = 0;		//������Ϸ��ʼ״̬ 
int gamecontinue = 0;	//������ĸ��Ϸ״̬ 
int totalnum = 0;		//��¼���а��ܸ��� 
int yzz;				//��¼���а񳤶�
int score = 0;			//ͳ�Ƶ÷� 
int change = 0;			//������ɫ�任 
char str[2];
char randomcolor;		//��ɫ�任 	
struct{					//��¼����100�� 
	int score;
	double currenttime;
	int year;			//��¼������ 
	int month;
	int day;
}reserve[100];
void timegetting( int index )						//��ȡ��ǰϵͳʱ�䲢��ֵ 
{
	time_t nowtime = time( 0 );
	struct tm *timeinfo = localtime( &nowtime );
	reserve[index].year = timeinfo->tm_year + 1900;	//����ת��Ϊ��ǰ������ 
	reserve[index].month = timeinfo->tm_mon + 1;
	reserve[index].day = timeinfo->tm_mday;
} 
void swaprank( int i )		   //Ϊ���а��������� 
{
	int j = 0,index = -1;
	int cscore, cyear, cmonth, cday;
	int year, month, day;
	double ccurrenttime;
	for( ; j<i ; j++ )						//�Ƚ�score��currenttime��С 
	{
		if( score>reserve[j].score )
		{								//��¼������ʼλ�� 
			index = j;
			break;
		}
		else if( score==reserve[j].score&&currenttime<reserve[j].currenttime )
		{
			index=j;
			mem=j;
			break;
		}
	}
	for( ; index<i ; index++ )				//�ɸ��������� 
	{
		cscore = reserve[index].score;
		ccurrenttime = reserve[index].currenttime;
		cyear = reserve[index].year;
		cmonth = reserve[index].month;
		cday = reserve[index].day;
		reserve[index].score = score;
		reserve[index].currenttime = currenttime;
		if( index!=j )						//ԭλ�������ձ��� 
		{
			reserve[index].year = year;
			reserve[index].month = month;
			reserve[index].day = day;
		}
		score = cscore;
		currenttime = ccurrenttime;
		year = cyear;
		month = cmonth;
		day = cday;
	}
	reserve[index].score = score;			//����ֵ 
	reserve[index].currenttime = currenttime;
	reserve[index].year = year;
	reserve[index].month = month;
	reserve[index].day = day;	
	timegetting( j );
}
void displayrank()
{
	int i = 0, t = 0;
	FILE *fp = fopen( "record.txt", "r" );
	fscanf( fp, "%d\n", &totalnum );
	for( i=0; i<totalnum; i++ )							//����ԭ������
		fscanf( fp, "%d %lf %d %d %d\n", &reserve[i].score, &reserve[i].currenttime,
			&reserve[i].year, &reserve[i].month, &reserve[i].day );
	fclose( fp );
	swaprank( totalnum );
	fp=fopen( "record.txt","w" );
	fprintf( fp, "%d\n", totalnum+1 );						//�ܸ�����һ 
	for( ; t<totalnum+1; t++ )								//���������������� 
		fprintf( fp, "%d %.2lf %d %d %d\n", reserve[t].score, reserve[t].currenttime,
			reserve[i].year, reserve[i].month, reserve[i].day);
	fclose( fp );
}
void drawranklevel()
{
	int t,i;
	double fH = GetFontHeight();
	FILE *fp2 = fopen( "record.txt","r" );				    //����ԭ������
	fscanf( fp2, "%d\n", &totalnum );
	for( i = 0; i<totalnum; i++ )							
		fscanf( fp2, "%d %lf %d %d %d\n", &reserve[i].score, &reserve[i].currenttime,
			&reserve[i].year, &reserve[i].month, &reserve[i].day );
	fclose( fp2 );
	if( totalnum == 0 )										//��������� 
			drawLabel( winwidth/2-1, winheight/2, "��ʱû��������Ϣ" );
	else if( totalnum >= 2 )
	{
		MovePen( winwidth/2-1.5, yzz+fH );
		DrawTextString( "���а�" ); 						//����������� 
		yzz=winheight-1.5;
		for( t = 0; t<totalnum; t++ )
		{
			if( yzz-fH*t<=3 )
				break;
			if( t == mem )
				SetPenColor( "green" );						//�������γɼ� 
			MovePen( winwidth/2-0.5, yzz-fH*t );
			DrawTextString( IntegerToString( reserve[t].score ) );
			SetPenColor( "blue" );
		}
	}
}
void drawMenu()							//���Ʋ˵��� 
{
	int selection;
	static char*menuListFile[] = {		//�˵��� 
		"����",	"��ʼ",	"��ͣ",	"����",	"����",	"�˳�",
	};
	static char*menuListTool[] = {
		"����","����","���а�",
	}; 
	int i = 0, t = 0;					//��������ͳ�� 
	double x = 0, y=winheight;			//�˵�����λ 
	double fH = GetFontHeight();
	double w = TextStringWidth( menuListFile[0]) * 2;
	drawMenuBar( 0, y-fH * 1.5, GetWindowWidth(), fH * 1.5);
	selection = menuList( GenUIID(0), x, y - fH * 1.5, w + 0.5, w + 0.5, fH * 1.5, 
		menuListFile, sizeof( menuListFile )/sizeof( menuListFile[0] ) );
	if( selection == 1 )					//�˵�����������Ӧ 
	{									//��ʼ 
		gamestart = 1;
		startTimer( 0, 50 );
	}
	else if( selection == 2 )			//��ͣ 
	{
		gamestart = 2;
		cancelTimer( 0 );
		drawLabel( xz, yz-fH * 8.5, str);
	}
	else if( selection == 3 )			//���� 
	{
		if( speed <= 0.1 )
			speed = speed + 0.02;
		else
			speed = 0.1;
	}
	else if( selection==4 ) 			//���� 
	{
		if( speed >= 0.02 )
			speed = speed - 0.02;
		else
			speed = 0.02;
	}
	else if( selection == 5 )			//�˳� 
	{
		gamestart = -1;
		if( score != 0 )
			displayrank();
		drawranklevel();
	}
	selection = menuList( GenUIID(0), x + w + 0.5, y - fH * 1.5,w + 0.5, w + 0.5, fH * 1.5,
		menuListTool, sizeof( menuListTool )/sizeof( menuListTool[0] ) );
	if( selection == 1 )				//��������������Ӧ 
	{									//���� 
		cancelTimer( 0 );
		gamestart = 3;
	}
	else if( selection == 2 )			//���а� 
		gamestart = 4;
	if( gamestart == 4 )				//��ʾ���а� 
		drawranklevel();
	if( gamestart == 3 )				//��ʾ�����ı� 
	{
		MovePen (winwidth / 2 - 1, winheight / 2 );
		DrawTextString( "����һ���Ʒ��ҵ���Ϸ" );
	}
}
void drawbuttons()
{													//��ť���Ƽ���Ӧ 
	double fH = GetFontHeight();
	if( button( GenUIID( 0 ), 1, 0.5, 1, 0.5, "��ʼ" ) )
	{
		gamestart = 1;
		startTimer( 0, 50 );
	}
	if( button( GenUIID( 0 ), 2.5, 0.5, 1, 0.5, "��ͣ" ) )
	{
		gamestart = 2;
		cancelTimer( 0 );
		drawLabel( xz, yz - fH * 8.5, str );
	}
	if( button( GenUIID( 0 ), 4, 0.5, 1, 0.5, "����" ) )
	{
		if( speed <= 0.1 )
			speed = speed + 0.02;
		else
			speed = 0.1;
	} 
	if( button( GenUIID( 0 ), 5.5, 0.5, 1, 0.5, "����" ) )
	{
		if( speed >= 0.02 )
			speed = speed - 0.02;
		else
			speed = 0.02;
	}
	if( button( GenUIID( 0 ), 7, 0.5, 1, 0.5, "�˳�" ) )
	{
		gamestart = -1;
		if( score != 0 )
			displayrank();
		drawranklevel();
	}			
	if( gamestart == 2 )
		drawLabel( xz, yz - fH * 8.5, str );
	if( gamestart == -1 )
		drawranklevel();
}
void drawGamezone()									//������Ϸ���� 
{
	int i;
	double y = winheight;
	double fH = GetFontHeight(); 
	static char text[100];
	if( gamestart == 1 && gamecontinue == 0 )		//��������ַ� 
	{
		str[0] = RandomInteger( 1, 26 ) - 1 + 'a';
		str[1] = '\0';
		xz = RandomInteger( 1, 5 ); 
		gamecontinue = 1;
	}
	if( change == 0 )									//�÷����߿���ɫ�任 
		SetPenColor( "blue" );
	else if( change == 1 )
		SetPenColor( "randomcolor" );
	MovePen( 0, 1.1 );									//�÷����߿���� 
	DrawLine( winwidth, 0 );
	MovePen( 0, y - fH * 7.5 );
	DrawLine( winwidth, 0 );
	SetPenColor( "blue" );
	MovePen( winwidth - 2, y - fH * 3 );				//������ʾ������ 
	DrawTextString( "�÷�:" );
	DrawTextString( IntegerToString( score ) );
	textbox( GenUIID( 0 ), 0, 1.1, winwidth, 0.4, text, sizeof( text ) );	//����� 
	if( gamestart == 1 && gamecontinue == 1 )									//��ĸ���� 
	{
		drawLabel( xz, yz - fH * 8.5, str );
		for( i = 0; text[i]; i++ )
		{
			if( text[i] == str[0] )									//���ַ�������ȷ��score���� 
			{
				score = score + 10;
				gamecontinue = 0;
				yz = winheight;
				for( i = 0; text[i]; i++ )
					text[i] = '\0';
				break;
			}
		}
	}
}
void display()											//����ҳ�� 
{
	DisplayClear();
	drawbuttons();
	drawMenu();
	drawGamezone();
}
void KeyboardEventProcess( int key, int event ) 	   	//��Ӧ���� 
{
	uiGetKeyboard( key, event) ;
	if( gamestart != -1 )
		display();
} 
void TimerEventProcess( int id )						//ʱ����� 
{
	if(id == 0 && gamestart == 1 && gamecontinue == 1 )	//�ַ�����		 
	{
		if( yz <= 3 )
		{											//�ж��ַ��������λ�� 
			gamecontinue = 0;
			yz = winheight;
		}
		else if( yz > 3 )
			yz = yz - speed;	
	}
	if( id == 0 && gamestart == 1 )
		currenttime = currenttime + 50;
	if( id == 1 && gamestart == 1 )					//��ĸȥ�߿�ɫ 
	{
		change = 1;
		DefineColor( "randomcolor", RandomInteger( 1 ,255 ) * 1.0 / 255, RandomInteger( 1, 255 ) * 1.0 / 255, 
			RandomInteger( 1, 255 ) * 1.0 / 255 );	
	}
	if( gamestart != -1 )
		display();
} 
void CharEventProcess( char ch )				   	//�ַ���Ӧ 
{
	uiGetChar( ch );
	if( gamestart != -1 )
		display();
}
void MouseEventProcess( int x, int y, int button, int event )   //�����Ӧ 
{
	uiGetMouse( x, y, button, event );
	display();
}
void Main()
{
	SetWindowTitle( "��ҵ2" );
	InitGraphics();
	winheight = GetWindowHeight();
	winwidth = GetWindowWidth();
	xz = winwidth / 2;
	yz = winheight;
	registerMouseEvent( MouseEventProcess ); 
	registerKeyboardEvent( KeyboardEventProcess );
	registerCharEvent( CharEventProcess );
	registerTimerEvent( TimerEventProcess );
	startTimer( 0, 50 );
	startTimer( 1, 10000 );
	display();
}
