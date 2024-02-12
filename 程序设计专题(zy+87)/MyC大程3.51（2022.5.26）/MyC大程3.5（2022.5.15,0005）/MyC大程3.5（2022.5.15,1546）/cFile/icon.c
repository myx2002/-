//======================================================
//	icon.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================

#include "icon.h"
#include "graphics.h"
#include "extgraph.h"
#include "page.h"

extern page PageList[20];					//��ҳ�ṹ���� 
extern int ithWebPage; 						//��ǰ��ҳ 
extern int webPageNum;						//����ҳ�� 
extern int unfoldMenu;						//�˵��Ƿ�� 

/*
 *	������: drawCircleReflesh 
 *
 *	���ܣ���ˢ�°�ťͼ��
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawCircleReflesh(double x, double y, double r)
{
	SetPenColor("black");
	y-=0.04;
	SetPenSize(2);
	r*=0.6;
	double height=r * 2.4;		//��ͷ���Ʋ���  
	MovePen(x + r,y);
	DrawArc(r, 0, -270);
	SetPenSize(1);
	StartFilledRegion(1);   
	MovePen(x + 0.6 * r,y + r); //����ˢ�°�ť������ 
	DrawLine(-0.5*r,0.2*height);
	DrawLine(0, -0.4 * height);
	DrawLine(x + 0.6 * r - GetCurrentX(), y + r - GetCurrentY());
	EndFilledRegion();
}

/*
 *	������: drawLeftArrow
 *
 *	���ܣ���"����"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawLeftArrow(double x, double y, double r)
{
	double width,height;
	SetPenColor("black");
	if(!permitLeftArrow())
	{
		SetPenColor("light gray");	//��ɫ������ʹ�� 
	}
	SetPenSize(2);
	width = r * 0.47;
	height = r * 0.46;
	MovePen(x - width, y);			//���Ƽ�ͷ��� 
	DrawLine(width * 2,0);
	MovePen(x - width, y);
	DrawLine(width, height);		//���Ƽ�ͷ˫�� 
	MovePen(x - width, y);
	DrawLine(width, -height);
	SetPenSize(1);
}

/*
 *	������: drawRightArrow
 *
 *	���ܣ���"ǰ��"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawRightArrow(double x,double y,double r)
{
	double width,height;
	SetPenColor("black");
	if(!permitRightArrow())
	{
		SetPenColor("light gray");	//��ɫ������ʹ�� 
	}
	SetPenSize(2);
	width = r * 0.47;
	height = r * 0.46;
	MovePen(x + width, y);			//���Ƽ�ͷ��� 
	DrawLine(-2 * width, 0);
	MovePen(x + width, y);
	DrawLine(-width, height);		//���Ƽ�ͷ˫�� 
	MovePen(x + width, y);
	DrawLine(-width, -height);
	SetPenSize(1);
}

/*
 *	������: drawHome
 *
 *	���ܣ���"��ҳ"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawHome(double x,double y,double r)
{
	/*MovePen(x+r,y);
	DrawArc(r,0,359);*/					//�����Ƿ��ڰ�ťԲȦ�� 
	SetPenColor("black");
	SetPenSize(2);
	MovePen(x + 0.5 * r, y + 0.15 * r);	//���Ʒ��ݵײ�		
	DrawLine(0, -0.75 * r);
	DrawLine(-1 * r, 0);
	DrawLine(0, 0.75 * r);
	
	MovePen(x - 0.75 * r, y + 0.15 * r);//���Ʒ��ݶ���
	DrawLine(1.5 * r, 0);
	DrawLine(-0.75 * r, 0.5 * r);
	DrawLine(-0.75 * r, -0.5 * r);
	
	MovePen(x, y - 0.6 * r);			//���Ʒ����ڲ���Ϣ 
	DrawLine(0, 0.5 * r);
	DrawLine(0.25 * r, 0);
	DrawLine(0, -0.5 * r);
	SetPenSize(1);
}

/*
 *	������: drawPlus
 *
 *	���ܣ���"�½�"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ���� 
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
 *	������: drawMultiplication 
 *
 *	���ܣ���"�ر�"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ����Ծ� r 	����ֵ����  
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
 *	������: reactCircleReflesh 
 *
 *	���ܣ�ˢ�� 
 */
void reactCircleReflesh()
{
	loadingPage(&PageList[ithWebPage], PageList[ithWebPage].location);
}

/*
 *	������: reactLeftArrow 
 *
 *	���ܣ����ˣ���һҳ��
 */
void reactLeftArrow()
{
	strcpy(PageList[ithWebPage].nextPage, PageList[ithWebPage].location);		//�ѵ�ǰҳ��ַ���� "��һҳ" 
	//��һ������Ҫ��ͨ������'\0'��ʹ��loadingPage�����formalPage��ֵ��
	//������formalPage��Ϊ��ǰҳ������"����"�Ľ����Ϊ��ǰҳ 
	PageList[ithWebPage].location[0] = 0;																 
	loadingPage(&PageList[ithWebPage], PageList[ithWebPage].formalPage);		//�� "��һҳ" 
	PageList[ithWebPage].formalPage[0] = 0; 
}

/*
 *	������: reactRightArrow 
 *
 *	���ܣ�ǰ������һҳ ��
 */
void reactRightArrow()
{
	strcpy(PageList[ithWebPage].formalPage, PageList[ithWebPage].location);	//�ѵ�ǰҳ��ַ���� "��һҳ" 
	loadingPage(&PageList[ithWebPage], PageList[ithWebPage].nextPage);			//�� "��һҳ" 
	PageList[ithWebPage].nextPage[0] = 0; 
} 

/*
 *	������: reactHome 
 *
 *	���ܣ�������ҳ 
 */
void reactHome()
{
	loadingPage(&PageList[ithWebPage], "source/homepage.html");			//������ҳ�ļ� 
}

/*
 *	������: reactPlus
 *
 *	���ܣ��½���ҳ 
 */
void reactPlus()
{
	if(!unfoldMenu)	//����˵�δ�� 
	{
		webPageNum++;													//��ҳ����+1 
		loadingPage(&PageList[webPageNum], "source/homepage.html");		//������ҳ�ļ� 
		ithWebPage = webPageNum;											//��ǰ��ҳ��ת���½���ҳ 
	}
}

/*
 *	������: permitRightArrow
 *
 *	���ܣ��ж��Ƿ���Ե��ǰ����ť
 *
 *	����ֵ����-1 ��-0 
 */
int permitRightArrow()
{
	return PageList[ithWebPage].nextPage[0]!=0;			//��"��һҳ"����ʱ 
}

/*
 *	������: permitLeftArrow
 *
 *	���ܣ��ж��Ƿ���Ե�����˰�ť 
 *
 *	����ֵ����-1 ��-0 
 */
int permitLeftArrow()
{
	return PageList[ithWebPage].formalPage[0]!=0;			//��"��һҳ"����ʱ 
}

/*
 *	������: permitPlus 
 *
 *	���ܣ��ж��Ƿ�����½���ҳ 
 *
 *	����ֵ����-1 ��-0 
 */
int permitPlus()
{
	return webPageNum<=8;		//��0-8һ�����Ե���½�9�Σ�����������ҳ�������Դ�10����ҳ 
}
