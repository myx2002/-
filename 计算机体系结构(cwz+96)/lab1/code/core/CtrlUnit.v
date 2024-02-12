`timescale 1ns / 1ps


module CtrlUnit(
    input[31:0] inst,
    input cmp_res,//是否跳转信号
    output Branch, ALUSrc_A, ALUSrc_B, DatatoReg, RegWrite, mem_w,
        MIO, rs1use, rs2use,
    output [1:0] hazard_optype,
    output [2:0] ImmSel, cmp_ctrl,
    output [3:0] ALUControl,
    output JALR
);

    wire[6:0] funct7 = inst[31:25];
    wire[2:0] funct3 = inst[14:12];
    wire[6:0] opcode = inst[6:0];

    wire Rop = opcode == 7'b0110011;
    wire Iop = opcode == 7'b0010011;
    wire Bop = opcode == 7'b1100011;
    wire Lop = opcode == 7'b0000011;
    wire Sop = opcode == 7'b0100011;

    wire funct7_0  = funct7 == 7'h0;
    wire funct7_32 = funct7 == 7'h20;

    wire funct3_0 = funct3 == 3'h0;
    wire funct3_1 = funct3 == 3'h1;
    wire funct3_2 = funct3 == 3'h2;
    wire funct3_3 = funct3 == 3'h3;
    wire funct3_4 = funct3 == 3'h4;
    wire funct3_5 = funct3 == 3'h5;
    wire funct3_6 = funct3 == 3'h6;
    wire funct3_7 = funct3 == 3'h7;

    wire ADD  = Rop & funct3_0 & funct7_0;
    wire SUB  = Rop & funct3_0 & funct7_32;
    wire SLL  = Rop & funct3_1 & funct7_0;
    wire SLT  = Rop & funct3_2 & funct7_0;
    wire SLTU = Rop & funct3_3 & funct7_0;
    wire XOR  = Rop & funct3_4 & funct7_0;
    wire SRL  = Rop & funct3_5 & funct7_0;
    wire SRA  = Rop & funct3_5 & funct7_32;
    wire OR   = Rop & funct3_6 & funct7_0;
    wire AND  = Rop & funct3_7 & funct7_0;

    wire ADDI  = Iop & funct3_0;	
    wire SLTI  = Iop & funct3_2;
    wire SLTIU = Iop & funct3_3;
    wire XORI  = Iop & funct3_4;
    wire ORI   = Iop & funct3_6;
    wire ANDI  = Iop & funct3_7;
    wire SLLI  = Iop & funct3_1 & funct7_0;
    wire SRLI  = Iop & funct3_5 & funct7_0;
    wire SRAI  = Iop & funct3_5 & funct7_32;

    wire BEQ = Bop & funct3_0;                            //to fill sth. in 
    wire BNE = Bop & funct3_1;                            //to fill sth. in 
    wire BLT = Bop & funct3_4;                            //to fill sth. in 
    wire BGE = Bop & funct3_5;                            //to fill sth. in 
    wire BLTU = Bop & funct3_6;                           //to fill sth. in 
    wire BGEU = Bop & funct3_7;                           //to fill sth. in 

    wire LB =  Lop & funct3_0;                            //to fill sth. in 
    wire LH =  Lop & funct3_1;                            //to fill sth. in 
    wire LW =  Lop & funct3_2;                            //to fill sth. in 
    wire LBU = Lop & funct3_4;                            //to fill sth. in 
    wire LHU = Lop & funct3_5;                            //to fill sth. in 

    wire SB = Sop & funct3_0;                             //to fill sth. in 
    wire SH = Sop & funct3_1;                             //to fill sth. in 
    wire SW = Sop & funct3_2;                             //to fill sth. in 

    wire LUI  = opcode == 7'b0110111;                          //to fill sth. in 
    wire AUIPC = opcode == 7'b0010111;                          //to fill sth. in 

    wire JAL  = opcode == 7'b1101111; 
    
    wire jalr = (opcode == 7'b1100111) && funct3_0;

    wire R_valid = AND | OR | ADD | XOR | SLL | SRL | SRA | SUB | SLT | SLTU;
    wire I_valid = ANDI | ORI | ADDI | XORI | SLLI | SRLI | SRAI | SLTI | SLTIU;
    wire B_valid = BEQ | BNE | BLT | BGE | BLTU | BGEU;
    wire L_valid = LW | LH | LB | LHU | LBU;
    wire S_valid = SW | SH | SB;
    
    //选择ALU结果还是PC原本数值
    //直接作加是JAL和B型指令
    assign JALR = jalr;
    //assign JALR = (opcode == 7'b1100111) && funct3_0;                        //to fill sth. in 

    //只关注指令是否会介导跳转
    //跳转是否允许cmp_res与是否需要跳转inst
    //jal不使用这个branch信号
    assign Branch = (cmp_res && B_valid) || JAL || jalr ;                       //to fill sth. in 

    parameter Imm_type_I = 3'b001;
    parameter Imm_type_B = 3'b010;
    parameter Imm_type_J = 3'b011;
    parameter Imm_type_S = 3'b100;
    parameter Imm_type_U = 3'b101;
    //指令类型
    assign ImmSel = {3{I_valid | jalr | L_valid}} & Imm_type_I |
                    {3{B_valid}}                  & Imm_type_B |
                    {3{JAL}}                      & Imm_type_J |
                    {3{S_valid}}                  & Imm_type_S |
                    {3{LUI | AUIPC}}              & Imm_type_U ;


    assign cmp_ctrl ={3{BEQ}} & 3'b001 |
                     {3{BNE}} & 3'b010 |
                     {3{BLT}} & 3'b011 |
                     {3{BGE}} & 3'b101 |
                     {3{BLTU}}& 3'b100 |
                     {3{BGEU}}& 3'b110;                        //to fill sth. in 
    //ALU操作数选择信号
    //0-rs1：R_valid、I_valid、L_valid、S_valid
    //1-PC：AUIPC、JALR
    //无所谓：LUI、JAL（PC寻址）、B_valid直接跳转
    //JALR指令在此体现，其他好像不需要判断信号
    assign ALUSrc_A = jalr || AUIPC || JAL;                         //to fill sth. in 

    //0-rs2：R_valid、
    //1-imm：I_valid、L_valid、S_valid、AUIPC、JALR
    //无所谓：B_valid、JAL、LUI
    assign ALUSrc_B = I_valid || L_valid || S_valid || AUIPC || LUI;                         //to fill sth. in 

    parameter ALU_ADD  = 4'b0001;
    parameter ALU_SUB  = 4'b0010;
    parameter ALU_AND  = 4'b0011;
    parameter ALU_OR   = 4'b0100;
    parameter ALU_XOR  = 4'b0101;
    parameter ALU_SLL  = 4'b0110;
    parameter ALU_SRL  = 4'b0111;
    parameter ALU_SLT  = 4'b1000;
    parameter ALU_SLTU = 4'b1001;
    parameter ALU_SRA  = 4'b1010;
    parameter ALU_Ap4  = 4'b1011;
    parameter ALU_Bout = 4'b1100;
    assign ALUControl = {4{ADD | ADDI | L_valid | S_valid | AUIPC}} & ALU_ADD  |
                        {4{SUB}}                                    & ALU_SUB  |
                        {4{AND | ANDI}}                             & ALU_AND  |
                        {4{OR | ORI}}                               & ALU_OR   |
                        {4{XOR | XORI}}                             & ALU_XOR  |
                        {4{SLL | SLLI}}                             & ALU_SLL  |
                        {4{SRL | SRLI}}                             & ALU_SRL  |
                        {4{SLT | SLTI}}                             & ALU_SLT  |
                        {4{SLTU | SLTIU}}                           & ALU_SLTU |
                        {4{SRA | SRAI}}                             & ALU_SRA  |
                        {4{JAL | jalr}}                             & ALU_Ap4  |
                        {4{LUI}}                                    & ALU_Bout ;
     //ALUControl 根据指令分解获得对映的ALU控制符号

    assign DatatoReg = L_valid;

    assign RegWrite = R_valid | I_valid | JAL | JALR | L_valid | LUI | AUIPC;

    assign mem_w = S_valid;

    assign MIO = L_valid | S_valid;
    
    //JAL（UJ）/JALR（I）/LUI（U）/AUIPC（U）
    //是否需要使用寄存器rs1
    assign rs1use = R_valid || I_valid || S_valid || B_valid || L_valid || jalr ;                        //to fill sth. in 
    //是否需要使用寄存器rs2
    assign rs2use = R_valid || S_valid || B_valid;                         //to fill sth. in 

    //00表示S型指令可能介导的问题
    //01表示一般的data hazard
    //10表示L型指令介导的data hazard
    //11表示control hazard（跳转相关）
    assign hazard_optype = {2{R_valid | I_valid}}             & 2'b01 |
                           {2{L_valid}}                       & 2'b10 |
                           {2{B_valid | JAL | jalr}}          & 2'b11 |
                           {2{S_valid}}                       & 2'b00 ;                   //to fill sth. in 





endmodule