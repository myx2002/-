//======================================================
//	analyses.h
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================

#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "strlib.h"

/*
 *	������: createTextWidget
 *
 *	���ܣ������ı��ؼ������򴴽�һ���µ� TextWidget�����뵽����
 *		  ������Ƶ��ı��ؼ���������ʱ����ʾ��Ӧ���� 
 *
 *	�����β�: �ı����� a, �ؼ����� type, ��Ӧ���� buffer	����ֵ: �� 
 */
int createTextWidgt(string a, enum printType textType, string buffer);

/*
 *	������: drawRoundButton
 *
 *	���ܣ�����draw����ָ�����ˢ�¡����ˡ�ǰ������ҳ���رջ��½���ťͼ��
 *		  ������ƶ�����ť��ʱ����ť�ײ�����ֻ�ɫԲ��
 *		  ���������ťʱ������Ӧ��react����ָ��ִ����Ӧ����
 *
 *	�����β�: ͼ��λ�� x,y ͼ���С r ��ͼ����ָ�� draw ��Ӧ����ָ��react ����ֵ: �� 
 */
void drawRoundButton(double x, double y, double r, void(*draw)(double, double, double), void (*react)(), int (*permit)());

/*
 *	������: drawPage 
 *
 *	���ܣ�д��i����ҳ�ĵ���ҳ����ʵ�����ǵ�i+1����ҳ,��Ϊi��0��ʼ��
 *		  ���ƹرհ�ťͼ�� 
 *		  �Թرհ�ť����Ӧ�������á������ 
 *		  �����ǰ��ҳ��ǩ��ʱ����1 
 *
 *	�����β�: ��ǩ������ x,y,w,h ��ҳ�� name canclePage����ָ�� react  ��ǰ��ҳi 
 *	����ֵ����� 1 δ��� 0 
 */
int drawPage(double x, double y, double w, double h, string name, int i);

#endif 
