#include<iostream>
#include<string>    
#include<vector>
#include<sstream>
#include<fstream>
#include<map>
using namespace std;

//全局变量传入用于解决
vector<string> input_language;
//定义语义传递信息
int return_int_value ;
int index = 0;
int total;
//记录变量
map<string,int> symbol_table;
//记录是否为四则运算处理
int op_flag = 0;
//记录 exps 的结果vector
vector<int> exps_result;

void error();
int stm();
int exps();
int exp();
int exp_op();
bool match_id(int index);
void print_language(vector<string>);
bool isNumber(string& str);
string find_next_op(int index);

//输出错误情况
void error() { cout << "Error" << endl; }
//字符串判断
bool match_id(int index)
{
    for(int i = 0; i < input_language[index].size(); i++)
    {
        if(!isalpha(input_language[index][i]))
            return 0;
    }
    if(input_language[index] == "print")
        return 0;
    return 1;
}
//语句
int stm()
{
    int result = 0;
    if(index + 1 < total && input_language[index] == "print" && input_language[index + 1] == "(")
    {
        index += 2;
        // cout << index << endl;
        exps_result.clear();
        result = exps();
        // cout << index << endl;
        // cout << total << endl;
        if( result && index < total && input_language[index] == ")")
        {
            index++;
            for(int i = 0; i < exps_result.size(); i++)
                cout << exps_result[i] << " ";
            cout << endl;
        }
    }
    else if(index < total && match_id(index) && input_language[index + 1] == "=")
    {
        index += 2;
        int record = index;
        result = exp();
        // cout << "----- " << return_int_value << " ------" << endl;
        // cout << "----- " << input_language[record - 2] << " ------" << endl;
        // cout << "----- " << result << " ------" << endl;
        symbol_table[input_language[record - 2]] = return_int_value;
        // cout << "----- " << input_language[record - 2];
        // cout << " ----- " << return_int_value << " ------" << endl;
    }
    if(result == 0)
        return 0;
    else
    {
        if(index < total && input_language[index] == ";")
        {
            index++;
            // cout << "index: " << index << endl;
            return stm();
        }
    }
    return result;
}
//表达式组
int exps()
{
    int result = 0;
    result = exp();
    exps_result.push_back(return_int_value);
    // cout << "----- " << return_int_value << " ------" << endl;
    if(result == 0)
        return 0;
    if(index < total && input_language[index] == ",")
    {
        index++;
        return exps();
    }
    return result;
}
//表达式
int exp()
{
    int result = 0;
    if(find_next_op(index) != "" && !op_flag)
    {
        op_flag = 1;
        result = exp_op();
    }
    else if(index + 1 < total && match_id(index) && input_language[index + 1] != "=")
    {
        if(symbol_table.find(input_language[index]) != symbol_table.end())
        {
            index++;
            return_int_value = symbol_table[input_language[index - 1]];
            result = 1;
        }
        else
            return 0;
    }
    else if(index < total && isNumber(input_language[index]))
    {
        return_int_value = stoi(input_language[index]);
        // cout << return_int_value << endl;
        // cout << index << endl;
        index++;
        result = 1;
    }
    else if(index < total && input_language[index] == "(")
    {
        index++;
        int return_flag = op_flag;
        op_flag = 0;
        result = exp();
        op_flag = return_flag;
        if(result && index < total && input_language[index] == ")")
            index++;
        else
            return 0;
    }
    else if((index + 1 < total && match_id(index) && input_language[index + 1] == "=") || 
        index + 1 < total && input_language[index] == "print" && input_language[index + 1] == "(")
    {
        result = stm();
        if(result && index < total && input_language[index] == ",")
        {
            index++;
            result = exp();
        }
        else
            return 0;
    }
    else
        return 0;
    // cout << index << " " << result << endl;
    return result;
}
//计算部分
int exp_op()
{
    int result = 0;
    result = exp();
    // cout << "----- " << return_int_value << " ------" << endl;
    // cout << "----- " << index << " " << result << " ------" << endl;
    if(result == 0)
        return 0;
    int return_left = return_int_value;
    string op_temp;
    while((op_temp = find_next_op(index)) != "")
    {
        // cout << "----- " << op_temp << " ------" << endl;
        if(op_temp == "*")
        {
            index++;
            result = exp();
            if(result == 0)
                return 0;
            return_left *= return_int_value;
        }
        else if(op_temp == "/")
        {
            index++;
            result = exp();
            if(result == 0)
                return 0;
            if(return_int_value == 0)
                return 0;
            return_left /= return_int_value;
        }
        else
        {
            string op_in;
            index++;
            result = exp();
            // cout << "----- " << return_int_value << " ------" << endl;
            if(result == 0)
                return 0;
            int return_right = return_int_value;
            while((op_in = find_next_op(index)) != "" && (op_in == "*" || op_in == "/"))
            {
                if(op_in == "*")
                {
                    index++;
                    result = exp();
                    if(result == 0)
                        return 0;
                    return_right *= return_int_value;
                }
                else if(op_in == "/")
                {
                    index++;
                    result = exp();
                    if(result == 0)
                        return 0;
                    if(return_int_value == 0)
                        return 0;
                    return_right /= return_int_value;
                }
            }
            if(op_temp == "+")
                return_left += return_right;
            else if(op_temp == "-")
                return_left -= return_right;
        }
    }
    return_int_value = return_left;
    op_flag = 0;
    return result;  
}
//输出内容
void print_language(vector<string> language)
{
    for (int i = 0; i < language.size(); i++)
        cout << language[i] << endl;
}
//判断数字函数
bool isNumber(string& str) 
{
    char ch;
    for (int i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))
            return false;
    return true;
}
//判断下一个运算符
string find_next_op(int index)
{
    int temp_index = index;
    while(temp_index < input_language.size())
    {
        if(input_language[temp_index] == "+" || input_language[temp_index] == "-" || 
            input_language[temp_index] == "*" || input_language[temp_index] == "/")
            return input_language[temp_index];
        else if(input_language[temp_index] == "(")
        {
            while(input_language[temp_index] != ")")
                temp_index++;
        }
        else if(input_language[temp_index] == ")" || input_language[temp_index] == "," ||
            input_language[temp_index] == ";" || input_language[temp_index] == "=")
        {
            return "";
        }
        temp_index++;
    }
    return "";
}

int main()
{
    // 打开文件
    ifstream input_file("input.txt");
    if (!input_file.is_open()) 
    {
        cout << "无法打开文件 " << endl;
        return 1;
    }
    // 读取文件内容
    stringstream string_in;
    string_in << input_file.rdbuf();
    input_file.close();

    string word;
    while (string_in >> word) 
        input_language.push_back(word);
    // print_language(input_language);
    total = input_language.size();
    int result = stm();
    if (result == 1)
        cout << "--- Success ---" << endl;
    return result;
}