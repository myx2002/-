//======================================================
//	display.h
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/1 
//
//======================================================

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/*
 *	������: display
 *
 *	���ܣ����������
 */
void display();

/*
 *	������: drawButton
 *
 *	���ܣ����Ʋ˵��� 
 */
void drawMenu(); 

/*
 *	������: drawButton
 *
 *	���ܣ����ư�ť
 */
void drawButton();

/*
 *	������: drawTextBox
 *
 *	���ܣ����Ƶ�ַ�� 
 */
void drawTextBox(); 

/*
 *	������: drawOutputBox
 *
 *	���ܣ���ӡ��������ݲ�����Ҫ��ҳʱ���ô��ڵ���Ϊ��һҳ�ı�ͷ 
 */
void drawOutputBox();

/*
 *	������: drawInputBox
 *
 *	���ܣ���ӡ��������ݲ�����Ҫ��ҳʱ���ô��ڵ���Ϊ��һҳ�ı�ͷ  
 */
void drawInputBox();

/*
 *	������: printNodeIntoTextBox
 *
 *	���ܣ����ļ�����������������
 *
 *	�����βΣ� ָ��������ݵ�ָ�� obj  ����ֵ���� 
 */
void drawPageBar();

#endif
