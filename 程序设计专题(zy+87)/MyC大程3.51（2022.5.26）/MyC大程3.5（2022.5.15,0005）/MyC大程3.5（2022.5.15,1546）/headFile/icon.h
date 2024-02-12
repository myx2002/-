//======================================================
//	icon.h
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================
#ifndef __ICON_H__
#define __ICON_H__

/*
 *	������: drawCircleReflesh 
 *
 *	���ܣ���ˢ�°�ťͼ��
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawCircleReflesh(double x, double y, double r);

/*
 *	������: drawLeftArrow
 *
 *	���ܣ���"����"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawLeftArrow(double x, double y, double r);

/*
 *	������: drawRightArrow
 *
 *	���ܣ���"ǰ��"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawRightArrow(double x, double y, double r);

/*
 *	������: drawHome
 *
 *	���ܣ���"��ҳ"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ����  
 */
void drawHome(double x, double y, double r);

/*
 *	������: drawMultiplication 
 *
 *	���ܣ���"�ر�"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ����Ծ� r 	����ֵ����  
 */
void drawMultiplication(double x, double y, double r);

/*
 *	������: drawPlus
 *
 *	���ܣ���"�½�"��ťͼ�� 
 *
 *	�����βΣ�Բ������ x,y  ��԰뾶 r 	����ֵ���� 
 */
void drawPlus(double x, double y, double r);

/*
 *	������: reactCircleReflesh 
 *
 *	���ܣ�ˢ�� 
 */
void reactCircleReflesh(); 

/*
 *	������: reactLeftArrow 
 *
 *	���ܣ����ˣ���һҳ�� 
 */
void reactLeftArrow();

/*
 *	������: reactRightArrow 
 *
 *	���ܣ�ǰ������һҳ �� 
 */
void reactRightArrow();

/*
 *	������: reactHome 
 *
 *	���ܣ�������ҳ 
 */
void reactHome();

/*
 *	������: reactPlus
 *
 *	���ܣ��½���ҳ 
 */
void reactPlus();

/*
 *	������: permitRightArrow
 *
 *	���ܣ��ж��Ƿ���Ե��ǰ����ť
 *
 *	����ֵ����-1 ��-0 
 */
int permitRightArrow();

/*
 *	������: permitLeftArrow
 *
 *	���ܣ��ж��Ƿ���Ե�����˰�ť 
 *
 *	����ֵ����-1 ��-0 
 */
int permitLeftArrow();

/*
 *	������: permitPlus 
 *
 *	���ܣ��ж��Ƿ�����½���ҳ 
 *
 *	����ֵ����-1 ��-0 
 */
int permitPlus();

#endif
