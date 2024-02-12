//======================================================
//	display.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================

#include"display.h"
#include"string.h"
#include"stdio.h"
#include"linkedlist.h"
#include"displaybox.h"
#include"analysis.h"
#include"widget.h"
#include"strlib.h"
#include"imgui.h"
#include"page.h"
#include "graphics.h"
#include "extgraph.h"
#include "icon.h"


extern double windowHeight;					//����߶� 
extern double windowWidth;					//������ 
char fileName[100] = "source/homepage.txt";	//�ļ��� 
extern displayBox textBox, outputBox;		//չʾ������ 
extern page PageList[20];					//��¼���10��ҳ�� 
extern int ithWebPage; 						//��ǰҳ�� 
extern int webPageNum;						//��ҳ��

//�������������ļ�����ͨ���������������� 
static void printNodeIntoTextBox(void *obj); 

/*
 *	������: display
 *
 *	���ܣ����������
 */
void display()
{
	DisplayClear();		//���

	drawTextBox();		//���Ƶ�ַ�� 
	drawInputBox(); 	//����� 
	drawOutputBox();	//����� 
	drawButton();		//���ư�ť
	drawPageBar();		//���Ʊ�ǩҳ�� 
	drawMenu();			//���Ʋ˵��� 
}

/*
 *	������: drawMenu
 *
 *	���ܣ����Ʋ˵�����ʵ�ֲ˵��Ϳ�ݼ����� 
 */
void drawMenu()
{
	double menuBarHeight = windowHeight / 20;
	drawMenuBar(0, windowHeight - menuBarHeight, windowWidth, menuBarHeight);	//���Ƴ��� 
	
	static char * menuListMenu[] = {"�˵�",
		"��ҳ | Ctrl-H",
		"ˢ�� | Ctrl-R",
		"���� | Ctrl-B",
		"ǰ�� | Ctrl-A",
		"�ر� | Ctrl-Q", 
		"�½� | Ctrl-T"};
	int selection;
	//���Ʋ˵� 
	double menuWidth = windowHeight / 15;
	double wlist = TextStringWidth(menuListMenu[1]) * 1.2;
	selection = menuList(GenUIID(0), menuWidth / 50 + windowHeight * 0.18, windowHeight - menuBarHeight, menuWidth,
						wlist, menuBarHeight, menuListMenu, sizeof(menuListMenu) / sizeof(menuListMenu[0]));
	//�˵���Ӧ
	switch (selection)
	{
		case 1:
			reactHome();				//��ҳ
			break;
		case 2:
			reactCircleReflesh();		//ˢ��
			break;
		case 3:
			reactLeftArrow();			//����
			break;
		case 4:
			reactRightArrow();			//ǰ��
			break;
		case 5:
			cancelPage(ithWebPage);		//�ر�
			break; 
		case 6:
			reactPlus();				//�½�
			break;
		default:
			break;
	}	//��Ϊ�˵���������ͱ�ǩҳ���������غϣ�������Ҫ�ڴ򿪲˵����ڼ��ñ�ǩ��ʧЧ��ͨ��������ı���unfoldMenu���� 
}

/*
 *	������: drawButton
 *
 *	���ܣ����ư�ť
 */
void drawButton()
{
	double buttonWidth = windowHeight / 10;		//��ť��� 
	double buttonHeight = windowHeight / 20;	//��ť�߶� 
	SetPointSize(20);
	//����"��ת"��ť���жϰ��� 
	if(button(GenUIID(0), windowWidth / 2 - buttonWidth,
			  windowHeight - buttonHeight, buttonWidth, buttonHeight, "��ת"))
	{
		if(fopen(fileName, "r")==NULL)				//����������ַ�����Դ� 
		{
			return;
		 } 
		loadingPage(&PageList[ithWebPage], fileName);	//�����ļ�����ҳ�� 
	}
	if(button(GenUIID(0), 0, 
			  0, 1.5, 0.6, "ǰ��"))					//��ǰ����ҳ�� -1 
	{
		if(PageList[ithWebPage].ithTextPage - 1>=0)
		PageList[ithWebPage].ithTextPage--;	
		display();
	}
	if(button(GenUIID(0), 1.5,
			  0, 1.5, 0.6, "��"))					//��ǰ���ҳ�� +1
	{	
		if(PageList[ithWebPage].ithTextPage + 1<=PageList[ithWebPage].textPageNum)
		PageList[ithWebPage].ithTextPage++;
		display();		 
	}
	if(button(GenUIID(0), 11,
			  0, 1.5, 0.6, "ǰ��"))					//��ǰ����ҳ�� -1 
	{
		if(PageList[ithWebPage].ithOutputPage - 1>=0)
		PageList[ithWebPage].ithOutputPage--;
		display();	
	}
	if(button(GenUIID(0), 12.5,
			  0, 1.5, 0.6, "��"))					//��ǰ���ҳ�� +1
	{
		if(PageList[ithWebPage].ithOutputPage + 1<=PageList[ithWebPage].outputPageNum)
		PageList[ithWebPage].ithOutputPage++;
		display();	
	}
	
	//������ƶ���ˢ�¡����ˡ�ǰ������ҳͼ��ʱ���Ƶ�Բ 
	drawRoundButton(windowWidth / 50, windowHeight - windowHeight / 40, 
			windowHeight * 0.05 * 0.4, drawCircleReflesh, reactCircleReflesh, NULL);
	drawRoundButton(windowWidth / 50 + windowHeight * 0.04, windowHeight - windowHeight / 40,
			windowHeight * 0.05 * 0.4, drawLeftArrow, reactLeftArrow, permitLeftArrow);
	drawRoundButton(windowWidth / 50 + windowHeight * 0.08, windowHeight - windowHeight / 40, 
			windowHeight * 0.05 * 0.4, drawRightArrow, reactRightArrow, permitRightArrow);
	drawRoundButton(windowWidth / 50 + windowHeight * 0.12, windowHeight - windowHeight / 40,
			windowHeight * 0.05 * 0.4, drawHome, reactHome, NULL);
}

/*
 *	������: drawTextBox
 *
 *	���ܣ����Ƶ�ַ�� 
 */
void drawTextBox()
{
	double menuBarHeight = windowHeight / 20;
	textbox(GenUIID(0), windowWidth / 2, windowHeight - menuBarHeight,
			windowWidth, menuBarHeight, fileName, 100); 
}

/*
 *	������: drawOutputBox
 *
 *	���ܣ���ӡ��������ݲ�����Ҫ��ҳʱ���ô��ڵ���Ϊ��һҳ�ı�ͷ 
 */
void drawOutputBox()
{
	drawDisplayBox(&outputBox);
	reset(&outputBox);
	//��������ı���ӡ������¶�λ����ͷ 
	if(PageList[ithWebPage].text!=NULL)													
	{
		linkedlistADT nodeptr=NULL;
		if(!PageList[ithWebPage].ithOutputPage)															//�������Ϊ��0ҳʱ��ʵ����Ϊ��һҳ�� 
		{
			nodeptr = PageList[ithWebPage].outputPageContent[PageList[ithWebPage].ithOutputPage]->next;	//��ͷ��������ݣ����Ե���ҳ������ҳ����	
		}
		else
		{
			nodeptr = PageList[ithWebPage].outputPageContent[PageList[ithWebPage].ithOutputPage];		//��ö�Ӧҳ��������׽ڵ�
		}
		//��ʼ������� 
		while (nodeptr!=NULL && nodeptr!=PageList[ithWebPage].outputPageContent[PageList[ithWebPage].ithOutputPage + 1]) 
		{																								//�ڶ���������Ϊ�˿��Ʋ�������ӡ��һҳ������ 
			if(dealInput(nodeptr->dataptr)==1)															//�����������ʱ 
			{
				return;
			}
			if(outputBox.nowPrintY<outputBox.positionY)													//����ӡ��չʾ��ײ�ʱ�������µ�����ҳ
			{
				PageList[ithWebPage].outputPageNum++;											 		//������ҳ����1
				PageList[ithWebPage].outputPageContent[PageList[ithWebPage].outputPageNum] = nodeptr;	//�˴��ڵ㸳����һҳ������ͷ 
				display();
				break; 
			}
			nodeptr = nodeptr->next;																	//������һ���ڵ����
		}
	}
	//����ҳ�� 
	SetPenColor("blue");
	MovePen(outputBox.positionX + outputBox.boxWdith - TextStringWidth("page:") -
			TextStringWidth(IntegerToString(PageList[ithWebPage].ithOutputPage + 1)), outputBox.positionY-GetFontHeight());
	DrawTextString("page:");
	DrawTextString(IntegerToString(PageList[ithWebPage].ithOutputPage + 1));
}

/*
 *	������: drawInputBox
 *
 *	���ܣ���ӡ��������ݲ�����Ҫ��ҳʱ���ô��ڵ���Ϊ��һҳ�ı�ͷ  
 */
void drawInputBox()
{
	reset(&textBox);
	drawDisplayBox(&textBox);
	if(PageList[ithWebPage].text!=NULL)
	{
		linkedlistADT nodeptr;
		//���ݰ�ť"ǰ�� ""�� "���ı䵱ǰҳ�루ithTextPage) 
		if(!PageList[ithWebPage].ithTextPage)														//��ǰҳΪ����ҳʱ��ʵ����Ϊ��һҳ�� 
		{
			nodeptr = PageList[ithWebPage].textPageContent[PageList[ithWebPage].ithTextPage]->next;	//��ͷ��������ݣ����Ե���ҳ������ҳ����		
		}
		else
		{
			nodeptr = PageList[ithWebPage].textPageContent[PageList[ithWebPage].ithTextPage];		//��ö�Ӧҳ��������׽ڵ�	
		}
		
		while (nodeptr != NULL&&nodeptr!=PageList[ithWebPage].textPageContent[PageList[ithWebPage].ithTextPage+1]) 
																									//�ڶ���������Ϊ�˿��Ʋ�������ӡ��һҳ������ 
		{
			printNodeIntoTextBox(nodeptr->dataptr);													//��ӡ��ÿ���ڵ�洢������ 
			if(textBox.nowPrintY<textBox.positionY)													//����ӡ��չʾ��ײ�ʱ�������µ�����ҳ 
			{
				PageList[ithWebPage].textPageNum++;													//������ҳ����1 
				PageList[ithWebPage].textPageContent[PageList[ithWebPage].textPageNum] = nodeptr;	//�˴��ڵ㸳����һҳ������ͷ 
				display();								
				break; 
			}
			nodeptr = nodeptr->next;																//������һ���ڵ���� 
		}	
	}
	else
	{
		writeInBox("�Ҳ������ļ�", &textBox);
	}
	//����ҳ�� 
	MovePen(textBox.positionX + textBox.boxWdith - TextStringWidth("page:") -
			TextStringWidth(IntegerToString(PageList[ithWebPage].ithTextPage + 1)),textBox.positionY - GetFontHeight());
	DrawTextString("page:");
	DrawTextString(IntegerToString(PageList[ithWebPage].ithTextPage + 1));	
}

/*
 *	������: printNodeIntoTextBox
 *
 *	���ܣ����ļ�����������������
 *
 *	�����βΣ� ָ��������ݵ�ָ�� obj  ����ֵ���� 
 */
void printNodeIntoTextBox(void *obj)
{
	writeInBox(obj, &textBox);
	shift(&textBox);
}

/*
 *	������: drawPageBar
 *
 *	���ܣ�������ҳ��ǩ��
 *		  �������ĳ��ǩ��ʱ����ַ����"��ǰ 
 */
void drawPageBar()
{
 	double menuBarHeight = windowHeight / 20;
 	double pageTagLength = webPageNum>=4?windowWidth/(webPageNum + 2):windowWidth / 5;		//����ǩҳ����ʱ��ǩҳ������ 
 	SetFont("����"); 
 	SetPointSize(17);
		
	 //���Ʊ�ǩҳ����
	 int i; 
 	for(i = 0; i<=webPageNum; i++)
 	{
 		int tmp = drawPage(pageTagLength * i, windowHeight - 2 * menuBarHeight,
		 pageTagLength, menuBarHeight, PageList[i].webPageName, i);
 		//����ֵ=1˵��������˸ñ�ǩ������2˵���ر���ҳ 
  		if(tmp==1)
  		{
   			ithWebPage = i;
   			strcpy(fileName, PageList[ithWebPage].location);								//���������ҳ��ַ����fileName 
  		}
  		else if(tmp==2)
  		{
  			cancelPage(i);
		}
 	}
 	drawRoundButton(pageTagLength * (webPageNum + 1) + 0.5, windowHeight - 1.5 * menuBarHeight,	//�����½���ťͼ�� 
	 							menuBarHeight * 0.35, drawPlus, reactPlus, permitPlus);
 	drawMenuBar(0, windowHeight - 2 * menuBarHeight, windowWidth, menuBarHeight); 					//���Ʊ�ǩҳ����
	//ͨ����ɫ�����γɱ�ǩҳ������ҳ������ͨ��Ч�� 
	if(ithWebPage > webPageNum)
	{
		ithWebPage--;
	}
	SetPenColor("white");
	MovePen(pageTagLength * ithWebPage, windowHeight - 2 * menuBarHeight);
	DrawLine(pageTagLength, 0);
	SetPenColor("blue"); 
}
