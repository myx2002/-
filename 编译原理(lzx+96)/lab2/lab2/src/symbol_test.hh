#ifndef _SYMBOL_H_

#define _SYMBOL_H_

#include<iostream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<vector>
#include"tree.hh"
using namespace std;


struct symbol_leaf
{
    enum symbol_Kind {INT, ARRAY, VOID} kind;
    vector<int> array_size;
    //暂且用来记录func的参数
    vector<string> param;
};

struct symbol_table
{
    map<string, symbol_leaf*> var_def;
    symbol_table* next;
};

//先记录write和read函数
void build_base_func();

int symbol_table_test(node* root);
int test_func_def(node* temp);
int get_func_para(node* temp);
int test_block(node* temp);
int test_decl_def(node* temp);
int test_stmt_def(node* temp);
int test_decl_def_func(node* temp);
int test_exp(node* temp,int return_index = 1);
int test_cond(node* temp);
int test_array(node* temp, int level);
int find_layer(node* temp);

#endif