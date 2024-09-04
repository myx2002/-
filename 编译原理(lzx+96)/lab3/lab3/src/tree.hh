#ifndef _TREE_H_

#define _TREE_H_

// #include<cstdio>
#include<iostream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

extern int yylineno;
enum Kind {NUM_k, STR_k, NOE_k};
struct node
{
    string type;  //token类型
    //enum结构便捷搜索
    Kind kind;
    int value;    //数值
    string name;  //变量名
    int lineno;   //行号
    struct node* left;  //左右结点
    struct node* right;
};

extern struct node* root; //全局根结点

node* create_node(string type="", Kind kind = NOE_k, int value = 0, string name = "", node* left = NULL, node* right = NULL);
void tree_print(node* t_root, int level);
void def_right(node* origin, node* value);

//八进制数转十进制数
int base_to_dec(int oct, int base);

#endif