//======================================================
//	analysis.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================

#ifndef __ANALYSES_H__
#define __ANALYSES_H__

#include "strlib.h"

/*
 *	函数名: dealInput
 *
 *	功能：处理传入的原文本字符串
 *
 *	接受形参: 字符串 s	返回值:	点击超链接时返回 1 其余 0  
 */
int dealInput(char *buffer);

/*
 *	函数名: apeendString
 *
 *	功能：读入字符加到字符串 
 *
 *	接受形参: 字符串 string  读入字符char	返回值: 无 
 */
void appendString(char *string, char i);

#endif
