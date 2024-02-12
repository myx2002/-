//======================================================
//	displaybox.h
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//  Type:ddisplaybox
//	��ͷ�ļ�������һ�������ͣ�չʾ�򣬹���ʱֻҪ�ṩ�����½�����
//  ���ȣ��߶ȣ���������Ĭ�����ã�����Ҫ����Ϊ������һ���ַ�����
//  ����Ļ����ʾ����ʾʱ����ݿ�߽��Զ����С� 
//	
//	���һ���޸ģ�2022/5/15
//======================================================

#ifndef __DISPLAYBOX_H__
#define __DISPLAYBOX_H__
#include "strlib.h"
#include "enumType.h"

//����displayBox�Ľṹ 
typedef struct displayBox{
	double positionX, positionY;		//����չʾ�����½������� 
	double boxWdith, boxHeight; 		//����չʾ������߶�
	double boxSideGap;				//���崰����ʾ���־�߿�ľ���
	double nowPrintX, nowPrintY; 	//��ǰ�ַ���ӡλ��
	int fontSize;					//��ǰ�����С 
	int globalFontSize;				//ȫ�������С 
	char fontType[10];				//��¼���� 
} displayBox;

/*
 *	������: registerDefaultDisplayBox
 *
 *	���ܣ�ע��һ��Ĭ�����͵�չʾ�� 
 *
 *	�����βΣ�λ�� x,y ��͸� boxWidth,boxHeight ָ��չʾ�� nowBox 
 */
void registerDefaultDisplayBox(double positionX, double positionY,
						double boxWidth, double boxHeight, displayBox*); 

/*
 *	������: drawDisplayBox 
 *
 *	���ܣ�����չʾ�� 
 *
 *	�����βΣ�ָ��չʾ�� box 
 */
void drawDisplaybox(displayBox* a); 

/*
 *	������: writeInBox
 *
 *	���ܣ���ָ��չʾ���ӡ�ַ��� a  
 *
 *	�����β�: �ַ��� a , չʾ�� nowBox  ����ֵ: �� 
 */
void writeInBox(string, displayBox*);  

/*
 *	������: shift
 *
 *	���ܣ�����
 *
 *	�����β�: ָ��չʾ�� nowBox	 ����ֵ: �� 
 */
void shift(displayBox*); 

/*
 *	������: reset
 *
 *	���ܣ�����ҳʱ���������λ����չʾ����ʼ 
 *
 *	�����β�: ָ��չʾ�� nowBox	 ����ֵ: �� 
 */
void reset(displayBox*); 

/*
 *	������: drawHorizentalLine
 *
 *	���ܣ����ָ��� 
 *
 *	�����β�: ָ��չʾ�� nowBox	 ����ֵ: �� 
 */
void drawHorizentalLine(displayBox*);

/*
 *	������: drawLineWithText
 *
 *	���ܣ���������ߵ��ı����»��ߡ�ɾ���ߣ� 
 *
 *	�����β�: ָ��չʾ�� nowBox	, �ַ��� a, �ߵ�λ�� lineLocation �ߵ�����style
 *	����ֵ: �� 
 */
void drawLineWithText(displayBox*,string, enum lineLocationEnum, enum lineTypeEnum, string);

/*
 *	������: drawCircle
 *
 *	���ܣ���ԲȦ�������б� 
 *
 *	�����β�: ָ��չʾ�� a ԲȦ���� type ����ֵ: �� 
 */
void drawCircle(displayBox* a, int type);

/*
 *	������: resetBoxSideGap 
 *
 *	���ܣ����ݴ���������ṹ�������� 
 *
 *	�����β�: ָ������ length ����ֵ: ��  
 */
void resetBoxSideGap(displayBox* now);
#endif

