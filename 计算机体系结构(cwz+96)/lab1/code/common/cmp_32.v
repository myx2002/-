`timescale 1ns / 1ps

module cmp_32(  input [31:0] a,
                input [31:0] b,
                input [2:0] ctrl,
                output c
    );
    parameter cmp_EQ  = 3'b001;
    parameter cmp_NE  = 3'b010;
    parameter cmp_LT  = 3'b011;
    parameter cmp_LTU = 3'b100;
    parameter cmp_GE  = 3'b101;
    parameter cmp_GEU = 3'b110;

    wire res_EQ  = a == b;//相等为1
    wire res_NE  = ~res_EQ;//不相等为1
    wire res_LT  = (a[31] & ~b[31]) || (~(a[31] ^ b[31]) && a < b);
    //a0b0,a<b--1
    //a1b0,a<b--1
    //a1b1,a<b--1
    //a0b1,a<b--0
    wire res_LTU = a < b;//无符号比较
    wire res_GE  = ~res_LT;//less than 反转
    wire res_GEU = ~res_LTU;

    //用ctrl判断三位输入
    //完成
    wire EQ  = ctrl == cmp_EQ ; 
    wire NE  = ctrl == cmp_NE ; 
    wire LT  = ctrl == cmp_LT ; 
    wire LTU = ctrl == cmp_LTU;
    wire GE  = ctrl == cmp_GE ; 
    wire GEU = ctrl == cmp_GEU;

    //小于设置为1--->根据cmp_ctrl来设置
    //cmp_ctrl来源于ctrlunit
    //根据EQ/NE.....判断需要控制的条件
    assign c = (EQ && res_EQ) || (NE && res_NE) || (LT && res_LT) || (LTU && res_LTU) || (GEU && res_GEU) || (GE && res_GE);          //to fill sth. in ()
    //只要c为1，就可以介导跳转
endmodule