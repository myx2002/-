`timescale 1ns / 1ps

module ExceptionUnit(
    input clk, rst, //cpu clk
    //ָ�������ź�
    input csr_rw_in, //ָ���Ƿ�鵼�Ĵ�����д��
    input[1:0] csr_wsc_mode_in, //��ǰģʽ��csr�Ĵ����Ķ�ָ���12��13λ-->01w,10s,11c
    //ָ������ǰ�csr�Ĵ����е�ֵȡ������չ��32bitȻ�����rd�У��ٰ�rs�е�ֵ����csr�Ĵ�����
    //csrrs x5, mie, x6          x5 = mie; mie = x6
    //αָ��� I ����ԭ��rs��λ�ø�Ϊ5bit�޷���������
    input csr_w_imm_mux, //�Ƿ����������ź�
    input[11:0] csr_rw_addr_in,//ָ���20-31-->��Ҫ��д��csr�Ĵ�����ַ
    input[31:0] csr_w_data_reg,//��Ҫд��csr�Ĵ���������
    input[4:0] csr_w_data_imm,//��Ҫд���������������Ŀ��Ĵ�����
    
    output [31:0] csr_r_data_out,//��Ҫд��Ĵ������洢��csr�е�����
    
    //�ж�/�쳣��������ź�
    input interrupt, //������Դ��for future use
    input illegal_inst,//���Ϸ�ָ���źţ�
    //~isFlushed_WB & exp_vector_WB[3] 
    //WB�Ƿ�flush��û�б�flush;ill-inst���Ϊ1
    input l_access_fault,//L��ָ��Խ���ȡ
    input s_access_fault,//S��ָ��Խ��洢
    input ecall_m,//flushΪ0 + ecall�źŴ���

    input mret,
    // inst == 32'b0011000_00010_00000_000_00000_1110011 �ж��Ƿ�ʹ��mretαָ���ź�
    input[31:0] epc_cur,//��ǰ��ҪWB�׶ε�PC
    input[31:0] epc_next,//û�б�flush�ĵ�����һ���׶ε�PC
    output  [31:0] PC_redirect,//ȷ����Ҫд��IF�׶ε���ȷPC
    output  redirect_mux,//��һָ��PCѡ���ź�

    output  reg_FD_flush, reg_DE_flush, reg_EM_flush, reg_MW_flush, //�Ƿ���Ҫflush���׶μĴ����ź�
    output  RegWrite_cancel//�ܾ�д��Ĵ������ź�
);
    reg[31:0] CSR [0:15];
    
    wire[11:0] csr_raddr;//csr������ַ
    assign csr_raddr = csr_rw_addr_in;
    wire[11:0] csr_waddr;//csrд���ַ
    assign csr_waddr = csr_rw_addr_in;
    wire[31:0] csr_wdata;//csrд������
    assign csr_wdata = csr_w_imm_mux ? {27'b0,csr_w_data_imm}: csr_w_data_reg;
    wire csr_w;//csr�Ƿ�д���ź�
    assign csr_w = csr_rw_in;
    wire [1:0] csr_wsc;//csrд��ģʽ�ź�
    assign csr_wsc = csr_wsc_mode_in;
    
    wire raddr_valid;//csr�Ƿ����
    assign raddr_valid = csr_raddr[11:7] == 5'h6 && csr_raddr[5:3] == 3'h0;
    wire[3:0] raddr_map;//csr����Ŀ���ַ
    assign raddr_map = (csr_raddr[6] << 3) + csr_raddr[2:0];//�Ĵ�����ַ
    wire waddr_valid;//csr�Ƿ�д��
    assign waddr_valid = csr_waddr[11:7] == 5'h6 && csr_waddr[5:3] == 3'h0;
    wire[3:0] waddr_map;//csrд��Ŀ���ַ
    assign waddr_map = (csr_waddr[6] << 3) + csr_waddr[2:0];
    assign csr_r_data_out =CSR[raddr_map];//csr����д��Ŀ��Ĵ�������
    
    reg[31:0] mstatus;
    //��rdataΪģ���ֱ�����
     always@(posedge clk or posedge rst) begin
        mstatus = CSR[0];
        if(rst) begin
            //��ʼ��scr�Ĵ�����  
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
            //����һ��ststus��������Ҫ����Ĵ���
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
            CSR[9] = epc_next - 4'b0100;//������epc_next
            CSR[10] = {1'b1,31'b000_0000_0000_0000_0000_0000_0000_1011};
            CSR[11] = epc_cur;
            CSR[0] = {mstatus[31:13],2'b11,mstatus[10:7],mstatus[3],mstatus[5:4],1'b0,mstatus[2:0]};
        end
    end
    //�� mstatus��clk��rst
    //�ȿ����쳣
    wire flag = mret || illegal_inst || l_access_fault || s_access_fault || ecall_m || (interrupt && mstatus[3]);
    wire flag1 = illegal_inst || l_access_fault || s_access_fault || ecall_m || (interrupt && mstatus[3]);
    assign reg_FD_flush = flag ? 1'b1 : 1'b0;//fd�Ĵ���ˢ���ź�
    assign reg_DE_flush = flag ? 1'b1 : 1'b0;//de�Ĵ���ˢ���ź�
    assign reg_EM_flush = flag ? 1'b1 : 1'b0;//em�Ĵ���ˢ���ź�
    assign reg_MW_flush = flag ? 1'b1 : 1'b0; //mw�Ĵ���ˢ���ź�
    assign redirect_mux = flag ? 1'b1 : 1'b0;//ѡ��һ��PC����/trap��ת/mepc����
    assign PC_redirect = mret ? CSR[9] : CSR[5]; //
    assign RegWrite_cancel = l_access_fault ? 1'b1 : 1'b0;

endmodule