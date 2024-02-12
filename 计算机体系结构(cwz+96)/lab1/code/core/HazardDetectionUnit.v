`timescale 1ps/1ps

module HazardDetectionUnit(
    input clk,//时钟 debugclk,关键的东西(每个阶段跳一下）
    //use表示是否使用
    input Branch_ID, rs1use_ID, rs2use_ID,
    //BID--是否跳转信号
    input[1:0] hazard_optype_ID,//hazard类型，介导什么填入ALU中
    input[4:0] rd_EXE, rd_MEM, rs1_ID, rs2_ID, rs2_EXE,
    //确定具体位置,前半理解没有问题
    output PC_EN_IF, reg_FD_EN, reg_FD_stall, reg_FD_flush,
        reg_DE_EN, reg_DE_flush, reg_EM_EN, reg_EM_flush, reg_MW_EN,
    output forward_ctrl_ls,
    output[1:0] forward_ctrl_A, forward_ctrl_B
);
   //ID阶段会向其中传入数据，类型并不是实时的
   reg [5:0]record = 6'b0;
    always@(posedge clk)begin
        record[5:2] <= record[3:0];
        record[1:0] <= hazard_optype_ID;
    end
    //hazard解决
    //exe hazard
    //不知道regwrite是否有效---可能与hazardtype有关
    //要求exe/mem的regwrite为1(计算结果)
    wire exehazardA = (rs1use_ID && (rs1_ID != 0) && (rs1_ID == rd_EXE));
    wire exehazardB = (rs2use_ID && (rs2_ID != 0) && (rs2_ID == rd_EXE));
    
    //mem hazard
    //类型为计算结果前递，,regwrite需要为1
    wire memhazardA = (rs1use_ID && (rs1_ID != 0) && (rs1_ID == rd_MEM));
    wire memhazardB = (rs2use_ID && (rs2_ID != 0) && (rs2_ID == rd_MEM));
    
    //L类型指令的特殊化，需要使用中断（mem write为1）
    //插入bubble再前递
    //if/if_id都要为0，中断需要控制0，nop_idex也需要设置-
    //需要一个检测机制
    wire data_stallA = (rs1use_ID && (rs1_ID != 0) && (rs1_ID == rd_EXE));
    wire data_stallB = (rs2use_ID && (rs2_ID != 0) && (rs2_ID == rd_EXE));
    wire data_stall = (data_stallA || data_stallB) && record[3:2] == 2'b10;
    
    //需要写入内存的寄存器数据可能被改写
    //考虑S指令的冲突
    wire Shazard = (rs2_EXE == rd_MEM) && (rs2_EXE != 0) && (record[5:4] == 2'b0);
    
    //处理跳转相关，先假设一直按照正常方向进行
    //JAL与B型指令
    wire control_stall = Branch_ID; 
    
    //选择mux接受器
    reg [1:0] fca;
    reg [1:0] fcb;
    reg [4:0] flagen;
    reg [4:0] flagcon;
    reg flagstall;
    reg flagflush;
    reg fcl;
    reg zero = 1'b0;
    
    assign PC_EN_IF = flagen[0] || flagcon[0];
    assign reg_FD_EN = flagen[1] || flagcon[1];
    assign reg_DE_EN = flagen[2] || flagcon[2];
    assign reg_EM_EN = flagen[3] || flagcon[3];
    assign reg_MW_EN = flagen[4] || flagcon[4];
    assign reg_FD_stall = flagstall;
    assign reg_FD_flush = flagflush;
    //有了优先判断机制，应该不需要继续向后移动了
    //且不再需要flush ID阶段后的寄存器
    assign reg_DE_flush = zero;
    assign reg_EM_flush = zero;
    always@(posedge clk or negedge clk)begin
        fcl <= 1'b0;
        fcb <= 2'b0;
        fca <= 2'b0;  
        flagcon <= 5'b11111;
        flagflush <= 1'b0;
        flagen <= 5'b11111;
        flagstall <= 1'b0;
        //对于插入bubble的处理
        if(data_stall && ~control_stall)begin
            flagen <= 5'b11110;
            flagcon <= 5'b11110;
            flagstall <= 1'b1;
        end
        //对于跳转的特殊处理
        else if(control_stall)begin
            flagcon <= 5'b11111;
            flagen <= 5'b11111;
            flagflush <= 1'b1;
        end
        //S类型特殊情况处理
        else if(Shazard)begin
            fcl <= 1'b1;
        end
        else begin 
            if(exehazardA && record[1:0] == 2'b01)begin
                fca <= 2'b01;
            end
            else if(memhazardA && record[1:0] == 2'b01)begin
            //10表示已到mem阶段的计算前递
                fca <= 2'b10;
            end
            else if(data_stallA && record[1:0] == 2'b10)begin
                fca <= 2'b11;
            end
               
            if(exehazardB && record[1:0] == 2'b01)begin
                fcb <= 2'b01;
            end
            else if(memhazardB && record[1:0] == 2'b01)begin
                fcb <= 2'b10;
            end
            else if(data_stallB && record[1:0] == 2'b10)begin
                fcb <= 2'b11;
            end
        end
    end
    
    //forward四选一*2
    assign forward_ctrl_A = fca;
    assign forward_ctrl_B = fcb;
    //write to reg的选择器
    assign forward_ctrl_ls = fcl;
    
    
    
    
    
    
    
    
    
endmodule