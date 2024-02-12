//======================================================
//	analysisTag.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/1 
//
//======================================================

#ifndef __ANALYSISTAG_H__
#define __ANALYSIStAG_H__

#include "strlib.h"

/*
 *	函数名: dealTag
 *
 *	功能：进一步读取标签，并执行相应操作 
 *
 *	接受形参: 完整标签 fulltag	返回值: 无 
 */
void dealTag(string);

/*
 *	函数名: openTextInputMode
 *
 *	功能：判断是否是库中含有的标签以决定是否读取后面的文本 
 *
 *	接受形参: 读取的标签 nowTag	 返回值: 是-1 否-0 
 */
int OpenTextInputMode(string);

#endif
