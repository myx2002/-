`timescale 1ns / 1ps

module FU_jump(
	input clk, EN, JALR,
	input[2:0] cmp_ctrl,
	input[31:0] rs1_data, rs2_data, imm, PC,
	output[31:0] PC_jump, PC_wb,
	output cmp_res, finish
);

    reg state;
    assign finish = state == 1'b1;
	initial begin
        state = 0;
    end

	reg JALR_reg;
	reg[2:0] cmp_ctrl_reg;
	reg[31:0] rs1_data_reg, rs2_data_reg, imm_reg, PC_reg;

    always@(posedge clk) begin
        if(EN & ~state) begin // state == 0
            JALR_reg = JALR;
            cmp_ctrl_reg = cmp_ctrl;
	        rs1_data_reg = rs1_data;
	        rs2_data_reg = rs2_data; 
	        imm_reg = imm; 
	        PC_reg = PC;
            state <= 1;
        end
        else begin
            JALR_reg = 0;
            cmp_ctrl_reg = 0;
	        rs1_data_reg = 0;
	        rs2_data_reg = 0; 
	        imm_reg = 0; 
	        PC_reg = 0;
            state <= 0;
        end
    end
    
    reg [31:0] PC_result;
    reg flag;
    always@(*)begin
        if(JALR && EN)begin
            PC_result = rs1_data_reg + imm_reg;
        end
        else if(EN) begin
            case(cmp_ctrl_reg)
                3'b000: begin
                     PC_result = PC_reg + imm_reg;
                end
                3'b001: begin 
                    PC_result = (rs1_data_reg == rs2_data_reg)? PC_reg + imm_reg : PC_reg + 4;
                    flag = (rs1_data_reg == rs2_data_reg)? 1'b1 : 1'b0;
                end
                3'b010: begin
                    PC_result = (rs1_data_reg == rs2_data_reg)? PC_reg + 4 : PC_reg + imm_reg;
                    flag = (rs1_data_reg == rs2_data_reg)? 1'b0 : 1'b1;
                end
                3'b101: begin
                    PC_result = (rs1_data_reg < rs2_data_reg)? PC_reg + imm_reg : PC_reg + 4;
                    flag = (rs1_data_reg == rs2_data_reg)? 1'b1 : 1'b0;
                end
                3'b110: begin
                    PC_result = (rs1_data_reg >= rs2_data_reg)? PC_reg + imm_reg : PC_reg + 4;
                    flag = (rs1_data_reg == rs2_data_reg)? 1'b1 : 1'b0;
                end
                3'b011: begin
                    flag = ((rs1_data_reg[31] & ~rs2_data_reg[31]) || ((~rs1_data_reg[31] ^ rs2_data_reg[31]) && (rs1_data_reg[31] < rs2_data_reg[31])))? 1'b1 : 1'b0;
                    PC_result = flag?  PC_reg + imm_reg : PC_reg + 4;
                end
                3'b100: begin
                    flag = (!(rs1_data_reg[31] & ~rs2_data_reg[31]) || ((~rs1_data_reg[31] ^ rs2_data_reg[31]) && (rs1_data_reg[31] < rs2_data_reg[31])))? 1'b1 : 1'b0;
                    PC_result = flag?  PC_reg + imm_reg : PC_reg + 4;
                end     
            endcase
        end
        else begin
            PC_result = PC_result;     
        end
    end
    assign cmp_res = flag;
    assign PC_jump = PC_result;
    assign PC_wb = PC_reg + 4;

endmodule