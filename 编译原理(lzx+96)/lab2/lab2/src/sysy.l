%option noinput
%option nounput
%option noyywrap
%option yylineno

%{
    #include "sysy.tab.hh"
    #include "tree.hh"
%}

digit [0-9]
blank [ \t\n\r]

%%

"if"        {printf("IF\n");
            yylval.tree = create_node("IF");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return IF;}
"else"      {printf("ELSE\n");
            yylval.tree = create_node("ELSE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return ELSE;}
"while"     {printf("WHILE\n");
            yylval.tree = create_node("WHILE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return WHILE;}
"return"    {printf("RETURN\n");
            yylval.tree = create_node("RETURN");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return RETURN;}
"int"       {printf("INT\n");return INT;}
"void"      {printf("VIOD\n");
            yylval.tree = create_node("VOID");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return VOID;}

"("         {printf("(\n");
            yylval.tree = create_node("LPAREN");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return LPAREN;}

")"         {printf(")\n");
            yylval.tree = create_node("RPAREN");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return RPAREN;}

";"         {printf(";\n");
            yylval.tree = create_node("SEMICOLON");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return SEMICOLON;}

","         {printf(",\n");return COMMA;}

"{"         {printf("{\n");
            yylval.tree = create_node("LBRACE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return LBRACE;}

"}"         {printf("}\n");
            yylval.tree = create_node("RBRACE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return RBRACE;}

"["         {printf("[\n");return LBRACKET;}
"]"         {printf("]\n");return RBRACKET;}
"+"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("ADD");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return ADD;}

"*"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("MUL");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return MUL;}

"/"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("DIV");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return DIV;}

"-"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("SUB");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return SUB;}

"%"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("MOD");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return MOD;}

">"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("GT");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return GT;}

"<"         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("LT");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return LT;}

">="        {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("GE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return GE;}

"<="        {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("LE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return LE;}

"=="        {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("EQ");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return EQ;}

"!="        {printf("OP(%s)\n", yytext);
            yylval.tree = create_node("NE");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return NE;}

"!"         {printf("OP(%s)\n", yytext);
            yylval.tree = create_node("NOT");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return NOT;}
"||"        {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("OR");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return OR;}

"&&"        {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("AND");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return AND;}

"="         {printf("OP(%s)\n", yytext); 
            yylval.tree = create_node("ASSIGN");
            yylval.tree->lineno = yylineno;
            yylval.tree->kind = NOE_k;
            return ASSIGN;}

"0"[0-7]+               {printf("NUM(%s)\n", yytext);
                        yylval.tree = create_node("NUM");
                        yylval.tree->lineno = yylineno;
                        int number = base_to_dec(atoi(yytext), 8);
                        yylval.tree->value = number;
                        yylval.tree->kind = NUM_k;
                        return NUM;}

"0"[xX][0-9a-fA-F]+        {printf("NUM(%s)\n", yytext);
                        yylval.tree = create_node("NUM");
                        yylval.tree->lineno = yylineno;
                        int number = strtol(yytext, NULL, 16);
                        yylval.tree->value = number;
                        yylval.tree->kind = NUM_k;
                        return NUM;}

{digit}+                {printf("NUM(%s)\n", yytext);
                        yylval.tree = create_node("NUM");
                        yylval.tree->lineno = yylineno;
                        yylval.tree->value = atoi(yytext);
                        yylval.tree->kind = NUM_k;
                        return NUM;}

[a-zA-Z_][a-zA-Z0-9_]*  {printf("IDENT(%s)\n", yytext);
                        yylval.tree = create_node("IDENT");
                        yylval.tree->name = yytext;
                        yylval.tree->lineno = yylineno;
                        return IDENT;}

{blank}+                {}
"//".*                  {}
"/*"[^*]*"*"+([^*/][^*]*"*"+)*"/" {}
.                       { printf("LEX:ERROR(%s)\n", yytext); }
%%
