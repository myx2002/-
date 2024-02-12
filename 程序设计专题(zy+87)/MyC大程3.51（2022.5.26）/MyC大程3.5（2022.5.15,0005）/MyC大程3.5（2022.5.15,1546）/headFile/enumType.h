//======================================================
//	enumType.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/5
//
//======================================================

#ifndef __ENUMTYPE_H__
#define __ENUMTYPE_H__

//枚举文本输出类型：普通、下划线、删除线、缩写、超链接、标题 
enum printType
{
	normalText, insText, delText, abbrText, hrefText, titleText
};

//枚举线条的类型：实线、虚线 
enum lineTypeEnum
{
	normalLine, dotLine
};

//枚举线条的位置：上、中、下 
enum lineLocationEnum
{
	down, middle, up
};

#endif 
