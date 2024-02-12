`timescale 1ns / 1ps

module FU_mul(
    input clk, EN,
    input[31:0] A, B,
    output[31:0] res,
    output finish
);
    wire[31:0] mulres;
    reg[6:0] state;
    assign finish = state[0] == 1'b1;
    initial begin
        state = 0;
    end

    reg[31:0] A_reg, B_reg;
    
    always@(posedge clk) begin
        if(EN) begin 
            A_reg = A;
            B_reg = B;
            state = 7'b100_0000;
        end
        else if(finish)begin
            A_reg = 0;
            B_reg = 0;
            state = 0;
        end
        else begin
            state = state >> 1;
        end
    end
    
    multiplier mul(.CLK(clk),.A(A_reg),.B(B_reg),.P(mulres));
    
    assign res = mulres[31:0];

endmodule