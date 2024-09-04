#include "riscv_trans.hh"

//用于记录需要使用的临时变量个数
//默认大1
int temp_number_riscv = 0;
int is_main = 0;
//用于记录当前函数中临时变量与内存对映位置的映射信息
//int负数表示数组变量（暂时不需要）
map<string, int> number_temp_map;
//数组定义语句传递
string array_info_riscv;
//记录参数个数
int arg_number = 0;
//记录获得的参数个数
int get_number = 0;
//提前获取参数个数
int pre_get_number = 0;
//记录sp的偏移量
int sp_offset = 0;

string ir_to_riscv(string ir_result)
{
    // cout << ir_result << endl;
    string riscv_result;
    riscv_result += ".data\n";
    //将连续的字符串按照换行符分割
    istringstream string_list(ir_result);
    vector<std::string> ir_vector;
    string line;
    // 逐行读取，并存储到vector中
    while (std::getline(string_list, line)) 
        ir_vector.push_back(line);
    int func_flag = 0;
    for (int i = 0; i < (int)ir_vector.size(); i++) 
    {
        if(ir_vector[i].compare(0, 8, "FUNCTION") == 0)
        {
            if(func_flag == 0)
            {
                //直接跳转至main函数
                riscv_result += ".text\n";
                riscv_result += print_tab(4) + "j main\n";
                func_flag = 1;
            }
            pre_get_number = 0;
            //一个为s0寄存器，一个为ra寄存器
            temp_number_riscv = 2;
            number_temp_map.clear();
            //main函数需要区别对待
            if(ir_vector[i].substr(9) == "main:")
                is_main = 1;
            else
                is_main = 0;
            get_temp_number(ir_vector, i + 1);
            // cout<<"is_main: "<<is_main<<endl;
            // for (auto it = number_temp_map.begin(); it != number_temp_map.end(); ++it) 
            //     cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
            // cout << "temp_number_riscv: " << temp_number_riscv << endl;
            // cout << pre_get_number << endl;
        }
        // cout<<ir_vector[i]<<endl;
        // riscv_result += ir_vector[i] + ":\n";
        riscv_result += translate_riscv(ir_vector[i]);
        // riscv_result += "\n";
    }
    // cout<<riscv_result<<endl;
    return riscv_result;
}

//判断给定字符串是否为数字/*OK*/
int is_digit(string number)
{
    for(int i = 0; i < (int)number.size(); i++)
    {
        if(!isdigit(number[i]))
            return 0;
    } 
    return 1;
}

//获取对应函数中临时变量个数/*OK*/
//内部数组定义map值为负
void get_temp_number(vector<string> ir_vector, int index)
{
    // int flag = 1;
    for(int i = index; i < (int)ir_vector.size(); i++)
    {
        // flag = 1;
        if(ir_vector[i].compare(0, 8, "FUNCTION") == 0)
            break;
        else if(ir_vector[i].compare(4, 5, "PARAM") == 0)
            pre_get_number++;
        int t_pos = 0;
        string tnumber;
        //先获得 t 的位置
        while((t_pos = (int)ir_vector[i].find("t", t_pos)) != std::string::npos) 
        {
            // cout<<ir_vector[i]<<endl;
            int space_pos = ir_vector[i].find(' ', t_pos);
            if(space_pos != std::string::npos) 
                // 使用substr()截取子串
                tnumber = ir_vector[i].substr(t_pos + 1, space_pos - t_pos - 1);  
            else
                tnumber = ir_vector[i].substr(t_pos + 1);
            //判断是否为temp临时变量
            if(is_digit(tnumber))
                {
                    if(number_temp_map.find(tnumber) == number_temp_map.end())
                        number_temp_map[tnumber] = temp_number_riscv++;
                }
                else
                    break;
            t_pos++;
        }
        //DEC后序判断，确保顺序正常
        if(ir_vector[i].compare(4, 3, "DEC") == 0)
        {
            int c_pos = ir_vector[i].find("#");
            temp_number_riscv += stoi(ir_vector[i].substr(c_pos + 1)) / 4 - 1;
            number_temp_map[tnumber] = -number_temp_map[tnumber];
        }
    }
    return;
}

//通过临时变量获得对映栈中位置
//接口为tx
int get_position(string return_temp)
{
    int pos = return_temp.find("t");
    return number_temp_map[return_temp.substr(pos + 1)];
}

//删除字符串中指定字符并返回
string delete_symbol(string input_str, string delete_ch)
{
    int pos = input_str.find(delete_ch);
    return input_str.erase(pos, 1);
}

//转换具体的IR中间代码
string translate_riscv(string ir_input)
{
    // cout << ir_input << endl;
    string result = "";
    //处理函数头情况
    if(ir_input.compare(0, 8, "FUNCTION") == 0)
    {
        //已经带有 : 换行符
        result += ir_input.substr(9) + "\n";
        //打印函数必要部分
        result += print_tab(4) + "addi sp, sp, -" + to_string(4 * temp_number_riscv) + "\n";
        result += print_tab(4) + "sw s0, 0(sp)\n";
        result += print_tab(4) + "sw ra, 4(sp)\n";
        result += print_tab(4) + "addi s0, sp, " + to_string(4 * temp_number_riscv) + "\n";
        get_number = 0;
    }
    //注意print_tab，偏移量为 4
    else if(ir_input.compare(4, 6, "RETURN") == 0)
    {
        //有具体返回内容
        if(ir_input.size() > 10)
        {
            string return_temp = ir_input.substr(11);
            int sp_position = get_position(return_temp);
            if(is_main)
                result += print_tab(4) + "lw a1, " + to_string(4 * sp_position) + "(sp)\n";
            else
                result += print_tab(4) + "lw a0, " + to_string(4 * sp_position) + "(sp)\n";
        }
        result += print_tab(4) + "lw s0, 0(sp)\n";
        result += print_tab(4) + "lw ra, 4(sp)\n";
        result += print_tab(4) + "addi sp, sp, " + to_string(4 * temp_number_riscv) + "\n";
        if(is_main)
        {
            result += print_tab(4) + "li a0, 10\n";
            result += print_tab(4) + "ecall\n";
        }
        else
            result += print_tab(4) + "ret\n";            
    }
    //全局变量标签
    else if(ir_input.compare(0, 6, "GLOBAL") == 0)
        result += ir_input.substr(7) + "\n";
    //全局空间变脸赋值
    else if(ir_input.compare(4, 5, ".WORD") == 0)
    {
        int c_pos = ir_input.find("#");
        result +=  print_tab(4) + ".word " + ir_input.substr(c_pos + 1) + "\n"; 
    }
    //无条件跳转goto
    else if(ir_input.compare(4, 4, "GOTO") == 0)
    {
        int l_pos = ir_input.find("L");
        result += print_tab(4) + "j " +ir_input.substr(l_pos) + "\n";
    }
    //label标签的转化
    else if(ir_input.compare(2, 5, "LABEL") == 0)
    {
        int l_pos = ir_input.find("L", 8);
        result += ir_input.substr(l_pos) + "\n"; 
    }
    //if条件判断转化
    else if(ir_input.compare(4, 2, "IF") == 0)
    {
        string true_input = ir_input.substr(4);
        int goto_pos = true_input.find("GOTO");
        int t1_pos = true_input.find("t");
        int t2_pos = true_input.find("t", t1_pos + 1); 
        int check_pos = true_input.find(" ", t1_pos);
        int t1_stack = get_position(true_input.substr(t1_pos, check_pos - t1_pos));
        int t2_stack = get_position(true_input.substr(t2_pos, goto_pos - t2_pos - 1));
        int l_pos = true_input.find("L");
        // cout<<true_input.substr(t1_pos, check_pos - t1_pos);
        // cout<<true_input.substr(t2_pos, goto_pos - t2_pos - 1)<<endl;
        result += print_tab(4) + "lw t0, " + to_string(t1_stack * 4) + "(sp)\n";
        result += print_tab(4) + "lw t1, " + to_string(t2_stack * 4) + "(sp)\n";
        if(true_input.find("==") != std::string::npos)
            result += print_tab(4) + "beq t0, t1, " + true_input.substr(l_pos) + "\n";
        else if(true_input.find("!=") != std::string::npos)
            result += print_tab(4) + "bne t0, t1, " + true_input.substr(l_pos) + "\n";
        else if(true_input.find(">=") != std::string::npos)
            result += print_tab(4) + "bge t0, t1, " + true_input.substr(l_pos) + "\n";
        else if(true_input.find("<=") != std::string::npos)
            result += print_tab(4) + "ble t0, t1, " + true_input.substr(l_pos) + "\n";
        else if(true_input.find(">") != std::string::npos)
            result += print_tab(4) + "bgt t0, t1, " + true_input.substr(l_pos) + "\n";
        else if(true_input.find("<") != std::string::npos)
            result += print_tab(4) + "blt t0, t1, " + true_input.substr(l_pos) + "\n";
    }
    //函数参数传递
    else if(ir_input.compare(4, 3, "ARG") == 0)
    {
        int t1_pos = ir_input.find("t");
        int t1_stack = get_position(ir_input.substr(t1_pos));
        if(arg_number >= 8)
        {
            //需要使用栈传递参数
            if(t1_stack < 0 )
            {
                t1_stack = -t1_stack;
                result += print_tab(4) + "addi t0, sp, " + to_string(t1_stack * 4) + "\n";
            }
            else
                //获取传递的参数
                result += print_tab(4) + "lw t0, " + to_string(t1_stack * 4 + sp_offset) + "(sp)\n";
            //移动栈针
            result += print_tab(4) + "addi sp, sp, -4\n";
            sp_offset += 4;
            //存入帧中
            result += print_tab(4) + "sw t0, 0(sp)\n"; 
        }
        else
        {
            if(t1_stack < 0 )
            {
                t1_stack = -t1_stack;
                result += print_tab(4) + "addi t0, sp, " + to_string(t1_stack * 4) + "\n";
                result += print_tab(4) + "mv a" + to_string(arg_number) + ", t0\n";
            }
            else
                result += print_tab(4) + "lw a" + to_string(arg_number) + ", " + to_string(t1_stack * 4) + "(sp)\n";
        }
        arg_number++;
    }
    //记录函数参数
    else if(ir_input.compare(4, 5, "PARAM") == 0)
    {
        int t1_pos = ir_input.find("t");
        int t1_stack = get_position(ir_input.substr(t1_pos));
        if(get_number >= 8)
        {
            //从栈中获取
            int get_offset = temp_number_riscv * 4;
            get_offset += (pre_get_number - 8) * 4 - 4 * (get_number - 7);
            result += print_tab(4) + "lw t0, " + to_string(get_offset) + "(sp)\n";
            result += print_tab(4) + "sw t0, " + to_string(t1_stack * 4) + "(sp)\n";
        }
        else
            result += print_tab(4) + "sw a" + to_string(get_number) + ", " + to_string(t1_stack * 4) + "(sp)\n";
        get_number++;
    }
    //调用对映函数
    else if(ir_input.compare(4, 4, "CALL") == 0)
    {
        string true_input = ir_input.substr(4);
        //write临时变量的情况
        if(true_input.compare(5, 5, "write") == 0 && true_input.size() == 10)
        {
           result += print_tab(4) + "mv a1, a0\n";
           result += print_tab(4) + "li a0, 1\n";
           result += print_tab(4) + "ecall\n";
        }
        else
            result += print_tab(4) + "call " + true_input.substr(5) + "\n";
        arg_number = 0;
        result += print_tab(4) + "addi sp, sp, " + to_string(sp_offset) + "\n";
        sp_offset = 0;
    }
    //数组赋初值
    else if(ir_input.compare(4, 2, "x ") == 0)
        //直接传递语句
        array_info_riscv = ir_input;
    else if(ir_input.compare(4, 2, "y ") == 0)
    {
        int t_pos = array_info_riscv.find("t");
        int add_pos = array_info_riscv.find("+ #");
        int stack_pos = get_position(array_info_riscv.substr(t_pos, add_pos - t_pos - 1)) * 4;
        int c_pos = ir_input.find("#");
        if(stack_pos < 0)
        {
            stack_pos = -stack_pos;
            stack_pos += stoi(array_info_riscv.substr(add_pos + 3));
            // result += print_tab(4) + "addi t0, sp, " + to_string(stack_pos) + "\n";
            string assign_number = ir_input.substr(c_pos + 1);
            result += print_tab(4) + "li t1, " + assign_number + "\n";
            // result += print_tab(4) + "sw t1, 0(t0)\n";
            result += print_tab(4) + "sw t1, " + to_string(stack_pos) + "(sp)\n";
        }
        else
        {
            stack_pos += stoi(array_info_riscv.substr(add_pos + 3));
            string assign_number = ir_input.substr(c_pos + 1);
            result += print_tab(4) + "li t0, " + assign_number + "\n";
            result += print_tab(4) + "sw t0, " + to_string(stack_pos) +"(sp)\n";
        }
        
    }
    //数组/全局变量赋值情况
    else if(ir_input.compare(4, 2, "*t") == 0)
    {
        string true_input = ir_input.substr(4);
        int assign_pos = true_input.find("=");
        int t1_pos = true_input.find("t");
        int t2_pos = true_input.find("t", t1_pos + 1);
        int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
        int t2_stack = get_position(true_input.substr(t2_pos));
        result += print_tab(4) + "lw t0, " + to_string(t2_stack * 4) + "(sp)\n";
        //取得地址
        result += print_tab(4) + "lw t1, " + to_string(t1_stack * 4) + "(sp)\n";
        result += print_tab(4) + "sw t0, 0(t1)\n";
    }
    else if(ir_input.compare(4, 1, "t") == 0)
    {
        //有关临时变量操作的部分
        string true_input = ir_input.substr(4);
        int assign_pos = true_input.find("=");
        //int变量赋初值
        if(true_input.find("= #") != std::string::npos)
        {
            int stack_pos = get_position(true_input.substr(0, assign_pos - 1));
            int c_pos = true_input.find("#");
            string assign_number = true_input.substr(c_pos + 1);
            result += print_tab(4) + "li t0, " + assign_number + "\n";
            result += print_tab(4) + "sw t0, " + to_string(4 * stack_pos) + "(sp)\n";
        }
        //函数调用相关 x = call
        else if(true_input.find("CALL") != std::string::npos)
        {
            int call_pos = true_input.find("CALL");
            //调用函数部分
            string call_fun = true_input.substr(call_pos);
            int t1_pos = true_input.find("t");
            int assign_pos = true_input.find("=");
            int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
            //read函数调用特殊处理
            if(call_fun == "CALL read")
            {
                result += print_tab(4) + "li a0, 6\n";
                result += print_tab(4) + "ecall\n";
            }
            else
                result += print_tab(4) + "call " + true_input.substr(call_pos + 5) + "\n";
            result += print_tab(4) + "addi sp, sp, " + to_string(sp_offset) + "\n";
            result += print_tab(4) + "sw a0, " + to_string(t1_stack * 4) + "(sp)\n";
            arg_number = 0;
            sp_offset = 0;
        }
        //获得全局变量标签地址
        else if(true_input.find("= &") != std::string::npos)
        {
            int t_pos = true_input.find("t");
            int t_stack = get_position(true_input.substr(t_pos, assign_pos - t_pos - 1));
            int l_pos = true_input.find("L");
            string label_str = true_input.substr(l_pos);
            result += print_tab(4) + "la t0, " + label_str + "\n";
            result += print_tab(4) + "sw t0, " + to_string(t_stack * 4) + "(sp)\n";
        }
        //地址取数赋值
        else if(true_input.find("= *") != std::string::npos)
        {
            int t1_pos = true_input.find("t");
            int t2_pos = true_input.find("t", t1_pos + 1); 
            int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
            int t2_stack = get_position(true_input.substr(t2_pos));
            result += print_tab(4) + "lw t0, " + to_string(t2_stack * 4) + "(sp)\n";
            result += print_tab(4) + "lw t0, 0(t0)\n";
            result += print_tab(4) + "sw t0, " + to_string(t1_stack * 4) +"(sp)\n";
        }
        //实际不存在寄存器与立即数间四则运算
        //只有 * #4 的情况
        else if(true_input.find("* #") != std::string::npos)
        {
            int t1_pos = true_input.find("t");
            int t2_pos = true_input.find("t", t1_pos + 1);
            int ope_pos = true_input.find("*");
            int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
            int t2_stack = get_position(true_input.substr(t2_pos, ope_pos - t2_pos - 1));
            result += print_tab(4) + "lw t0, " + to_string(t2_stack * 4) + "(sp)\n";
            result += print_tab(4) + "slli t0, t0, 2\n";
            result += print_tab(4) + "sw t0, " + to_string(t1_stack * 4) + "(sp)\n";
        }
        //单目运算符
        //--暂时未考虑！
        else if(true_input.find("= -") != std::string::npos || true_input.find("= +") != std::string::npos)
        {
            int t1_pos = true_input.find("t");
            int t2_pos = true_input.find("t", t1_pos + 1); 
            int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
            int t2_stack = get_position(true_input.substr(t2_pos));
            result += print_tab(4) + "lw t0, " + to_string(t2_stack * 4) + "(sp)\n";
            if(true_input.find("-") != std::string::npos)
                result += print_tab(4) + "sub t0, x0, t0\n";
            result += print_tab(4) + "sw t0, " + to_string(t1_stack * 4) + "(sp)\n";
        }
        //寄存器间四则运算
        else if(true_input.find("+") != std::string::npos || true_input.find("-") != std::string::npos ||
            true_input.find("*") != std::string::npos || true_input.find("/") != std::string::npos ||
            true_input.find("%") != std::string::npos)
        {
            string operation;
            int t1_pos = true_input.find("t");
            int t2_pos = true_input.find("t", t1_pos + 1); 
            int t3_pos = true_input.find("t", t2_pos + 1); 
            int ope_pos = t3_pos - 2;
            int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
            int t2_stack = get_position(true_input.substr(t2_pos, ope_pos - t2_pos - 1));
            int t3_stack = get_position(true_input.substr(t3_pos));
            if(t2_stack < 0 )
            {
                t2_stack = -t2_stack;
                result += print_tab(4) + "addi t0, sp, " + to_string(t2_stack * 4) + "\n";
            }
            else
                result += print_tab(4) + "lw t0, " + to_string(t2_stack * 4) + "(sp)\n";
            result += print_tab(4) + "lw t1, " + to_string(t3_stack * 4) + "(sp)\n";
            if(true_input.find("+") != std::string::npos)
                result += print_tab(4) + "add t2, t0, t1\n";
            else if(true_input.find("-") != std::string::npos)
                result += print_tab(4) + "sub t2, t0, t1\n";
            else if(true_input.find("*") != std::string::npos)
                result += print_tab(4) + "mul t2, t0, t1\n";
            else if(true_input.find("/") != std::string::npos)
                result += print_tab(4) + "div t2, t0, t1\n";
            else if(true_input.find("%") != std::string::npos)
                result += print_tab(4) + "rem t2, t0, t1\n";
            result += print_tab(4) + "sw t2, " + to_string(t1_stack * 4) + "(sp)\n";
        }
        //常量间赋值
        else if(true_input.find("= t") != std::string::npos)
        {
            //数组间直接赋值未考虑
            int t1_pos = true_input.find("t");
            int t2_pos = true_input.find("t", t1_pos + 1); 
            int t1_stack = get_position(true_input.substr(t1_pos, assign_pos - t1_pos - 1));
            int t2_stack = get_position(true_input.substr(t2_pos));
            result += print_tab(4) + "lw t0, " + to_string(t2_stack * 4) + "(sp)\n";
            result += print_tab(4) + "sw t0, " + to_string(t1_stack * 4) + "(sp)\n";
        }
    }
    return result;
}