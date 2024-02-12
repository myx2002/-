`timescale 1ns / 1ps

module FU_mem(
    input clk, EN, mem_w,
    input[2:0] bhw,
    input[31:0] rs1_data, rs2_data, imm,
    output[31:0] mem_data,
    output finish
);

    reg[1:0] state;
    assign finish = state[0] == 1'b1;
    initial begin
        state = 0;
    end

    reg mem_w_reg;
    reg[2:0] bhw_reg;
    reg[31:0] rs2_data_reg;
    reg[31:0] addr;
    always@(posedge clk) begin
        if(EN)begin
            addr = rs1_data + imm;
            rs2_data_reg = rs2_data;
            mem_w_reg = mem_w;
            bhw_reg = bhw;
            state = 2'b10;
        end
        else if(finish)begin
            state = 0;
            addr =0;
            rs2_data_reg = 0;
            mem_w_reg = 0;
            bhw_reg = 0;
        end
        else begin
            state = state >> 1;
        end
    end

    RAM_B ram(.clka(clk),.addra(addr),.dina(rs2_data_reg),.wea(mem_w_reg),
        .douta(mem_data),.mem_u_b_h_w(bhw_reg));

endmodule