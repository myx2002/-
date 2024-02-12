#include "menu.h"
int xianshi()
{
	//输出菜单，并获取选择的功能
	int sel;
	printf("\n           图书管理系统              \n");
	printf("━━━━━━━━━━━━━━━━━━ \n");
	printf("          1-图书信息录入\n");
	printf("          2-图书信息浏览\n");
	printf("          3-图书信息查询\n");
	printf("          4-图书信息修改\n");
	printf("          5-图书信息删除\n");
	printf("          6-图书信息保存\n");
	printf("          7-增加新管理员\n");
	printf("          8-删除管理员\n");
	printf("          9-管理员信息浏览\n");
	printf("          10-退出\n");
	printf("━━━━━━━━━━━━━━━━━━ \n");
	printf("请选择：");
	fflush(stdin);
	scanf("%d", &sel);
	return sel;
}

