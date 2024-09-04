#include<cstdio>
#include<fstream>
#include "tree.hh"
#include "symbol_test.hh"
#include "riscv_trans.hh"

extern int yyparse();
extern FILE* yyin;
node* root;

int main(int argc, char *argv[])
{
    //判断参数个数
    if(argc < 3)
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

    // tree_print(root, 0);
    if(re == 1)
        return 1;
    else
        re = symbol_table_test(root);

    //还需要支持一个输出，用于输出中间代码

    string IR_str = "";
    if(re == 1)
        return 1;
    else
        IR_str = translate_ir(root);
    
    // cout << IR_str << endl;

    std::ofstream outputFile("output.txt");
        outputFile << IR_str << endl;
    outputFile.close();

    string riscv_str = ir_to_riscv(IR_str);

    std::ofstream outputFile_riscv(argv[2]);
        outputFile_riscv << riscv_str << endl;
    outputFile_riscv.close();

    return re;
}