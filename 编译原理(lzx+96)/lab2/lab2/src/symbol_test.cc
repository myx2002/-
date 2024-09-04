#include"symbol_test.hh"
struct symbol_table* table = new symbol_table; 
//函数返回
string func_return;
//是否需要函数返回
int return_val = 0;
//函数名
string func_name;
//数组容纳总大小
int total_count;
int now_count;
vector<int> level_info;

/*定义冲突仅需测试当前symbol table*/
/*使用是否定义需要测试全symbol table*/


/*返回 0 表示不存在问题
  返回 1 表示存在错误类型匹配*/
/*已考虑 func、define 情况 */
int symbol_table_test(node* temp)
{
    if(temp == NULL)
        return 0;
    int result = 0;
    //root点
    if(temp->type == "START")/*OK 没有右节点*/
    {
        struct symbol_table* temp_table = new symbol_table;
        temp_table->next = NULL;
        table->next = temp_table;
        build_base_func();

        if(temp->left != NULL)
        {
            result = symbol_table_test(temp->left);
            if(result == 1)
            {
                cout << "-----Failed!----" << endl;
                return result;
            }
        }
        else
        {
            cout<<"Error: line "<<temp->lineno<<": empty definition"<<endl;
            cout << "-----Failed!----" << endl;
            return 1;
        }

        //可能不止一个right节点
        if(temp->left->right == NULL){}
        else
        {
            node* temp_node = temp->left;
            while(temp_node->right != NULL)
            {
                temp_node = temp_node->right;
                result = symbol_table_test(temp_node);
                if(result == 1)
                {
                    cout << "-----Failed!----" << endl;
                    // cout << result << endl;
                    return result;
                }
            }
        }
        if (!result)
            cout << "----Successfully!----" << endl;
        else
            cout << "-----Failed!----" << endl;
        return result;
    }
    else if(temp->type == "FUNC_DEF")/*OK*/
    {
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        if(temp->left->type == "INT")
            temp_leaf->kind = symbol_leaf::INT;
        else if(temp->left->type == "VOID")
            temp_leaf->kind = symbol_leaf::VOID;

        /*仅测试当前symbol table不可拥有*/
        if(table->next->var_def[temp->left->right->name] == NULL)
            table->next->var_def[temp->left->right->name] = temp_leaf;
        else
        {
            cout<<"Error: line "<<temp->left->right->lineno<<": function redefinition"<<endl;
            return 1;
        }
        //记录function类型
        func_return = temp->left->type;
        //记录函数返回量
        return_val = 0;
        //记录一下函数名称(参数记录)
        func_name = temp->left->right->name; 
        result = test_func_def(temp->left->right->left);
        // cout<<result<<endl;
        if(result == 1)
            return result;
        return result;
    }
    else if(temp->type == "DECL")
    {
        result = test_decl_def(temp->left->left);
        if(result == 1)
            return result;
    }
    return result;
}

void build_base_func()
{
    //先建立int read()
    struct symbol_leaf* temp_leaf = new symbol_leaf;
    table->next->var_def["read"] = temp_leaf;
    temp_leaf->kind = symbol_leaf::INT;
    //再建立 void write(int)
    struct symbol_leaf* temp_leaf_t = new symbol_leaf;
    table->next->var_def["write"] = temp_leaf_t;
    temp_leaf_t->kind = symbol_leaf::VOID;
    temp_leaf_t->param.push_back("INT");
}

/*某个函数*/
//测试函数定义(接口是左括号)/*OK*/
int test_func_def(node* temp)
{
    int result = 0;
    //为function获得参数列表
    result = get_func_para(temp->left);
    if(result == 1)
        return result;
    //测试函数体
    if(temp->right->left->type == "BLOCK")
    {
        result = test_block(temp->right->left->left);
        //消除最临近的符号表
        table->next = table->next->next;
        // cout<<result<<endl;
        if(result == 1)
            return result;
        else if(return_val == 0 && func_return == "INT")
        {
            cout<<"Error: line "<<temp->lineno<<": missing return value"<<endl;
            return 1;
        }
    }
    return result;
}

/*函数参数列表*/
//函数建立参数列表（接口是para起始值（空/int））/*OK*/
int get_func_para(node* temp)
{
    int result = 0;
    struct symbol_table* temp_table = new symbol_table;
    temp_table->next = table->next;
    table->next = temp_table;
    if(temp == NULL)
        return 0;
    //其余情况建立详细的参数列表
    else
    {
        node* temp_node = temp;
        while(temp_node != NULL)
        {
            result = test_decl_def_func(temp_node);
            if(result ==  1)
                return result;
            temp_node = temp_node -> right;
        }
        return result;
    }
    return result;
}

/*某个block*/
//Block处理函数（接口是左大括号）
//test_block本身没有建立新表/*OK*/
int test_block(node* temp)
{
    // cout<<"test block"<<endl;
    int result = 0;
    if(temp->left == NULL)
        return 0;
    else
    {
        //DECL处理
        if(temp->left->type == "DECL")
        {
            node* temp_node = temp->left->left->left;
            while(temp_node != NULL)
            {
                result = test_decl_def(temp_node);
                if(result == 1)
                    return result;
                temp_node = temp_node->right;
            } 
        } 
        //STMT处理
        else if(temp->left->type == "STMT")
        {
            result = test_stmt_def(temp->left->left);
            if(result == 1)
                return result;
        }
            
        //可能不止一个right
        if(temp->left->right == NULL)
            return 0;
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
                        result = test_decl_def(temp_node_t);
                        if(result == 1)
                            return result;
                        temp_node_t = temp_node_t->right;
                    } 
                }
                else if(temp_node->type == "STMT")
                {
                    result = test_stmt_def(temp_node->left);
                    // cout<<result<<endl;
                    if(result == 1)
                        return result;
                } 
            }
        }
        return result;
    }
    return result;
}

/*某个声明*/
//声明处理函数（接口是ident）
int test_decl_def(node* temp)
{
    int result = 0;
    //不含assign一般变量
    if(temp->type == "IDENT" && (temp->left == NULL || temp->left->type != "ARRAY" ))
    {
        //判断当前symbol table是否有重复定义
        if(table->next->var_def[temp->name] == NULL)
        {
            struct symbol_leaf* temp_leaf = new symbol_leaf;
            temp_leaf->kind = symbol_leaf::INT;
            table->next->var_def[temp->name] = temp_leaf;
        }
        else
        {
            cout<<"Error: line "<<temp->lineno<<": variable redefinition"<<endl;
            return 1;
        }
        if(temp->left == NULL)
            return 0;
        else
        {
            result = test_exp(temp->left->right->left);
            if(result == 1)
                return result;
        }
    }
    //数组情况
    else if(temp->type == "IDENT" && temp->left->type == "ARRAY")
    {
        total_count = 1;
        now_count = 0;
        struct symbol_leaf* temp_leaf = new symbol_leaf;
        if(table->next->var_def[temp->name] == NULL)
        {
            temp_leaf->kind = symbol_leaf::ARRAY;
            node* temp_node = temp->left->left;
            //记录数组定义信息
            while(temp_node != NULL)
            {
                temp_leaf->array_size.push_back(temp_node->value);
                total_count *= temp_node->value;
                temp_node = temp_node->right;;
            }
            table->next->var_def[temp->name] = temp_leaf;
            level_info = temp_leaf->array_size;
        }
        else
        {
            cout<<"Error: line "<<temp->lineno<<": variable redefinition"<<endl;
            return 1;
        }
        //分类assign数组与非assign数组
        if(temp->left->right == NULL)
            return 0;
        else
        {
            //assign 类型判断(temp为ident)
            result = test_array(temp->left->right->left, 1);
            return result;
        }
    }
    return result;
}

//数组容纳总大小
// total_count;
// now_count;
// vector<int> level_info;

/*以assign最外层大括号为接口*/
int test_array(node* temp, int level)
{
    int result = 0;
    int layer = 0;
    int arr_top = (int)level_info.size();
    int temp_top = 1;
    int temp_top_next = 1;
    // cout<<"into array_test"<<endl;
    node* temp_node = temp->left;
    while(temp_node != NULL)
    {
        if(temp_node->type == "EXP")
        {
            now_count++;
            // cout<<"EXP: "<<now_count<<endl;
        } 
        else if(temp_node->type == "LBRACE")
        {
            layer = find_layer(temp_node);
            if(layer + level > arr_top)
            {
                cout<<"Error: line "<<temp_node->lineno<<": array type mismatch"<<endl;
                return 1;
            }
            // cout<<"layer: "<<layer<<endl;
            //需要判断是否合法
            temp_top = 1;
            //有指定数组内层向外走
            for(int arr_index = arr_top - layer; arr_index >= 0 ; arr_index--)
            {
                temp_top *= level_info[arr_index];
                // cout<<"temp_top: "<<temp_top<<endl;
                // cout<<"now_count: "<<now_count<<endl;
                if(now_count % temp_top != 0 && arr_index == arr_top - 1)
                {
                    cout<<"Error: line "<<temp_node->lineno<<": array type mismatch"<<endl;
                    return 1;
                }
                else if(now_count % temp_top == 0 && now_count != 0)
                {
                    temp_top_next = temp_top;
                    // cout<<"temp_top_next: "<<temp_top_next<<endl;
                }
                //处理直接嵌套左大括号的情况
                else if(now_count == 0 && arr_index == arr_top - 1)
                {
                    temp_top_next = temp_top;
                    // cout<<"temp_top_next(non): "<<temp_top_next<<endl;
                }
            }
            // cout<<temp_node->left->type <<endl;
            result = test_array(temp_node, level + 1);
            if(result == 1)
                return result;
            // cout<<"LBRACE: "<<now_count<<endl;
        }
        else if(temp_node->type == "RBRACE")
        {
            while(now_count % temp_top_next != 0)
                now_count++;
            // cout<<"RBRACE: "<<now_count<<endl;
        }

        if(now_count > total_count)
        {
            cout<<"Error: line "<<temp_node->lineno<<": too much integer"<<endl;
            return 1;
        }
        temp_node = temp_node->right;
    } 
    return result;
}

/*以当前大括号为接口，寻找括号深度*/
int find_layer(node* temp)
{
    int layer = 0;
    int layer_max = 0;
    node* temp_node = temp->left;
    while(temp_node != NULL)
    {
        if(temp_node->type == "LBRACE")
        {
            layer = find_layer(temp_node);
            if(layer > layer_max)
                layer_max = layer;
        }
        temp_node = temp_node->right;
    }
    return layer_max + 1;
}

/*某个stmt*/
//STMT处理函数（STMT节点的left为接口）
int test_stmt_def(node* temp)
{
    int result = 0;
    if(temp->type == "EMPTY")
        return 0;
    else if(temp->type == "BLOCK")
    {
        //开拓新symbol table
        struct symbol_table* temp_table = new symbol_table;
        temp_table->next = table->next;
        table->next = temp_table;
        result = test_block(temp->left);
        table->next = table->next->next;
        if(result == 1)
            return result;
    }
    else if(temp->type == "IF")
    {
        //cond判断
        //接口再说
        result = test_cond(temp->left->left);
        if(result == 1)
            return result;
        //if后stmt的判断
        result = test_stmt_def(temp->left->right->left->left);
        if(result == 1)
            return result;
        //判断是否存在else，若有else继续stmt判断
        if(temp->right != NULL && temp->right->type == "ELSE")
        {
            result = test_stmt_def(temp->right->left->left);
            if(result == 1)
                return result;
        }
        return result;
    }
    else if(temp->type == "WHILE")
    {
        //cond判断
        result = test_cond(temp->left->left);
        if(result == 1)
            return result;
        result = test_stmt_def(temp->left->right->left->left);
        return result;
    }
    else if(temp->type == "EXP")
    {
        //函数与表达式处理
        result = test_exp(temp->left, 0);
        return result;
    }
    else if(temp->type == "RETURN")
    {
        return_val = 1;
        //判断函数返回类型是否一致
        if(temp->left == NULL)
        {
            if(func_return == "INT")
            {
                cout<<"Error: line "<< temp->lineno <<": error return value"<<endl;
                return 1;
            }
            else
                return 0;
        }
        else
        {
            //EXP判断
            result = test_exp(temp->left->left);
            if(result == 1)
                return result;
            if(func_return == "VOID")
            {
                cout<<"Error: line "<< temp->lineno <<": error return value"<<endl;
                return 1;
            }
            else
                return 0;
        }
    }
    else
    {
        //cout<<"into IDENT"<<endl;
        //赋值的情况
        //一般变量情况
        if(temp->left->type == "INT")
        {
            struct symbol_table* temp_table = table->next;
            result = 1;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::INT)
                {
                    result = 0;
                    break;
                }
                temp_table = temp_table->next;
            }
            if(result == 1)
            {
                cout<<"Error: line "<<temp->lineno<<": no variable definition"<<endl;
                return result;
            }
        }
        //数组
        else if(temp->left->type == "ARRAY")
        {
            //cout<<"into ARRAY"<<endl;
            //记录使用数组维数            
            int array_size = 0;
            node* temp_node = temp->left->left;
            while(temp_node != NULL)
            {
                temp_node = temp_node ->right;
                array_size++;
            }
            //判断数组是否定义
            struct symbol_table* temp_table = table->next;
            result = 1;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::ARRAY
                    && (int)temp_table->var_def[temp->name]->array_size.size() == array_size)
                    {
                        result = 0;
                        break;
                    }
                temp_table = temp_table->next;
            }
            if(result == 1)
            {
                cout<<"Error: line "<<temp->lineno<<": no variable definition"<<endl;
                return result;
            }
        }
        result = test_exp(temp->left->right->right->left);
        return result;
    }
    return result;
}

/*condition条件判断*/
/*接口为逻辑运算符*/
int test_cond(node* temp)
{
    int result = 0;
    if(temp->type != "OR" || temp->type != "AND" || temp->type != "NOT" || temp->type != "LT" || 
    temp->type != "LE" || temp->type != "GT" || temp->type != "GE" || temp->type != "EQ" || temp->type != "NE")
    {
        result = test_exp(temp);
        if(result == 1)
            return result;
    }
    else
    {
        if(temp->left != NULL)
        {
            result = test_cond(temp->left);
            if(result == 1)
                return result;
        }

        if(temp->right != NULL)
        {
            result = test_cond(temp->right);
            if(result == 1)
                return result;
        }
    }
    return result;
} 

/*某个参数*/
//函数参数表建立过程(借口是参数起始（int/array）)/*OK*/
int test_decl_def_func(node* temp)
{
    int result = 0;
    if(temp->type == "INT")
    {
        if(table->next->var_def[temp->left->name] == NULL)
        {
            struct symbol_leaf* temp_leaf = new symbol_leaf;
            temp_leaf->kind = symbol_leaf::INT;
            table->next->var_def[temp->left->name] = temp_leaf;
            //记录于func的param列表中
            table->next->next->var_def[func_name]->param.push_back(temp->type);
        }
        else
        {
            cout<<"Error: line "<<temp->left->lineno<<": variable redefinition"<<endl;
            return 1;
        }
        return 0;
    }
    //数组情况
    else
    {
        node* temp_node = temp->left;
        while(temp_node -> left == NULL)
            temp_node = temp_node -> right;
        if(table->next->var_def[temp_node->left->name] == NULL)
        {
            struct symbol_leaf* temp_leaf = new symbol_leaf;
            temp_leaf->kind = symbol_leaf::ARRAY;
            table->next->var_def[temp_node->left->name] = temp_leaf;
            temp_node = temp->left;
            int array_size = 0;
            node* temp_node_t = temp_node;
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
            table->next->next->var_def[func_name]->param.push_back(temp->type);
            //记录于func的param列表中
            table->next->next->var_def[func_name]->param.push_back(to_string(array_size));
            //记录具体维数
            while(temp_node_t != NULL)
            {
                if(temp_node_t->type == "DEFAULT")
                    table->next->next->var_def[func_name]->param.push_back(to_string(0));
                else
                table->next->next->var_def[func_name]->param.push_back(to_string(temp_node_t->value));
                temp_node_t = temp_node_t->right;
            }
        }
        else
        {
            cout<<"Error: line "<<temp->left->lineno<<": variable redefinition"<<endl;
            return 1;
        }
        return 0;
    }
    return result;
}

/*某个exp表达式判断*/
/*接口为EXP的left节点(ADD/SUB/MUL/DIV/MOD)*/
int test_exp(node* temp, int return_index)
{
    int result = 0;
    if(temp->type == "FUNC_EXP")
    {
        //找func->param列表
        struct symbol_table* temp_table = table->next;
        result = 1;
        while(temp_table != NULL)
        {
            if(temp_table->var_def[temp->left->name] != NULL)
            {
                //cout<<"---find function---"<<endl;
                if(temp_table->var_def[temp->left->name]->kind == symbol_leaf::VOID && return_index == 1)
                {
                    cout<<"Error: line "<<temp->lineno<<": function return type mismatch"<<endl;
                    return 1;
                }
                result = 0;
                //指向exp
                node* temp_node = temp->left->left->left;
                struct symbol_leaf* temp_leaf = temp_table->var_def[temp->left->name];
                int index = 0;
                while(temp_node != NULL)
                {
                    //保证参数个数一致
                    // cout<<index<<" ";
                    // cout<<temp_leaf->param.size()<<endl;
                    if(index >= (int)temp_leaf->param.size())
                    {
                        cout<<"Error: line "<<temp->lineno<<": too many parameters"<<endl;
                        return 1;
                    }

                    //条件
                    if(temp_leaf->param[index] == "INT" && temp_node->type == "EXP" &&  temp_node->left->type != "IDENT")
                    {
                        result = test_exp(temp_node->left);
                        if(result == 1)
                            return result;
                    }
                    else if(temp_leaf->param[index] == "INT" && temp_node->type == "EXP" && 
                    temp_node->left->type == "IDENT")
                    {
                        result = test_exp(temp_node->left);
                        if(result == 1)
                            return result;
                    }
                    else if(temp_leaf->param[index] == "ARRAY" && temp_node->type == "EXP" &&  
                    temp_node->left->type == "IDENT" && temp_node->left->left->type == "ARRAY")
                    {
                        //获得当前维度
                        int array_size = 0;
                        //获得内层EXP
                        node* temp_node_t = temp_node->left->left->left;
                        while(temp_node_t != NULL)
                        {
                            temp_node_t = temp_node_t ->right;
                            array_size++;
                        }
                        // cout<<"---array size---"<<array_size<<endl;
                        struct symbol_table* temp_table_t = table->next;
                        result = 1;
                        //确定是否为对映维度array
                        while(temp_table_t != NULL)
                        {
                            if(temp_table_t->var_def[temp_node->left->name] != NULL && 
                            temp_table_t->var_def[temp_node->left->name]->kind == symbol_leaf::ARRAY &&
                            (int)temp_table_t->var_def[temp_node->left->name]->array_size.size() - array_size == 
                            stoi(temp_leaf->param[++index]))
                            {
                                //维度开始索引
                                int temp_index = index + 1;
                                vector<int> array_vec = temp_table_t->var_def[temp_node->left->name]->array_size;
                                int vector_index = array_size;
                                index = index + stoi(temp_leaf->param[index]) + 1;
                                // cout<<index<<endl;
                                // cout<<temp_index<<endl;
                                // cout<<array_size<<endl;
                                // cout<<temp_leaf->param.size()<<endl;
                                while(temp_index < index)
                                {
                                    if(stoi(temp_leaf->param[temp_index]) == 0)
                                    {
                                        temp_index++;
                                        vector_index++;
                                    }
                                    else if(stoi(temp_leaf->param[temp_index]) == array_vec[vector_index])
                                    {
                                        temp_index++;
                                        vector_index++;
                                    }
                                    else
                                    {
                                        cout<<"Error: line "<<temp->lineno<<": parameter type mismatch"<<endl;
                                        return 1;
                                    }
                                }
                                result = 0;
                                break;
                            }
                            temp_table_t = temp_table_t->next;
                        }
                        if(result == 1)
                        {
                            cout<<"Error: line "<<temp->lineno<<": no variable definition(array)"<<endl;
                            return result;
                        }
                    }
                    else
                    {
                        cout<<"Error: line "<<temp->lineno<<": parameter type mismatch"<<endl;
                        return 1;
                    }
                    temp_node = temp_node->right;
                    index++;
                }
                break;
            }
            temp_table = temp_table->next;
        }
        return result;
    }
    else
    {
        if(temp->type == "IDENT" && temp->left->type == "INT")
        {
            // cout<<temp->name<<endl;
            //从当前全判断
            struct symbol_table* temp_table = table->next;
            result = 1;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::INT)
                {
                    result = 0;
                    break;
                }
                temp_table = temp_table->next;
            }
            if(result == 1)
            {
                cout<<"Error: line "<<temp->lineno<<": no variable definition"<<endl;
                return result;
            }
        }
        else if(temp->type == "IDENT" && temp->left->type == "ARRAY")
        {
            //记录使用数组维数            
            int array_size = 0;
            node* temp_node = temp->left->left;
            while(temp_node != NULL)
            {
                temp_node = temp_node ->right;
                array_size++;
            }

            struct symbol_table* temp_table = table->next;
            result = 1;
            while(temp_table != NULL)
            {
                if(temp_table->var_def[temp->name] != NULL && temp_table->var_def[temp->name]->kind == symbol_leaf::ARRAY
                    && (int)temp_table->var_def[temp->name]->array_size.size() == array_size)
                {
                    result = 0;
                    break;
                }
                temp_table = temp_table->next;
            }
            if(result == 1)
            {
                cout<<"Error: line "<<temp->lineno<<": no variable definition"<<endl;
                return result;
            }
        }

        if(temp->left != NULL)
        {
            result = test_exp(temp->left);
            if(result == 1)
                return result;
        }
        //考虑只有一个right节点的情况
        if(temp->right != NULL)
        {
            result = test_exp(temp->right);
            if(result == 1)
                return result;
        }
         return result;
    }
    return result;
}