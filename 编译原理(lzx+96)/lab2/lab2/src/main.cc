#include<cstdio>
#include"tree.hh"
#include"symbol_test.hh"

extern int yyparse();
extern FILE* yyin;
node* root;

int main(int argc, char *argv[])
{
    //判断参数个数
    if(argc < 2)
    {
        printf("Usage: %s filename\n",argv[0]);
        return 1;
    }

    //yyin为lex的默认输入流
    yyin = fopen(argv[1],"r");
    if(yyin == NULL)
    {
        printf("Can't open file %s\n",argv[1]);
        return 1;
    }
    int re = yyparse();

    fclose(yyin);

    tree_print(root, 0);
    if(re == 1)
        return 1;
    else
        re = symbol_table_test(root);

    return re;
}