#ifndef _RISCV_TRANS_H_

#define _RISCV_TRANS_H_

#include "tree.hh"
#include "symbol_test.hh"
#include <sstream>
#include <cctype>

string ir_to_riscv(string ir_result);
void get_temp_number(vector<std::string> ir_vector, int index);
string translate_riscv(string ir_input);
int get_position(string return_temp);
string delete_symbol(string input_str, string delete_ch);




#endif