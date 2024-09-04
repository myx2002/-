#include"symbol_test.hh"
struct symbol_table* new_table = new symbol_table; 

/*定义冲突仅需测试当前symbol table*/
/*使用是否定义需要测试全symbol table*/
//临时变量数值
int now_temp_num = 1;
//输出规范化
int tab_num = 0;
//记录函数传递值
string now_design;
//记录函数返回值
string return_design;
//记录当前函数名称
string new_func_name;
//记录当前标签位置
int now_temp_label = 1;
//数组总大小
int array_count;
//数组当前大小
int array_now;
//记录当前数组维度信息
vector<int> array_info;
//记录数组具体赋值信息
vector<int> array_def_num;
//数组赋值全局/一般记录
int state_flag;
//记录全局变量的临时使用
// int now_temp_global = 1;
string false_design;
string true_design;
//统计！次数
int not_flag = 0;
//判断对数组地址/数值的需求
//0表示value，1表示address（默认0）
int value_address = 0;
//用于记录函数名
string del_func_name;
//return函数值时需要特殊处理
int return_func = 0;

/*返回 0 表示不存在问题
  返回 1 表示存在错误类型匹配*/
/*已考虑 func、define 情况 */
/*OK*/
string translate_ir(node* temp)
{
    string result = "";
    if(temp == NULL)
        return result;
    //root点
    if(temp->type == "START")/*OK 没有右节点*/
    {
        struct symbol_table* temp_table = new symbol_table;
        temp_table->next = NULL;
        new_table->next = temp_table;
        build_base_func_ir();
        if(temp->left != NULL)
            result += translate_ir(temp->left);

        //可能不止一个right节点
        if(temp->left->right == NULL){}
        else
        {
            node* temp_node = temp->left;
            while(temp_node->right != NULL)
            {
                temp_node = temp_node->right;
                result += translate_ir(temp_node);
            }
        }
        return result;
    }
    else if(temp->type == "FUNC_DEF")/*OK*/
    {
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        if(temp->left->type == "INT")
            temp_leaf->kind = symbol_leaf::INT;
        else if(temp->left->type == "VOID")
            temp_leaf->kind = symbol_leaf::VOID;
        new_table->next->var_def[temp->left->right->name] = temp_leaf;
        del_func_name = temp->left->right->name;

        result += "FUNCTION " + temp->left->right->name + ":\n";
        result += func_def_ir(temp->left->right->left);

        if(temp_leaf->kind == symbol_leaf::VOID)
        {
            string return_temp = "RETURN\n";
            int return_len = return_temp.length();
            if(result.compare(result.length() - return_len, return_len, return_temp))
            {
                result += print_tab(4);
                result += "RETURN\n";
            }
        }
        return result;
    }
    else if(temp->type == "DECL")
        result += global_def(temp->left->left);
    return result;
}

/*全局变量的定义*/
//IDENT为接口 /*OK*/
string global_def(node* temp)
{
    string result = "";
    //一般变量未赋值
    if(temp->left == NULL || temp->left->type == "ASSIGN")
    {
        //global变量是标签
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        temp_leaf->kind = symbol_leaf::INT;
        temp_leaf->temp_num = -now_temp_label;
        new_table->next->var_def[temp->name] = temp_leaf;
        now_design = "L" + to_string(now_temp_label);
        result += "GLOBAL " + now_design +":"+"\n";
        now_temp_label++;
        result += print_tab(4);
        if(temp->left == NULL)
            result += ".WORD #0\n";
        else
        {
            result += ".WORD #";
            result += to_string(temp->left->right->left->value) + "\n";
        }
    }
    //数组情况
    else
    {
        array_count = 1;
        array_now = 0;
        //global标签记录(暂时仅记录有符号信息/未记录数组维度信息)
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        temp_leaf->kind = symbol_leaf::ARRAY;
        temp_leaf->temp_num = -now_temp_label;
        node* temp_node = temp->left->left;
        //记录数组定义信息
        while(temp_node != NULL)
        {
            temp_leaf->array_size.push_back(temp_node->value);
            array_count *= temp_node->value;
            temp_node = temp_node->right;
        }
        new_table->next->var_def[temp->name] = temp_leaf;
        array_info = temp_leaf->array_size;
        //按格式输出信息
        now_design = "L" + to_string(now_temp_label);
        result += "GLOBAL " + now_design +":"+"\n";
        now_temp_label++;
        if(temp->left->right == NULL)
        {
            for(int i = 0; i < array_count; i++)
            {
                result += print_tab(4);
                result += ".WORD #0\n";
            }    
        }
        else
        {
            state_flag = 1;
            array_def_num.clear();
            result += array_ir(temp->left->right->left, 1);
        }
    }
    return result;
}

//以assign最外层大括号为接口 /*OK*/
string array_ir(node* temp, int level)
{
    string result = "";
    int layer = 0;
    int arr_top = (int)array_info.size();
    int temp_top = 1;
    int temp_top_next = 1;
    //获得第一个EXP
    node* temp_node = temp->left;
    while(temp_node != NULL)
    {
        if(temp_node->type == "EXP")
        {
            array_now++;
            array_def_num.push_back(temp_node->left->value);
        } 
        else if(temp_node->type == "LBRACE")
        {
            layer = find_layer(temp_node);
            //需要判断是否合法
            temp_top = 1;
            //有指定数组内层向外走
            for(int arr_index = arr_top - layer; arr_index >= 0 ; arr_index--)
            {
                temp_top *= array_info[arr_index];
                if(array_now % temp_top == 0 && array_now != 0)
                    temp_top_next = temp_top;
                //处理直接嵌套左大括号的情况
                else if(array_now == 0 && arr_index == arr_top - 1)
                    temp_top_next = temp_top;
            }
            result += array_ir(temp_node, level + 1);
        }
        else if(temp_node->type == "RBRACE")
        {
            while(array_now % temp_top_next != 0)
            {
                array_now++;
                array_def_num.push_back(0);
            }
        }
        temp_node = temp_node->right;
    } 
    //补全最外层大括号
    if(level == 1)
    {
        while(array_now < array_count)
        {
            array_now++;
            array_def_num.push_back(0);
        }
        //state_flag 为 1 表示全局变量，为 0 表示一般赋值
        for(int i = 0;i < (int)array_def_num.size(); i++)
        {
            result += print_tab(4);
            if(state_flag == 1)
            {
                result += ".WORD #";
                result += to_string(array_def_num[i]) + "\n";
            }
            else
            {
                
                result += "x = " + now_design + " + #" + to_string(i * 4) + "\n";
                result += print_tab(4) + "y =" + " #" + to_string(array_def_num[i]) + "\n";
                result += print_tab(4) + "*x = y\n";
            
            }
        }
    }
    return result;
}

//考虑 read/write 函数的情况 /*OK*/
void build_base_func_ir()
{
    //先建立int read()
    struct symbol_leaf* temp_leaf = new symbol_leaf;
    new_table->next->var_def["read"] = temp_leaf;
    temp_leaf->kind = symbol_leaf::INT;
    //再建立 void write(int)
    struct symbol_leaf* temp_leaf_t = new symbol_leaf;
    new_table->next->var_def["write"] = temp_leaf_t;
    temp_leaf_t->kind = symbol_leaf::VOID;
    temp_leaf_t->param.push_back("INT");
}

//输出中间代码时调解输出格式/*OK*/
string print_tab(int num)
{
    string result = "";
    for(int i = 0; i < num; i++)
        result += " ";
    return result;
}

/*某个函数*/
//测试函数定义(接口是左括号)/*OK*/
string func_def_ir(node* temp)
{
    string result = "";
    //为function获得参数列表
    result += func_para_ir(temp->left);
    //测试函数体
    if(temp->right->left->type == "BLOCK")
    {
        result += block_ir(temp->right->left->left);
        //消除最临近的符号表
        new_table->next = new_table->next->next;
    }
    return result;
}

// /*函数参数列表*/
// //函数建立参数列表（接口是para起始值（空/int））/*OK*/
string func_para_ir(node* temp)
{
    string result = "";
    struct symbol_table* temp_table = new symbol_table;
    temp_table->next = new_table->next;
    new_table->next = temp_table;
    if(temp == NULL)
        return result;
    // 其余情况建立详细的参数列表
    else
    {
        node* temp_node = temp;
        while(temp_node != NULL)
        {
            result += decl_def_func_ir(temp_node);
            temp_node = temp_node -> right;
        }
    }
    return result;
}

/*某个block*/
//Block处理函数（接口是左大括号）/*OK*/
string block_ir(node* temp)
{
    // cout<<"test block"<<endl;
    string result = "";
    if(temp->left == NULL)
        return result;
    else
    {
        //DECL处理
        if(temp->left->type == "DECL")
        {
            node* temp_node = temp->left->left->left;
            while(temp_node != NULL)
            {
                result += decl_def_ir(temp_node);
                temp_node = temp_node->right;
            } 
        } 
        //STMT处理
        else if(temp->left->type == "STMT")
            result += stmt_def_ir(temp->left->left);
        //可能不止一个right
        if(temp->left->right == NULL)
            return result;
        else
        {
            node* temp_node = temp->left;
            while(temp_node->right != NULL)
            {
                temp_node = temp_node->right;
                //需要判断decl与stmt
                if(temp_node->type ==  "DECL" )
                {
                    node* temp_node_t = temp_node->left->left;
                    while(temp_node_t != NULL)
                    {
                        result += decl_def_ir(temp_node_t);
                        temp_node_t = temp_node_t->right;
                    } 
                }
                else if(temp_node->type == "STMT")
                    result += stmt_def_ir(temp_node->left);
            }
        }
    }
    return result;
}

/*某个声明*/
//声明处理函数（接口是ident）
//暂时认为只是函数体内的定义语句
//返回要带\n /*OK*/
string decl_def_ir(node* temp)
{
    string result = "";
    //不含assign一般变量
    if(temp->type == "IDENT" && (temp->left == NULL || temp->left->type != "ARRAY" ))
    {
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        temp_leaf->kind = symbol_leaf::INT;
        temp_leaf->temp_num = now_temp_num;
        new_table->next->var_def[temp->name] = temp_leaf;
        now_design = "t" + to_string(now_temp_num);
        now_temp_num++;
        if(temp->left == NULL)
        {
            //为初始化变量的处理（赋随机值/直接初始化为0）
            // result += print_tab(4);
            // result += now_design + " = " + "#0" + "\n";
        }
        else
        {
            if(temp->left->right->left->type == "FUNC_EXP")
                new_func_name = temp->left->right->left->left->name;
            result = exp_ir(temp->left->right->left);
        } 
        return result;
    }
    //数组情况
    else if(temp->type == "IDENT" && temp->left->type == "ARRAY")
    {
        array_count = 1;
        array_now = 0;
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        temp_leaf->kind = symbol_leaf::ARRAY;
        temp_leaf->temp_num = now_temp_num;
        node* temp_node = temp->left->left;
        //记录数组定义信息
        while(temp_node != NULL)
        {
            temp_leaf->array_size.push_back(temp_node->value);
            array_count *= temp_node->value;
            temp_node = temp_node->right;
        }
        new_table->next->var_def[temp->name] = temp_leaf;
        array_info = temp_leaf->array_size;
        //按格式输出信息
        now_design = "t" + to_string(now_temp_num);
        result += print_tab(4);
        result += "DEC " + now_design + " #" + to_string(array_count * 4) + "\n";
        now_temp_num++;
        if(temp->left->right == NULL)
        {
            for(int i = 0; i < array_count; i++)
            {
                result += print_tab(4);
                //使用临时变量x，y
                result += "x = " + now_design + " + #" + to_string(i * 4) + "\n";
                result += print_tab(4) + "y =" + " #0\n";
                result += print_tab(4) + "*x = y\n";
            }    
        }
        else
        {
            array_def_num.clear();
            state_flag = 0;
            result += array_ir(temp->left->right->left, 1);
        }
    }
    return result;
}

/*某个stmt*/
//STMT处理函数（STMT节点的left为接口）
string stmt_def_ir(node* temp)
{
    string result = "";
    if(temp->type == "EMPTY")
        return result;
    else if(temp->type == "BLOCK")
    {
        //开拓新symbol table
        struct symbol_table* temp_table = new symbol_table;
        temp_table->next = new_table->next;
        new_table->next = temp_table;
        result += block_ir(temp->left);
        new_table->next = new_table->next->next;
    }
    else if(temp->type == "IF")
    {
        int else_if = 0;
        if(temp->right != NULL && temp->right->type == "ELSE")
            else_if = 1;
        //false跳转方向
        string new_label_false = "L" + to_string(now_temp_label);
        now_temp_label++;
        false_design = new_label_false;
        //true跳转方向
        string new_label_true = "L" + to_string(now_temp_label);
        now_temp_label++;
        true_design = new_label_true;
        //cond判断
        result += cond_ir(temp->left->left);
        //显示true/false跳转方向
        result += print_tab(2) + "LABEL " + new_label_true + ":\n";
        result += stmt_def_ir(temp->left->right->left->left);
        string new_label_end = "L" + to_string(now_temp_label);
        now_temp_label++;
        if(else_if == 1)
            result += print_tab(4) + "GOTO " + new_label_end + "\n";
        result += print_tab(2) + "LABEL " + new_label_false + ":\n";
        //判断有无else
        if(else_if == 1)
        {
            result += stmt_def_ir(temp->right->left->left);
            result += print_tab(2) + "LABEL " + new_label_end + ":\n";
        }
        return result;
    }
    //--需要修改
    else if(temp->type == "WHILE")
    {
        //while可跳回区域
        string new_label_round = "L" + to_string(now_temp_label);
        now_temp_label++;
        result += print_tab(2) + "LABEL " + new_label_round + ":\n";
        //false跳转方向
        string new_label_false = "L" + to_string(now_temp_label);
        now_temp_label++;
        false_design = new_label_false;
        //true跳转方向
        string new_label_true = "L" + to_string(now_temp_label);
        now_temp_label++;
        true_design = new_label_true;
        //cond判断
        result += cond_ir(temp->left->left);
        //true跳转
        result += print_tab(2) + "LABEL " + new_label_true + ":\n";
        //结构体
        result += stmt_def_ir(temp->left->right->left->left);
        //循环跳转
        result += print_tab(4) + "GOTO " + new_label_round + "\n";
        //false 跳转
        result += print_tab(2) + "LABEL " + new_label_false + ":\n";
        return result;
    }
    else if(temp->type == "EXP")
    {
        //函数与表达式处理
        new_func_name = temp->left->left->name;
        result = exp_ir(temp->left, 0);
    }
    else if(temp->type == "RETURN")
    {
        //判断函数返回类型是否一致
        if(temp->left == NULL)
        {
            result += print_tab(4);
            result += "RETURN\n";
        }
        else
        {
            if(temp->left->left->type == "FUNC_EXP")
            {
                return_func = 1;
                new_func_name = temp->left->left->left->name;
                now_design = "t" + to_string(now_temp_num);
                now_temp_num++;
            }
            result = exp_ir(temp->left->left, 0);
            return_func = 0;
            result += print_tab(4);
            result += "RETURN ";
            result += return_design + "\n";
        }
    }
    else
    {
        //赋值的情况
        //一般变量情况
        if(temp->left->type == "INT")
        {
            struct symbol_table* temp_table = new_table->next;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::INT)
                {
                    if(temp->left->right->right->left->type == "FUNC_EXP")
                        new_func_name = temp->left->right->right->left->left->name;
                    if(temp_table->var_def[temp->name]->temp_num > 0)
                    {
                        now_design = "t" + to_string(temp_table->var_def[temp->name]->temp_num); 
                        result += exp_ir(temp->left->right->right->left);
                    }
                    else
                    {
                        string temp_design = "L" + to_string(-temp_table->var_def[temp->name]->temp_num); 
                        now_design = "t" + to_string(now_temp_num);
                        now_temp_num++;
                        result += print_tab(4) + now_design + " = &" + temp_design + "\n";
                        string no_temp = "t" + to_string(now_temp_num);
                        now_temp_num++;
                        //保留temp_now_design数值
                        string temp_now_design = now_design;
                        now_design = no_temp;
                        result += exp_ir(temp->left->right->right->left);
                        result += print_tab(4) + "*" + temp_now_design + " = " + no_temp + "\n";
                    }
                    break;
                }
                temp_table = temp_table->next;
            }
        }
        //数组
        else if(temp->left->type == "ARRAY")
        {
            //获得数组信息
            struct symbol_table* temp_table = new_table->next;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::ARRAY)
                    break;
                temp_table = temp_table->next;
            }
            vector<int> temp_array_dimen = temp_table->var_def[temp->name]->array_size;
            int temp_number = temp_table->var_def[temp->name]->temp_num;
            
            //记录使用数组维数            
            string array_size = "t" + to_string(now_temp_num);
            now_temp_num++;
            int index_array = 1;
            int array_mul = 1;
            //获得EXP节点
            node* temp_node = temp->left->left;
            //获得赋值位置
            // result += print_tab(4) + array_size + " = #0\n";
            int circle = 1;
            while(temp_node != NULL)
            {
                array_mul = 1;
                for(int i = index_array; i < (int)temp_array_dimen.size(); i++)
                    array_mul *= temp_array_dimen[i];
                result += exp_ir(temp_node->left, 0);
                string add_temp = "t" + to_string(now_temp_num);
                now_temp_num++;
                if(array_mul == 1)
                {
                    if(circle == 1)
                    {
                        result += print_tab(4) + array_size + " = " + return_design + "\n";
                        circle = 0;
                    }
                    else
                        result += print_tab(4) + array_size + " = " + array_size + " + " + return_design + "\n";
                    // result += print_tab(4) + array_size + " = " + return_design + "\n";
                }
                else
                {
                    string number_operation = "t" + to_string(now_temp_num);
                    now_temp_num++;
                    result += print_tab(4) + number_operation + " = #" +  to_string(array_mul) + "\n";
                    result += print_tab(4) + add_temp + " = " + return_design + " * " + number_operation + "\n";
                    if(circle == 1)
                    {
                        circle = 0;
                        result += print_tab(4) + array_size + " = " + add_temp + "\n";
                    }
                    else
                        result += print_tab(4) + array_size + " = " + array_size + " + " + add_temp + "\n";
                    // result += print_tab(4) + array_size + " = " + add_temp + "\n";
                }
                temp_node = temp_node ->right;
                index_array++;
            }
            result += print_tab(4) + array_size + " = " + array_size + " * #4\n";
            if(temp->left->right->right->left->type == "FUNC_EXP")
                new_func_name = temp->left->right->right->left->left->name;
            if(temp_number > 0)
            {
                now_design = "t" + to_string(temp_number);
                string temp_design = "t" + to_string(now_temp_num);
                now_temp_num++;
                result += print_tab(4) + temp_design + " = " + now_design + " + " + array_size +"\n";
                string no_temp = "t" + to_string(now_temp_num);
                now_temp_num++;
                now_design = no_temp;
                result += exp_ir(temp->left->right->right->left);
                result += print_tab(4) + "*" + temp_design + " = " + no_temp + "\n";
            }
            else
            {
                string temp_design = "L" + to_string(-temp_number);
                now_design = "t" + to_string(now_temp_num);
                now_temp_num++;
                result += print_tab(4) + now_design + " = &" + temp_design + "\n";
                result += print_tab(4) + now_design + " = " + now_design + " + " + array_size +"\n";
                string temp_now_design = now_design;
                string no_temp = "t" + to_string(now_temp_num);
                now_temp_num++;
                now_design = no_temp;
                result += exp_ir(temp->left->right->right->left);
                result += print_tab(4) + "*" + temp_now_design + " = " + no_temp + "\n";
            }
        }
    }
    return result;
}

/*condition条件判断*/
/*接口为逻辑运算符*/ /*OK*/
string cond_ir(node* temp, int single)
{
    string result = "";
    string return_left = "";
    string return_right = "";
    if(temp->type != "OR" && temp->type != "AND" && temp->type != "LT" && temp->type != "LE" && 
    temp->type != "GT" && temp->type != "GE" && temp->type != "EQ" && temp->type != "NE")
    {
        if(single == 0)
        {
            if(temp->type == "FUNC_EXP")
            {
                new_func_name = temp->left->name;
                now_design = "t" + to_string(now_temp_num);
                now_temp_num++;
                return_func = 1;
                result += exp_ir(temp, 0);
                return_func = 0;
            }
            else
                result += exp_ir(temp, 0);
        }
        else
        {
            not_flag = 0;
            if(temp->type == "FUNC_EXP")
            {
                new_func_name = temp->left->name;
                now_design = "t" + to_string(now_temp_num);
                now_temp_num++;
                return_func = 1;
                result += exp_ir(temp, 0);
                return_func = 0;
            }
            else
                result += exp_ir(temp, 0);
            string op_temp = " != ";
            if(not_flag == 1)
                op_temp = " == ";
            string cond_temp_num = return_design;
            //建立 0 等价
            string zero_temp = "t" + to_string(now_temp_num);
            now_temp_num++;
            result += print_tab(4) + zero_temp + " = #0\n";
            result += print_tab(4) + "IF " + cond_temp_num + op_temp + zero_temp;
            result += " GOTO " + true_design + "\n";
            result += print_tab(4) + "GOTO " + false_design + "\n";
        }
    }
    else if(temp->type == "AND")
    {
        string temp_true_design = true_design;
        //true跳转方向
        string new_label_true = "L" + to_string(now_temp_label);
        now_temp_label++;
        true_design = new_label_true;
        //前半OR
        result += cond_ir(temp->left, 1);
        result += print_tab(2) + "LABEL " + new_label_true + ":\n";
        true_design = temp_true_design;
        //后半OR
        result += cond_ir(temp->left->right, 1);
    }
    else if(temp->type == "OR")
    {
        string temp_false_design = false_design;
        //true跳转方向
        string new_label_false = "L" + to_string(now_temp_label);
        now_temp_label++;
        false_design = new_label_false;
        //前半OR
        result += cond_ir(temp->left, 1);
        result += print_tab(2) + "LABEL " + new_label_false + ":\n";
        false_design = temp_false_design;
        //后半OR
        result += cond_ir(temp->left->right, 1);
    }
    else
    {
        string op;
        if(temp->type == "EQ")
            op = " == ";
        else if(temp->type == "NE")
            op = " != ";
        else if(temp->type == "LT")
            op = " < ";
        else if(temp->type == "LE")
            op = " <= ";
        else if(temp->type == "GT")
            op = " > ";
        else if(temp->type == "GE")
            op = " >= ";
        // else if(temp->type == "NOT")
        //     op = " ! ";

        if(temp->left != NULL)
        {
            result += cond_ir(temp->left, 0);
            return_left = return_design;
        }
        if(temp->left->right != NULL)
        {
            result += cond_ir(temp->left->right, 0);
            return_right = return_design;
        }
        result += print_tab(4) + "IF ";        
        result += return_left + op + return_right + " GOTO " + true_design + "\n";
        result += print_tab(4) + "GOTO " + false_design + "\n";
    }
    return result;
} 

/*某个参数*/
//函数参数表建立过程(借口是参数起始（int/array）)/*OK*/
string decl_def_func_ir(node* temp)
{
    string result = "";
    if(temp->type == "INT")
    {
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        temp_leaf->kind = symbol_leaf::INT;
        temp_leaf->temp_num = now_temp_num;
        new_table->next->var_def[temp->left->name] = temp_leaf;
        new_table->next->next->var_def[del_func_name]->param.push_back(temp->type);
    }
    //数组情况
    else
    {
        node* temp_node = temp->left;
        node* temp_node_t = temp_node;
        while(temp_node -> left == NULL)
            temp_node = temp_node -> right;
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        temp_leaf->kind = symbol_leaf::ARRAY;
        temp_leaf->temp_num = now_temp_num;
        new_table->next->var_def[temp_node->left->name] = temp_leaf;
        temp_node = temp->left;
        int array_size = 0;
        //记录数组定义信息
        while(temp_node != NULL)
        {
            if(temp_node->type == "DEFAULT")
                temp_leaf->array_size.push_back(0);
            else
                temp_leaf->array_size.push_back(temp_node->value);
            temp_node = temp_node->right;
            array_size++;
        }
        //记录于func的param列表中
        new_table->next->next->var_def[del_func_name]->param.push_back(temp->type);
        //记录于func的param列表中
        new_table->next->next->var_def[del_func_name]->param.push_back(to_string(array_size));
        //记录具体维数
        while(temp_node_t != NULL)
        {
            if(temp_node_t->type == "DEFAULT")
                new_table->next->next->var_def[del_func_name]->param.push_back(to_string(0));
            else
                new_table->next->next->var_def[del_func_name]->param.push_back(to_string(temp_node_t->value));
            temp_node_t = temp_node_t->right;
        }
    }
    result += print_tab(4);
    result += "PARAM t" + to_string(now_temp_num) + "\n";
    now_temp_num++;
    // for(int i = 0;i < (int)new_table->next->next->var_def[del_func_name]->param.size(); i++)
    //     cout<<new_table->next->next->var_def[del_func_name]->param[i]<<endl;
    return result;
}

// /*某个exp表达式判断*/
// /*接口为EXP的left节点(ADD/SUB/MUL/DIV/MOD)*/
//不带多余空格 /*OK*/
string  exp_ir(node* temp, int return_index)
{
    string result = "";
    if(temp->type == "FUNC_EXP")
    {
        string temp_new_func_name = new_func_name;
        string temp_now_design = now_design;
        //获得函数参数
        struct symbol_table* temp_table = new_table->next;
        while(temp_table != NULL)
        {
            if(temp_table->var_def[temp->left->name] != NULL)
            {
                //指向exp
                node* temp_node = temp->left->left->left;
                //获得函数对映结点
                struct symbol_leaf* temp_leaf = temp_table->var_def[temp->left->name];
                // cout<<temp->left->name<<endl;
                vector<string> temp_param;
                int index_arr = 0;
                while(temp_node != NULL)
                {
                    // cout<<index_arr<<" ";
                    // cout<<temp_leaf->param.size()<<endl;
                    if(temp_node->left->type == "FUNC_EXP")
                    {
                        new_func_name = temp_node->left->left->name;
                        string temp_param_input = "t" + to_string(now_temp_num);
                        now_temp_num++;
                        now_design = temp_param_input;
                        result += exp_ir(temp_node->left);
                        temp_param.push_back(temp_param_input);
                    }
                    else if(temp_leaf->param[index_arr] == "ARRAY" && temp_node->type == "EXP")
                    {
                        // cout<<"test array"<<endl;
                        index_arr = index_arr + stoi(temp_leaf->param[index_arr + 1]) + 1;
                        value_address = 1;
                        result += exp_ir(temp_node->left, 0);
                        value_address = 0;
                        // cout<<return_design<<endl;
                        temp_param.push_back(return_design);
                    }
                    else
                    {
                        // cout<<temp->left->name<<endl;
                        // cout<<"---"<<endl;
                        result += exp_ir(temp_node->left, 0);
                        temp_param.push_back(return_design);
                    }  
                    temp_node = temp_node->right;
                    index_arr++;
                }
                for(int index = 0; index < (int)temp_param.size(); index++)
                {
                    result += print_tab(4);
                    result += "ARG ";
                    result += temp_param[index] + "\n";
                }
                result += print_tab(4);
                if(return_index == 1)
                    result += temp_now_design + " = ";
                else if(return_func == 1)
                {
                    result += temp_now_design + " = ";
                    return_design = temp_now_design;
                }
                result += "CALL " + temp_new_func_name + "\n";
                break;
            }
            temp_table = temp_table->next;
        }
        return result;
    }
    /* NUM 本身不返回 /n */
    else if(temp->type == "NUM")
    {
        if(return_index == 0)
        {
            string new_temp = "t" + to_string(now_temp_num);
            now_temp_num++;
            result += print_tab(4);
            result += new_temp + " = #" + to_string(temp->value) + "\n";
            return_design = new_temp;
        }   
        else
        {
            result += print_tab(4);
            result += now_design + " = ";
            result += "#" + to_string(temp->value) + "\n";
        }
        return result;
    }
    //--其他符号/括号注意
    else if((temp->type == "ADD") || (temp->type == "SUB") || (temp->type == "MUL") || 
        (temp->type == "DIV") || (temp->type == "MOD"))
    {
        string op;
        if(temp->type == "ADD")
            op = " + ";
        else if(temp->type == "SUB")
            op = " - ";
        else if(temp->type == "MUL")
            op = " * ";
        else if(temp->type == "DIV")
            op = " / ";
        else if(temp->type == "MOD")
            op = " % ";
        //记录返回结果
        string temp_now_design = now_design;
        //获得左侧结果
        if(temp->left->type == "FUNC_EXP")
        {
            new_func_name = temp->left->left->name;
            return_func = 1;
            now_design = "t" + to_string(now_temp_num);
            now_temp_num++;
            result += exp_ir(temp->left, 0);
            return_func = 0;
        }
        else
            result += exp_ir(temp->left, 0);
        string temp_left = return_design;
        //获得右侧结果
        if(temp->left->right->type == "FUNC_EXP")
        {
            new_func_name = temp->left->right->left->name;
            return_func = 1;
            now_design = "t" + to_string(now_temp_num);
            now_temp_num++;
            result += exp_ir(temp->left->right, 0);
            return_func = 0;
        }
        else
            result += exp_ir(temp->left->right, 0);
        string temp_right = return_design;
        if(return_index == 1)
        {
            //为最外层结果赋值
            result += print_tab(4);
            result += temp_now_design + " = ";
            result += temp_left + op + temp_right + "\n";
        }
        else
        {
            //嵌套调用
            string new_temp = "t" + to_string(now_temp_num);
            now_temp_num++;
            return_design = new_temp;
            result += print_tab(4);
            result += new_temp + " = ";
            result += temp_left + op + temp_right + "\n";
        }
    }
    else if(temp->type == "LPAREN")
    {
        now_design = "t" + to_string(now_temp_num);
        string temp_now_design = now_design;
        now_temp_num++;
        result = exp_ir(temp->left->left, 1);
        return_design = temp_now_design;
    }
    else if (temp->type == "RPAREN")
    {
        if(temp->right != NULL)
            result = exp_ir(temp->right, 0);
    }
    else if(temp->type == "NEGATIVE")
    {
        string temp_now_design = now_design;
        if(return_index == 1)
        {
            result += exp_ir(temp->left, 0);
            result += print_tab(4) + temp_now_design + " = -" + return_design + "\n";
        }
        else
        {
            result += exp_ir(temp->left, 0);
            string neg_temp = "t" + to_string(now_temp_num);
            now_temp_num++;
            result += print_tab(4) + neg_temp + " = -" + return_design + "\n";
            return_design = neg_temp;
        }
    }
    else if(temp->type == "POSITIVE")
    {
        if(return_index == 1)
            result += exp_ir(temp->left);
        else
            result += exp_ir(temp->left, 0);
    }
    else if(temp->type == "NOT")
    {
        not_flag = 1 - not_flag;
        if(return_index == 1)
            result += exp_ir(temp->left);
        else
            result += exp_ir(temp->left, 0);
    }
    else
    {
        if(temp->type == "IDENT" && temp->left->type == "INT")
        {
            //从当前全判断
            struct symbol_table* temp_table = new_table->next;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL)// && temp_table->var_def[temp->name]->kind == symbol_leaf::INT)
                {
                    if(temp_table->var_def[temp->name]->temp_num > 0)
                    {
                        if(return_index == 0)
                            return_design = "t"+to_string(temp_table->var_def[temp->name]->temp_num);
                        else
                        {
                            string assign_temp = "t" + to_string(temp_table->var_def[temp->name]->temp_num);
                            result += print_tab(4);
                            result += now_design + " = ";
                            result += assign_temp + "\n";
                        }
                    } 
                    else
                    {
                        string assign_temp = "L" + to_string(-temp_table->var_def[temp->name]->temp_num);
                        string temp_design = "t" + to_string(now_temp_num);
                        now_temp_num++;
                        result += print_tab(4) + temp_design + " = &" + assign_temp + "\n";
                        string no_temp;
                        if(value_address == 0 )
                        {
                            no_temp = "t" + to_string(now_temp_num);
                            now_temp_num++;
                            result += print_tab(4) + no_temp + " = *" + temp_design + "\n";
                        }
                        else
                        {
                            no_temp = temp_design;
                            value_address = 0;
                        }
                        if(return_index == 0)   
                            return_design = no_temp;
                        else
                            result += print_tab(4) + now_design + " = " + no_temp+"\n";
                    }
                    break;
                }
                temp_table = temp_table->next;
            }
        }
        else if(temp->type == "IDENT" && temp->left->type == "ARRAY")
        {
             //获得数组信息
            struct symbol_table* temp_table = new_table->next;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::ARRAY)
                    break;
                temp_table = temp_table->next;
            }
            vector<int> temp_array_dimen = temp_table->var_def[temp->name]->array_size;
            int temp_number = temp_table->var_def[temp->name]->temp_num;
            //记录使用数组维数            
            string array_size = "t" + to_string(now_temp_num);
            now_temp_num++;
            int index_array = 1;
            int array_mul = 1;
            //获得EXP节点
            node* temp_node = temp->left->left;
            //获得取值位置
            // result += print_tab(4) + array_size + " = #0\n";
            int circle = 1;
            while(temp_node != NULL)
            {
                array_mul = 1;
                // cout<<"-----"<<temp_array_dimen.size()<<endl;
                for(int i = index_array; i < (int)temp_array_dimen.size(); i++)
                    array_mul *= temp_array_dimen[i];
                //需要考虑不是数值的情况
                result += exp_ir(temp_node->left, 0);
                string add_temp = "t" + to_string(now_temp_num);
                now_temp_num++;
                if(array_mul == 1)
                {
                    if(circle == 1)
                    {
                        circle = 0;
                        result += print_tab(4) + array_size + " = " + return_design + "\n";
                    }
                    else
                        result += print_tab(4) + array_size + " = " + array_size + " + " + return_design + "\n";
                }
                else
                {
                    string number_operation = "t" + to_string(now_temp_num);
                    now_temp_num++;
                    result += print_tab(4) + number_operation + " = " + "#" + to_string(array_mul) + "\n";
                    result += print_tab(4) + add_temp + " = " + return_design + " * " + number_operation + "\n";
                    if(circle == 1)
                    {
                        circle = 0;
                        result += print_tab(4) + array_size + " = " + add_temp + "\n";
                    }
                    else
                        result += print_tab(4) + array_size + " = " + array_size + " + " + add_temp + "\n";
                }
                // array_size += temp_node->left->value * array_mul;
                // cout<<"index "<<temp_array_dimen[index_array - 1]<<" "<<array_size<<endl;
                temp_node = temp_node ->right;
                index_array++;
            }
            result += print_tab(4) + array_size + " = " + array_size + " * #4\n";
            //判断是否全局变量
            if(temp_number > 0)
            {
                string array_position = "t" + to_string(temp_number);
                string temp_design = "t" + to_string(now_temp_num);
                now_temp_num++;
                result += print_tab(4) + temp_design + " = " + array_position + " + " + array_size +"\n";
                string no_temp;
                if(value_address == 0)
                {
                    no_temp = "t" + to_string(now_temp_num);
                    now_temp_num++;
                    result += print_tab(4) + no_temp + " = *" + temp_design + "\n";
                }
                else
                {
                    no_temp = temp_design;
                    value_address = 0;
                }
                if(return_index == 0)
                    return_design = no_temp;
                else 
                    result += print_tab(4) + now_design + " = " +  no_temp + "\n";
            }
            else
            {
                string temp_now_design = now_design;
                string temp_design = "L" + to_string(-temp_number);
                now_design = "t" + to_string(now_temp_num);
                now_temp_num++;
                result += print_tab(4) + now_design + " = &" + temp_design + "\n";
                result += print_tab(4) + now_design + " = " + now_design + " + " + array_size +"\n";
                if(return_index == 1)
                    result += print_tab(4) + temp_now_design + " = *" + now_design + "\n";          
                else
                {
                    string no_temp;
                    if(value_address == 0)
                    {
                        no_temp = "t" + to_string(now_temp_num);
                        now_temp_num++;
                        result += print_tab(4) + no_temp + " = *" + now_design + "\n";
                    }
                    else
                    {
                        no_temp = now_design;
                        value_address = 0;
                    }
                    return_design = no_temp;
                }
            }
        }
    }
    return result;
}
