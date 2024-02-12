//======================================================
//	icon.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================

#include "icon.h"
#include "graphics.h"
#include "extgraph.h"
#include "page.h"

extern page PageList[20];					//网页结构数组 
extern int ithWebPage; 						//当前网页 
extern int webPageNum;						//总网页数 
extern int unfoldMenu;						//菜单是否打开 

/*
 *	函数名: drawCircleReflesh 
 *
 *	功能：画刷新按钮图标
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawCircleReflesh(double x, double y, double r)
{
	SetPenColor("black");
	y-=0.04;
	SetPenSize(2);
	r*=0.6;
	double height=r * 2.4;		//箭头绘制参数  
	MovePen(x + r,y);
	DrawArc(r, 0, -270);
	SetPenSize(1);
	StartFilledRegion(1);   
	MovePen(x + 0.6 * r,y + r); //绘制刷新按钮三角形 
	DrawLine(-0.5*r,0.2*height);
	DrawLine(0, -0.4 * height);
	DrawLine(x + 0.6 * r - GetCurrentX(), y + r - GetCurrentY());
	EndFilledRegion();
}

/*
 *	函数名: drawLeftArrow
 *
 *	功能：画"后退"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawLeftArrow(double x, double y, double r)
{
	double width,height;
	SetPenColor("black");
	if(!permitLeftArrow())
	{
		SetPenColor("light gray");	//灰色代表不可使用 
	}
	SetPenSize(2);
	width = r * 0.47;
	height = r * 0.46;
	MovePen(x - width, y);			//绘制箭头横杠 
	DrawLine(width * 2,0);
	MovePen(x - width, y);
	DrawLine(width, height);		//绘制箭头双翼 
	MovePen(x - width, y);
	DrawLine(width, -height);
	SetPenSize(1);
}

/*
 *	函数名: drawRightArrow
 *
 *	功能：画"前进"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawRightArrow(double x,double y,double r)
{
	double width,height;
	SetPenColor("black");
	if(!permitRightArrow())
	{
		SetPenColor("light gray");	//灰色代表不可使用 
	}
	SetPenSize(2);
	width = r * 0.47;
	height = r * 0.46;
	MovePen(x + width, y);			//绘制箭头横杠 
	DrawLine(-2 * width, 0);
	MovePen(x + width, y);
	DrawLine(-width, height);		//绘制箭头双翼 
	MovePen(x + width, y);
	DrawLine(-width, -height);
	SetPenSize(1);
}

/*
 *	函数名: drawHome
 *
 *	功能：画"主页"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawHome(double x,double y,double r)
{
	/*MovePen(x+r,y);
	DrawArc(r,0,359);*/					//调试是否在按钮圆圈内 
	SetPenColor("black");
	SetPenSize(2);
	MovePen(x + 0.5 * r, y + 0.15 * r);	//绘制房屋底部		
	DrawLine(0, -0.75 * r);
	DrawLine(-1 * r, 0);
	DrawLine(0, 0.75 * r);
	
	MovePen(x - 0.75 * r, y + 0.15 * r);//绘制房屋顶部
	DrawLine(1.5 * r, 0);
	DrawLine(-0.75 * r, 0.5 * r);
	DrawLine(-0.75 * r, -0.5 * r);
	
	MovePen(x, y - 0.6 * r);			//绘制房屋内部信息 
	DrawLine(0, 0.5 * r);
	DrawLine(0.25 * r, 0);
	DrawLine(0, -0.5 * r);
	SetPenSize(1);
}

/*
 *	函数名: drawPlus
 *
 *	功能：画"新建"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无 
 */
void drawPlus(double x, double y, double r)
{
	SetPenColor("blue");
	SetPenSize(2);
	MovePen(x + r, y);
	DrawArc(r, 0, 359);			
	MovePen(x - 0.6 * r, y);
	DrawLine(1.2 * r, 0);
	MovePen(x, y - 0.6 * r);
	DrawLine(0, 1.2 * r);
}

/*
 *	函数名: drawMultiplication 
 *
 *	功能：画"关闭"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  半相对径 r 	返回值：无  
 */
void drawMultiplication(double x, double y, double r)
{
	SetPenSize(2);
	MovePen(x + r, y);
	DrawArc(r, 0, 359);
	MovePen(x - 0.5 * r, y + 0.5 * r);
	DrawLine(r, -r);
	MovePen(x + 0.5 * r, y + 0.5 * r);
	DrawLine(-r, -r);
	SetPenSize(1);
}

/*
 *	函数名: reactCircleReflesh 
 *
 *	功能：刷新 
 */
void reactCircleReflesh()
{
	loadingPage(&PageList[ithWebPage], PageList[ithWebPage].location);
}

/*
 *	函数名: reactLeftArrow 
 *
 *	功能：后退（上一页）
 */
void reactLeftArrow()
{
	strcpy(PageList[ithWebPage].nextPage, PageList[ithWebPage].location);		//把当前页网址赋给 "下一页" 
	//下一步很重要！通过设置'\0'来使得loadingPage不会对formalPage赋值；
	//否则会把formalPage赋为当前页而导致"后退"的结果仍为当前页 
	PageList[ithWebPage].location[0] = 0;																 
	loadingPage(&PageList[ithWebPage], PageList[ithWebPage].formalPage);		//打开 "上一页" 
	PageList[ithWebPage].formalPage[0] = 0; 
}

/*
 *	函数名: reactRightArrow 
 *
 *	功能：前进（下一页 ）
 */
void reactRightArrow()
{
	strcpy(PageList[ithWebPage].formalPage, PageList[ithWebPage].location);	//把当前页网址赋给 "上一页" 
	loadingPage(&PageList[ithWebPage], PageList[ithWebPage].nextPage);			//打开 "下一页" 
	PageList[ithWebPage].nextPage[0] = 0; 
} 

/*
 *	函数名: reactHome 
 *
 *	功能：返回主页 
 */
void reactHome()
{
	loadingPage(&PageList[ithWebPage], "source/homepage.html");			//加载主页文件 
}

/*
 *	函数名: reactPlus
 *
 *	功能：新建网页 
 */
void reactPlus()
{
	if(!unfoldMenu)	//如果菜单未打开 
	{
		webPageNum++;													//网页总数+1 
		loadingPage(&PageList[webPageNum], "source/homepage.html");		//加载主页文件 
		ithWebPage = webPageNum;											//当前网页跳转至新建网页 
	}
}

/*
 *	函数名: permitRightArrow
 *
 *	功能：判断是否可以点击前进按钮
 *
 *	返回值：是-1 否-0 
 */
int permitRightArrow()
{
	return PageList[ithWebPage].nextPage[0]!=0;			//当"下一页"存在时 
}

/*
 *	函数名: permitLeftArrow
 *
 *	功能：判断是否可以点击后退按钮 
 *
 *	返回值：是-1 否-0 
 */
int permitLeftArrow()
{
	return PageList[ithWebPage].formalPage[0]!=0;			//当"下一页"存在时 
}

/*
 *	函数名: permitPlus 
 *
 *	功能：判断是否可以新建网页 
 *
 *	返回值：是-1 否-0 
 */
int permitPlus()
{
	return webPageNum<=8;		//从0-8一共可以点击新建9次，算上已有主页，最多可以打开10个网页 
}
