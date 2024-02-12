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
double winwidth; 		//获得屏幕状态的全部变量  
double xz;
double yz;				//控制字母位置 
double speed = 0.06;	//控制字符下落速度 
double currenttime = 0; //记录用时 
int mem = -1;			//记录本次数据位置
int gamestart = 0;		//控制游戏开始状态 
int gamecontinue = 0;	//控制字母游戏状态 
int totalnum = 0;		//记录排行榜总个数 
int yzz;				//记录排行榜长度
int score = 0;			//统计得分 
int change = 0;			//控制颜色变换 
char str[2];
char randomcolor;		//颜色变换 	
struct{					//记录数据100个 
	int score;
	double currenttime;
	int year;			//记录年月日 
	int month;
	int day;
}reserve[100];
void timegetting( int index )						//获取当前系统时间并赋值 
{
	time_t nowtime = time( 0 );
	struct tm *timeinfo = localtime( &nowtime );
	reserve[index].year = timeinfo->tm_year + 1900;	//秒数转变为当前年月日 
	reserve[index].month = timeinfo->tm_mon + 1;
	reserve[index].day = timeinfo->tm_mday;
} 
void swaprank( int i )		   //为排行榜数据排序 
{
	int j = 0,index = -1;
	int cscore, cyear, cmonth, cday;
	int year, month, day;
	double ccurrenttime;
	for( ; j<i ; j++ )						//比较score与currenttime大小 
	{
		if( score>reserve[j].score )
		{								//记录交换起始位置 
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
	for( ; index<i ; index++ )				//由高至低排序 
	{
		cscore = reserve[index].score;
		ccurrenttime = reserve[index].currenttime;
		cyear = reserve[index].year;
		cmonth = reserve[index].month;
		cday = reserve[index].day;
		reserve[index].score = score;
		reserve[index].currenttime = currenttime;
		if( index!=j )						//原位置年月日保留 
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
	reserve[index].score = score;			//最后项赋值 
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
	for( i=0; i<totalnum; i++ )							//读入原有数据
		fscanf( fp, "%d %lf %d %d %d\n", &reserve[i].score, &reserve[i].currenttime,
			&reserve[i].year, &reserve[i].month, &reserve[i].day );
	fclose( fp );
	swaprank( totalnum );
	fp=fopen( "record.txt","w" );
	fprintf( fp, "%d\n", totalnum+1 );						//总个数加一 
	for( ; t<totalnum+1; t++ )								//输出添加且排序数据 
		fprintf( fp, "%d %.2lf %d %d %d\n", reserve[t].score, reserve[t].currenttime,
			reserve[i].year, reserve[i].month, reserve[i].day);
	fclose( fp );
}
void drawranklevel()
{
	int t,i;
	double fH = GetFontHeight();
	FILE *fp2 = fopen( "record.txt","r" );				    //读入原有数据
	fscanf( fp2, "%d\n", &totalnum );
	for( i = 0; i<totalnum; i++ )							
		fscanf( fp2, "%d %lf %d %d %d\n", &reserve[i].score, &reserve[i].currenttime,
			&reserve[i].year, &reserve[i].month, &reserve[i].day );
	fclose( fp2 );
	if( totalnum == 0 )										//无数据输出 
			drawLabel( winwidth/2-1, winheight/2, "暂时没有排名信息" );
	else if( totalnum >= 2 )
	{
		MovePen( winwidth/2-1.5, yzz+fH );
		DrawTextString( "排行榜：" ); 						//输出满框数据 
		yzz=winheight-1.5;
		for( t = 0; t<totalnum; t++ )
		{
			if( yzz-fH*t<=3 )
				break;
			if( t == mem )
				SetPenColor( "green" );						//高亮本次成绩 
			MovePen( winwidth/2-0.5, yzz-fH*t );
			DrawTextString( IntegerToString( reserve[t].score ) );
			SetPenColor( "blue" );
		}
	}
}
void drawMenu()							//绘制菜单栏 
{
	int selection;
	static char*menuListFile[] = {		//菜单栏 
		"设置",	"开始",	"暂停",	"加速",	"减速",	"退出",
	};
	static char*menuListTool[] = {
		"关于","帮助","排行榜",
	}; 
	int i = 0, t = 0;					//已有排名统计 
	double x = 0, y=winheight;			//菜单栏定位 
	double fH = GetFontHeight();
	double w = TextStringWidth( menuListFile[0]) * 2;
	drawMenuBar( 0, y-fH * 1.5, GetWindowWidth(), fH * 1.5);
	selection = menuList( GenUIID(0), x, y - fH * 1.5, w + 0.5, w + 0.5, fH * 1.5, 
		menuListFile, sizeof( menuListFile )/sizeof( menuListFile[0] ) );
	if( selection == 1 )					//菜单栏各部分响应 
	{									//开始 
		gamestart = 1;
		startTimer( 0, 50 );
	}
	else if( selection == 2 )			//暂停 
	{
		gamestart = 2;
		cancelTimer( 0 );
		drawLabel( xz, yz-fH * 8.5, str);
	}
	else if( selection == 3 )			//加速 
	{
		if( speed <= 0.1 )
			speed = speed + 0.02;
		else
			speed = 0.1;
	}
	else if( selection==4 ) 			//减速 
	{
		if( speed >= 0.02 )
			speed = speed - 0.02;
		else
			speed = 0.02;
	}
	else if( selection == 5 )			//退出 
	{
		gamestart = -1;
		if( score != 0 )
			displayrank();
		drawranklevel();
	}
	selection = menuList( GenUIID(0), x + w + 0.5, y - fH * 1.5,w + 0.5, w + 0.5, fH * 1.5,
		menuListTool, sizeof( menuListTool )/sizeof( menuListTool[0] ) );
	if( selection == 1 )				//工具栏各部分响应 
	{									//帮助 
		cancelTimer( 0 );
		gamestart = 3;
	}
	else if( selection == 2 )			//排行榜 
		gamestart = 4;
	if( gamestart == 4 )				//显示排行榜 
		drawranklevel();
	if( gamestart == 3 )				//显示帮助文本 
	{
		MovePen (winwidth / 2 - 1, winheight / 2 );
		DrawTextString( "这是一个逼疯我的游戏" );
	}
}
void drawbuttons()
{													//按钮绘制及响应 
	double fH = GetFontHeight();
	if( button( GenUIID( 0 ), 1, 0.5, 1, 0.5, "开始" ) )
	{
		gamestart = 1;
		startTimer( 0, 50 );
	}
	if( button( GenUIID( 0 ), 2.5, 0.5, 1, 0.5, "暂停" ) )
	{
		gamestart = 2;
		cancelTimer( 0 );
		drawLabel( xz, yz - fH * 8.5, str );
	}
	if( button( GenUIID( 0 ), 4, 0.5, 1, 0.5, "加速" ) )
	{
		if( speed <= 0.1 )
			speed = speed + 0.02;
		else
			speed = 0.1;
	} 
	if( button( GenUIID( 0 ), 5.5, 0.5, 1, 0.5, "减速" ) )
	{
		if( speed >= 0.02 )
			speed = speed - 0.02;
		else
			speed = 0.02;
	}
	if( button( GenUIID( 0 ), 7, 0.5, 1, 0.5, "退出" ) )
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
void drawGamezone()									//绘制游戏界面 
{
	int i;
	double y = winheight;
	double fH = GetFontHeight(); 
	static char text[100];
	if( gamestart == 1 && gamecontinue == 0 )		//随机产生字符 
	{
		str[0] = RandomInteger( 1, 26 ) - 1 + 'a';
		str[1] = '\0';
		xz = RandomInteger( 1, 5 ); 
		gamecontinue = 1;
	}
	if( change == 0 )									//得分区边框颜色变换 
		SetPenColor( "blue" );
	else if( change == 1 )
		SetPenColor( "randomcolor" );
	MovePen( 0, 1.1 );									//得分区边框绘制 
	DrawLine( winwidth, 0 );
	MovePen( 0, y - fH * 7.5 );
	DrawLine( winwidth, 0 );
	SetPenColor( "blue" );
	MovePen( winwidth - 2, y - fH * 3 );				//分数显示区绘制 
	DrawTextString( "得分:" );
	DrawTextString( IntegerToString( score ) );
	textbox( GenUIID( 0 ), 0, 1.1, winwidth, 0.4, text, sizeof( text ) );	//输入框 
	if( gamestart == 1 && gamecontinue == 1 )									//字母绘制 
	{
		drawLabel( xz, yz - fH * 8.5, str );
		for( i = 0; text[i]; i++ )
		{
			if( text[i] == str[0] )									//若字符输入正确，score增加 
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
void display()											//重置页面 
{
	DisplayClear();
	drawbuttons();
	drawMenu();
	drawGamezone();
}
void KeyboardEventProcess( int key, int event ) 	   	//响应键盘 
{
	uiGetKeyboard( key, event) ;
	if( gamestart != -1 )
		display();
} 
void TimerEventProcess( int id )						//时间调控 
{
	if(id == 0 && gamestart == 1 && gamecontinue == 1 )	//字符下落		 
	{
		if( yz <= 3 )
		{											//判断字符下落具体位置 
			gamecontinue = 0;
			yz = winheight;
		}
		else if( yz > 3 )
			yz = yz - speed;	
	}
	if( id == 0 && gamestart == 1 )
		currenttime = currenttime + 50;
	if( id == 1 && gamestart == 1 )					//字母去边框换色 
	{
		change = 1;
		DefineColor( "randomcolor", RandomInteger( 1 ,255 ) * 1.0 / 255, RandomInteger( 1, 255 ) * 1.0 / 255, 
			RandomInteger( 1, 255 ) * 1.0 / 255 );	
	}
	if( gamestart != -1 )
		display();
} 
void CharEventProcess( char ch )				   	//字符响应 
{
	uiGetChar( ch );
	if( gamestart != -1 )
		display();
}
void MouseEventProcess( int x, int y, int button, int event )   //鼠标响应 
{
	uiGetMouse( x, y, button, event );
	display();
}
void Main()
{
	SetWindowTitle( "作业2" );
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
