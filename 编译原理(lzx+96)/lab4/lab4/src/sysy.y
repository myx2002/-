%{
    #include "tree.hh"
    void yyerror(const char *s);
    extern int yylex(void);
%}

%union
{
    struct node* tree;
}

%token <tree> INT "int"
%token <tree> SEMICOLON ";"
%token <tree> COMMA ","
%token <tree> IDENT
%token <tree> LBRACKET "["
%token <tree> RBRACKET "]"
%token <tree> NUM
%token <tree> ASSIGN "="
%token <tree> LBRACE "{"
%token <tree> RBRACE "}"
%token <tree> ADD "+"
%token <tree> SUB "-"
%token <tree> MUL "*"
%token <tree> DIV "/"
%token <tree> MOD "%"
%token <tree> LPAREN "("
%token <tree> RPAREN ")"
%token <tree> NOT "!"
%token <tree> OR "||"
%token <tree> AND "&&"
%token <tree> GT ">"
%token <tree> LT "<"
%token <tree> GE ">="
%token <tree> LE "<="
%token <tree> EQ "=="
%token <tree> NE "!="
%token <tree> WHILE "while"
%token <tree> IF "if"
%token <tree> RETURN "return"
%token <tree> ELSE "else"
%token <tree> VOID "void"

%type <tree> INT_ID VarDecl Decl CompUnit_free CompUnit VarDef_s  Calc InitVal Exp AddExp MulExp
%type <tree> UnaryExp PrimaryExp Number VarDef_arr VarDef InitVal_s UnaryOp Exp_c Exp_s
%type <tree> LVal FuncDef Block BlockItem_s BlockItem Stmt Cond FuncFParam_s FuncFParam RelExp EqExp LAndExp LOrExp

%start Calc

/*优先级从小到大排列*/ 
%left ADD SUB
%left MUL DIV MOD
%right NOT

%%
/*左节点下一层，右节点同一层*/
/*array与assign并列关系
    array的具体信息缩进
    assign的具体信息未缩进（注意）*/
/*采用    
          运算符
      参数1    参数2    
                    的记录形式*/
/*定义时，先类型再变量名；使用时，先变量名再类型*/
/*八进制/十六进制直接转化为十进制记录(均记录为 NUM_k kind)*/

/*node(start) OK*/
Calc          : CompUnit{printf("-----Successfully!-----\n\n");
                        $$ = create_node("START");
                        $$->left = $1;
                        $$->lineno = $1->lineno;
                        root = $$;
                        };

/*node(comunit_free) OK*/
CompUnit      : CompUnit_free{$$ = create_node();
                            $$ = $1;
                            }             
                | CompUnit CompUnit_free{$$ = create_node();
                                        def_right($1, $2);
                                        $$ = $1;
                                        };

/*node(comunit_free)*/
CompUnit_free : Decl{$$ = create_node();
                    $$ = $1;
                    } 
                | FuncDef{$$ = create_node();
                    $$ = $1;
                    } ;

/*node(decl) OK*/
Decl          : VarDecl{$$ = create_node();
                        $$=$1;
                        $$->type = "DECL";
                        };

/*node(int) -- node OK*/
INT_ID        : "int" IDENT{//---出口不唯一
                            $$ = create_node("INT");
                            $2->kind = STR_k;
                            $$->left = $2;
                            $$->lineno = $2->lineno;
                            $$->kind = NOE_k;
                            };

/*node(vardecl) --- node(int)  --- node(ident) --->node(ident) OK*/
VarDecl       : INT_ID VarDef_s ";"{$$ = create_node("VARDECL");
                                    $$->left = $1;
                                    $$->lineno = $1->lineno;
                                    if($2->type == "ASSIGN_TEMP"){
                                        $1->left->left = $2->left;
                                        if($2->left->left != NULL){
                                            $1->left->right = $2->left->left;
                                            $2->left->left = NULL;
                                        }
                                        
                                    }
                                    else if($2->type == "ARRAY"){
                                        $1->left->left = $2;
                                        if($2->left->left != NULL){
                                            $1->left->right = $2->left->left;
                                            $2->left->left = NULL;
                                        }
                                    }
                                    else
                                        $1->left->right = $2->left;
                                    }
                |INT_ID ";"{$$ = create_node("INT");
                            $$->left =  $1;
                            $$->lineno = $1->lineno;
                            $$->type = "VARDECL";
                            };  
/*OK*/
VarDef_s      : /*node(array) --- node(number) ---> node(number)*/
                VarDef{$$ = create_node();
                    $$ = $1;
                    } 
                /*node(int) --- node*/
                | VarDef_s "," IDENT{$$ = create_node();
                                    $3->kind = STR_k;
                                    if($1->type == "ASSIGN_TEMP" && $1->left->left == NULL){
                                        $1->left->left = $3;
                                    }
                                    else if($1->type == "ARRAY" && $1->left->left == NULL){
                                        $1->left->left = $3;
                                    }
                                    else{
                                        node* temp;
                                        if($1->left->left != NULL && $1->type != "TEMP")
                                            temp = $1->left->left;
                                        else
                                            temp = $1->left;
                                        while(temp->right != NULL)
                                            temp = temp->right;
                                        temp->right = $3;
                                    }
                                    $$ = $1;
                                    } 
                | VarDef_s "," IDENT VarDef{$$ = create_node();
                                            $3->kind = STR_k;
                                            if($1->type == "ASSIGN_TEMP" && $1->left->left == NULL){
                                                $1->left->left = $3;
                                            }
                                            else if($1->type == "ARRAY" && $1->left->left == NULL){
                                                $1->left->left = $3;
                                            }
                                            else{
                                                node* temp;
                                                if($1->left->left != NULL && $1->type != "TEMP")
                                                    temp = $1->left->left;
                                                else
                                                    temp = $1->left;
                                                while(temp->right != NULL)
                                                    temp = temp->right;
                                                temp->right = $3;
                                            }
                                            $3->left = $4;
                                            $$ = $1;
                                            } 
                /*node(assign_temp) --- node(assign) ---> node(num)*/
                /*                                   --- node(ident/assign)*/
                | VarDef_s "," IDENT "=" InitVal{$$ = create_node();
                                                $3->kind = STR_k;
                                                if(($1->type == "ASSIGN_TEMP" || $1->type == "ARRAY") 
                                                    && $1->left->left == NULL){
                                                    $1->left->left = $3;
                                                }
                                                else{
                                                    node* temp;
                                                    if($1->left->left != NULL && $1->type != "TEMP")
                                                        temp = $1->left->left;
                                                    else
                                                        temp = $1->left;
                                                    while(temp->right != NULL)
                                                        temp = temp->right;
                                                    temp->right = $3;
                                                }
                                                $3->left = $4;
                                                $4->right = $5;
                                                $$ = $1;
                                                }
                /*node(int) --- node(assign)*/
                | "," IDENT "=" InitVal{$$=create_node("TEMP");
                                        $2->kind = STR_k;
                                        $2->left = $3;
                                        $3->right = $4;
                                        $$->left = $2;
                                        $$->lineno = $2->lineno;
                                        }
                /*node(int) --- node*/
                | "," IDENT{$$ = create_node("INT");
                            $2->kind = STR_k;
                            $$->left = $2;
                            $$->lineno = $2->lineno;
                            $$->kind = NOE_k;
                            }
                | "," IDENT VarDef{$$ = create_node("TEMP");
                                $2->kind = STR_k;
                                $$->left = $2;
                                $$->lineno = $2->lineno;
                                $2->left = $3;
                                $$->kind = NOE_k;
                                }
                /*node(assign_temp) --- node(assign) ---> node(num)*/
                | "=" InitVal{$$ = create_node("ASSIGN_TEMP");
                            $1->right = $2;
                            $$->left = $1;
                            $$->lineno = $1->lineno;
                            };
/*node(array) --- node(number) ---> node(number)*/
/*            ---> node(assign) OK*/
VarDef        :  VarDef_arr{$$ = create_node("ARRAY");
                            $$ ->left =  $1;
                            $$->lineno = $1->lineno;
                            }
                | VarDef_arr "=" InitVal{$$ = create_node("ARRAY");
                                        $$->left = $1;
                                        $$->lineno = $1->lineno;
                                        $$->right = $2;
                                        $2->left = $3;    
                                        };

/*node(number) ---> node(numebr) OK*/
VarDef_arr    : "[" NUM "]"{/*暂仅考虑一个的情况*/
                            $$ = create_node();
                            $$ = $2;
                            }
                | VarDef_arr "[" NUM "]"{$$ = create_node();
                                        def_right($1, $3);
                                        $$ = $1;
                                        };
/*OK*/
InitVal       : "{" InitVal_s "}"{$$ = create_node();
                                $$ = $1;
                                $1->left = $2;
                                def_right($1, $3);
                                }
                /*直接返回*/ 
                | Exp{$$ = create_node("EXP");
                    $$ = $1;
                    }
                /*node("(") ---> node(")")*/
                |"{" "}"{$$ = create_node();
                        $1->right = $2;
                        $$ = $1;
                        };
/*OK*/
InitVal_s     : InitVal_s "," InitVal{$$ = create_node();
                                    def_right($1, $3);
                                    $$ = $1;
                                    }
                | InitVal{$$ = create_node();
                        $$ = $1;
                        };
/*OK*/
FuncDef       : "void" IDENT "(" FuncFParam_s ")" Block{$$ = create_node("FUNC_DEF");
                                                    $$->left = $1;
                                                    $$->lineno = $1->lineno;
                                                    $1->right = $2;
                                                    $2->left = $3;
                                                    $2->kind = STR_k;
                                                    $3->right = $5;
                                                    $3->left = $4;
                                                    $5->left = $6;
                                                    }
                |"void" IDENT "(" ")" Block{$$ = create_node("FUNC_DEF");
                                           $$->left = $1;
                                           $$->lineno = $1->lineno;
                                           $1->right = $2;
                                           $2->left = $3;
                                           $2->kind = STR_k;
                                           $3->right = $4;
                                           $4->left = $5;
                                            }
                | INT_ID "(" FuncFParam_s ")" Block{$$ = create_node("FUNC_DEF");
                                                    $$->left = $1;
                                                    $$->lineno = $1->lineno;
                                                    $1->right = $1->left;
                                                    $1->left = NULL;
                                                    $1->right->left = $2;
                                                    $2->left = $3;
                                                    $2->right = $4;
                                                    $4->left = $5;
                                                    }
                | INT_ID "(" ")" Block{$$ = create_node("FUNC_DEF");
                                        node* temp = create_node("INT");
                                        $$->left = temp;
                                        $$->lineno = $1->lineno;
                                        temp->lineno = $1->lineno;
                                        temp->right = $1->left;
                                        temp->right->left = $2;
                                        $2->right = $3;
                                        $3->left = $4;
                                        };
/*OK*/
FuncFParam_s  : FuncFParam{$$ = create_node();
                            $$ = $1;
                            } 
                | FuncFParam_s "," FuncFParam{$$ = create_node();
                                            def_right($1, $3);
                                            $$ = $1;
                                            };
/*OK*/
FuncFParam    :  INT_ID{$$ = create_node();
                        $$ = $1;
                        } 
                | INT_ID "[" "]"{$$ = create_node("ARRAY");
                                node* temp = create_node("DEFAULT");
                                $$->left = temp;
                                temp->left = $1->left;
                                $$->lineno = $1->lineno;
                                temp->lineno = $1->lineno;
                                }
                | INT_ID "[" "]" VarDef_arr{$$ = create_node("ARRAY");
                                            node* temp = create_node("DEFAULT");
                                            $$->left = temp;
                                            $$->lineno = $1->lineno;
                                            temp->lineno = $1->lineno;
                                            temp->right = $4;
                                            node* temp_pos = $4;
                                            while(temp_pos->right != NULL)
                                                temp_pos = temp_pos->right;
                                            temp_pos->left = $1->left;
                                            };
/*OK*/
/*函数体出现{}的次数等于block的次数*/
Block         : "{" "}"{$$ = create_node("BLOCK");
                        $1->right = $2;
                        $$->left = $1;
                        $$->lineno = $1->lineno;}
                |"{" BlockItem_s "}"{$$ = create_node("BLOCK");
                                    $1->right = $3;
                                    $$->left = $1;
                                    $$->lineno = $1->lineno;
                                    $1->left = $2;
                                    };
/*OK*/
BlockItem_s   : BlockItem{$$ = create_node();
                            $$ = $1;
                            }
                | BlockItem_s BlockItem{$$ = create_node();
                                        def_right($1, $2);
                                        $$ = $1;
                                        };
/*OK*/
/*在define语句中，先出现类型，再使用left node给出变量名*/
BlockItem     : Decl{$$ = create_node();
                    $$ = $1;
                    } 
                | Stmt{$$=create_node();
                    $$ = $1;
                    };
/*OK*/
/*STMT中 变量的left node一定为类型*/
Stmt          : LVal "=" Exp ";"{$$ = create_node("STMT");
                                $$->left = $1;
                                $$->lineno = $1->lineno;
                                def_right($1->left, $2);
                                $2->right = $3;
                                }
                | ";"{$$ = create_node("STMT");
                        node* temp = create_node("EMPTY");
                        $$->left = temp;
                        temp->lineno = $1->lineno;
                        $$->lineno = $1->lineno;
                    }
                | Exp ";"{$$ = create_node("STMT");
                        $$->left = $1;
                        $$->lineno = $1->lineno;
                        }
                | Block{$$ = create_node("STMT");
                        $$->left = $1;  
                        $$->lineno = $1->lineno;  
                        }
                | "if" "(" Cond ")" Stmt{$$ = create_node("STMT");
                                        $$->left = $1;
                                        $$->lineno = $1->lineno;
                                        $1->left = $2;
                                        $2->right = $4;
                                        $2->left = $3;
                                        $4->left = $5;
                                        } 
                | "if" "(" Cond ")" Stmt "else" Stmt{$$ = create_node("STMT");
                                                    $$->left = $1;
                                                    $$->lineno = $1->lineno;
                                                    $1->left = $2;
                                                    $2->right = $4;
                                                    $2->left = $3;
                                                    $4->left = $5;
                                                    $1->right = $6;
                                                    $6->left = $7;
                                                    }
                | "while" "(" Cond ")" Stmt{$$ = create_node("STMT");
                                            $$->left = $1;
                                            $$->lineno = $1->lineno;
                                            $1->left = $2;
                                            $2->right = $4;
                                            $2->left = $3;
                                            $4->left = $5;
                                            }
                | "return" ";"{$$ = create_node("STMT");
                                $$->left = $1;
                                $$->lineno = $1->lineno;
                                }
                | "return" Exp ";"{$$ = create_node("STMT");
                                    $$->left = $1;
                                    $$->lineno = $1->lineno;
                                    $1->left = $2;
                                    };

/*直接返回 OK*/
Exp           : AddExp{$$ = create_node("EXP");
                    $$->left = $1;
                    $$->lineno = $1->lineno;
                    };
/*OK*/
Cond          : LOrExp{$$ = create_node();
                        $$ = $1;
                        };

/*OK*/
/*node(IDENT) --- node(INT)/node(ARRAY)*/
LVal          : IDENT Exp_s{$$ = create_node();
                            $$ = $1;
                            $1->kind = STR_k;
                            $1->left = create_node("ARRAY");
                            $1->left->left = $2;
                            }
                | IDENT{$$ = create_node();
                        $1->kind = STR_k;
                        $$ = $1;
                        $1->left = create_node("INT");
                        };
/*OK*/
Exp_s         :"[" Exp "]"{$$ = create_node();
                        $$ = $2;} 
                | Exp_s "[" Exp "]"{$$ = create_node();
                                    def_right($1, $3);
                                    $$ = $1;
                                    };
/*OK*/
PrimaryExp    : "(" Exp ")"{/**/
                            $$ = create_node();
                            $1->left = $2;
                            def_right($1, $3);
                            $$ = $1;
                            } 
                | LVal{$$ = create_node();
                    $$ = $1;
                    } 
                | Number{/*node(num)*/
                        $$ = create_node();
                        $$ = $1;
                        };
/*OK*/
Number        : NUM{$$ = create_node();
                    $$ = $1;
                    };
/*OK*/
/*函数调用的参数先给出变量名，再使用left node记录其类型*/
UnaryExp      : PrimaryExp{$$ = create_node();
                        $$ = $1;
                        }
                /*括号缩进*/ 
                | IDENT "(" ")"{$$ = create_node("FUNC_EXP");
                                $$->left = $1;
                                $$->lineno = $1->lineno;
                                $1->kind = STR_k;
                                $1->left = $2;
                                $2->right = $3;
                                $2->type = "LPAREN_EXP";
                                $3->type = "RPAREN_EXP";
                                } 
                | IDENT "(" Exp Exp_c{$$ = create_node("FUNC_EXP");
                                    $$->left = $1;
                                    $$->lineno = $1->lineno;
                                    $1->kind = STR_k;
                                    $1->left = $2;
                                    $2->left = $3;
                                    $2->type = "LPAREN_EXP";
                                    if($4->type == "RPAREN_EXP"){
                                        $2->right = $4;
                                    }
                                    else{
                                        $2->right = create_node("RPAREN_EXP");
                                        $2->right->kind = NOE_k;
                                        $2->right->lineno = $2->lineno;
                                        def_right($3, $4);
                                    }
                                    } 
                | UnaryOp UnaryExp{$$ = create_node();
                                $$ = $1;
                                $1->left = $2;
                                };
/*OK*/
UnaryOp       : "+"{$$=create_node();
                    $1->type = "POSITIVE";
                    $$ = $1;
                    } 
                | "-"{$$=create_node();
                    $1->type = "NEGATIVE";
                    $$ = $1;
                    }  
                | "!"{$$=create_node();
                    $$ = $1;
                    } ;
/*OK*/
Exp_c         : ")" {$$ = create_node();
                    $$ = $1;
                    $$->type = "RPAREN_EXP";
                    }
                |  "," Exp Exp_c {$$ = create_node();
                                $$ = $2;
                                if($3->type == "RPAREN_EXP");
                                else
                                    def_right($2, $3);
                                };
/*OK*/
MulExp        : UnaryExp{$$ = create_node();
                        $$ = $1;
                        } 
                | MulExp "*" UnaryExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    }
                | MulExp "/" UnaryExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    }
                | MulExp "%" UnaryExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    }
/*OK*/
AddExp        : MulExp{$$ = create_node();
                    $$ = $1;
                    } 
                | AddExp "+" MulExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    }
                | AddExp "-" MulExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    };
/*OK*/
RelExp        : AddExp{$$ = create_node();
                        $$ = $1;
                        } 
                | RelExp "<" AddExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);} 
                | RelExp ">" AddExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);}
                | RelExp "<=" AddExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);}
                | RelExp ">=" AddExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);};
/*OK*/
EqExp         : RelExp{$$ = create_node();
                        $$ = $1;
                        } 
                | EqExp "==" RelExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    }
                | EqExp "!=" RelExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);      
                                    };
/*OK*/
LAndExp       : EqExp{$$ = create_node();
                        $$ = $1;
                        } 
                | LAndExp "&&" EqExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    };
/*OK*/
LOrExp        : LAndExp{$$ = create_node();
                        $$ = $1;
                        } 
                | LOrExp "||" LAndExp{$$ = create_node();
                                    $$ = $2;
                                    $2->left = $1;
                                    def_right($1, $3);
                                    };
%%

void yyerror(const char *s) 
{
    printf("Error: %s\n", s);
}
