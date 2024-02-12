`timescale 1ns / 1ps

module ExceptionUnit(
    input clk, rst, //cpu clk
    //指令处理相关信号
    input csr_rw_in, //指令是否介导寄存器的写读
    input[1:0] csr_wsc_mode_in, //当前模式，csr寄存器的读指令的12和13位-->01w,10s,11c
    //指令好像是把csr寄存器中的值取出，扩展至32bit然后放入rd中，再把rs中的值放入csr寄存器中
    //csrrs x5, mie, x6          x5 = mie; mie = x6
    //伪指令含有 I 即将原来rs的位置改为5bit无符号立即数
    input csr_w_imm_mux, //是否含有立即数信号
    input[11:0] csr_rw_addr_in,//指令的20-31-->需要读写的csr寄存器地址
    input[31:0] csr_w_data_reg,//需要写入csr寄存器的数据
    input[4:0] csr_w_data_imm,//需要写入的立即数，或者目标寄存器？
    
    output [31:0] csr_r_data_out,//需要写入寄存器，存储在csr中的数据
    
    //中断/异常处理相关信号
    input interrupt, //暂无来源，for future use
    input illegal_inst,//不合法指令信号，
    //~isFlushed_WB & exp_vector_WB[3] 
    //WB是否被flush，没有被flush;ill-inst检测为1
    input l_access_fault,//L型指令越界读取
    input s_access_fault,//S型指令越界存储
    input ecall_m,//flush为0 + ecall信号触发

    input mret,
    // inst == 32'b0011000_00010_00000_000_00000_1110011 判断是否使用mret伪指令信号
    input[31:0] epc_cur,//当前需要WB阶段的PC
    input[31:0] epc_next,//没有被flush的倒数第一个阶段的PC
    output  [31:0] PC_redirect,//确认需要写入IF阶段的正确PC
    output  redirect_mux,//下一指令PC选择信号

    output  reg_FD_flush, reg_DE_flush, reg_EM_flush, reg_MW_flush, //是否需要flush各阶段寄存器信号
    output  RegWrite_cancel//拒绝写入寄存器组信号
);
    reg[31:0] CSR [0:15];
    
    wire[11:0] csr_raddr;//csr读出地址
    assign csr_raddr = csr_rw_addr_in;
    wire[11:0] csr_waddr;//csr写入地址
    assign csr_waddr = csr_rw_addr_in;
    wire[31:0] csr_wdata;//csr写入数据
    assign csr_wdata = csr_w_imm_mux ? {27'b0,csr_w_data_imm}: csr_w_data_reg;
    wire csr_w;//csr是否写入信号
    assign csr_w = csr_rw_in;
    wire [1:0] csr_wsc;//csr写入模式信号
    assign csr_wsc = csr_wsc_mode_in;
    
    wire raddr_valid;//csr是否读出
    assign raddr_valid = csr_raddr[11:7] == 5'h6 && csr_raddr[5:3] == 3'h0;
    wire[3:0] raddr_map;//csr读出目标地址
    assign raddr_map = (csr_raddr[6] << 3) + csr_raddr[2:0];//寄存器地址
    wire waddr_valid;//csr是否写入
    assign waddr_valid = csr_waddr[11:7] == 5'h6 && csr_waddr[5:3] == 3'h0;
    wire[3:0] waddr_map;//csr写入目标地址
    assign waddr_map = (csr_waddr[6] << 3) + csr_waddr[2:0];
    assign csr_r_data_out =CSR[raddr_map];//csr读出写入目标寄存器数据
    
    reg[31:0] mstatus;
    //仅rdata为模块的直接输出
     always@(posedge clk or posedge rst) begin
        mstatus = CSR[0];
        if(rst) begin
            //初始化scr寄存器组  
			CSR[0] <= 32'h88;//mstatus
			CSR[1] <= 0;
			CSR[2] <= 0;
			CSR[3] <= 0;
			CSR[4] <= 32'hfff;
			CSR[5] <= 0;      //mtvec
			CSR[6] <= 0;
			CSR[7] <= 0;
			CSR[8] <= 0;
			CSR[9] <= 0; //mepc
			CSR[10] <= 0;//mcause
			CSR[11] <= 0;//mtval
			CSR[12] <= 0;
			CSR[13] <= 0;
			CSR[14] <= 0;
			CSR[15] <= 0;
		end
        else if(csr_w) begin
            case(csr_wsc)
                2'b01: CSR[waddr_map] = csr_wdata;
                2'b10: CSR[waddr_map] = CSR[waddr_map] | csr_wdata;
                2'b11: CSR[waddr_map] = CSR[waddr_map] & ~csr_wdata;
                default: CSR[waddr_map] = csr_wdata;
            endcase            
        end
        else if(mret)begin
            //重置一下ststus，可能需要清零寄存器
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b1,mstatus[2:0]};
        end
        else if(illegal_inst)begin
            CSR[9] = epc_cur;
            CSR[10] = {1'b0,31'b000_0000_0000_0000_0000_0000_0000_0010};
            CSR[11] = epc_cur;
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b0,mstatus[2:0]};
        end
        else if(l_access_fault)begin
            CSR[9] = epc_cur;
            CSR[10] = {1'b0,31'b000_0000_0000_0000_0000_0000_0000_0101};
            CSR[11] = epc_cur;
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b0,mstatus[2:0]};
        end
        else if(s_access_fault)begin
            CSR[9] = epc_cur;
            CSR[10] = {1'b0,31'b000_0000_0000_0000_0000_0000_0000_0111};
            CSR[11] = epc_cur;
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b0,mstatus[2:0]};
        end
        else if(ecall_m)begin
            CSR[9] = epc_cur;
            CSR[10] = {1'b0,31'b000_0000_0000_0000_0000_0000_0000_1011};
            CSR[11] = epc_cur;
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b0,mstatus[2:0]};
        end
        else if(interrupt && mstatus[3])begin
            CSR[9] = epc_next - 4'b0100;//可能是epc_next
            CSR[10] = {1'b1,31'b000_0000_0000_0000_0000_0000_0000_1011};
            CSR[11] = epc_cur;
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b0,mstatus[2:0]};
        end
    end
    //有 mstatus、clk、rst
    //先考虑异常
    wire flag = mret || illegal_inst || l_access_fault || s_access_fault || ecall_m || (interrupt && mstatus[3]);
    wire flag1 = illegal_inst || l_access_fault || s_access_fault || ecall_m || (interrupt && mstatus[3]);
    assign reg_FD_flush = flag ? 1'b1 : 1'b0;//fd寄存器刷新信号
    assign reg_DE_flush = flag ? 1'b1 : 1'b0;//de寄存器刷新信号
    assign reg_EM_flush = flag ? 1'b1 : 1'b0;//em寄存器刷新信号
    assign reg_MW_flush = flag ? 1'b1 : 1'b0; //mw寄存器刷新信号
    assign redirect_mux = flag ? 1'b1 : 1'b0;//选择一般PC运行/trap跳转/mepc跳回
    assign PC_redirect = mret ? CSR[9] : CSR[5]; //
    assign RegWrite_cancel = l_access_fault ? 1'b1 : 1'b0;

endmodule