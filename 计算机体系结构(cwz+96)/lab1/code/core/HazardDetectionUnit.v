`timescale 1ps/1ps

module HazardDetectionUnit(
    input clk,//ʱ�� debugclk,�ؼ��Ķ���(ÿ���׶���һ�£�
    //use��ʾ�Ƿ�ʹ��
    input Branch_ID, rs1use_ID, rs2use_ID,
    //BID--�Ƿ���ת�ź�
    input[1:0] hazard_optype_ID,//hazard���ͣ��鵼ʲô����ALU��
    input[4:0] rd_EXE, rd_MEM, rs1_ID, rs2_ID, rs2_EXE,
    //ȷ������λ��,ǰ�����û������
    output PC_EN_IF, reg_FD_EN, reg_FD_stall, reg_FD_flush,
        reg_DE_EN, reg_DE_flush, reg_EM_EN, reg_EM_flush, reg_MW_EN,
    output forward_ctrl_ls,
    output[1:0] forward_ctrl_A, forward_ctrl_B
);
   //ID�׶λ������д������ݣ����Ͳ�����ʵʱ��
   reg [5:0]record = 6'b0;
    always@(posedge clk)begin
        record[5:2] <= record[3:0];
        record[1:0] <= hazard_optype_ID;
    end
    //hazard���
    //exe hazard
    //��֪��regwrite�Ƿ���Ч---������hazardtype�й�
    //Ҫ��exe/mem��regwriteΪ1(������)
    wire exehazardA = (rs1use_ID && (rs1_ID != 0) && (rs1_ID == rd_EXE));
    wire exehazardB = (rs2use_ID && (rs2_ID != 0) && (rs2_ID == rd_EXE));
    
    //mem hazard
    //����Ϊ������ǰ�ݣ�,regwrite��ҪΪ1
    wire memhazardA = (rs1use_ID && (rs1_ID != 0) && (rs1_ID == rd_MEM));
    wire memhazardB = (rs2use_ID && (rs2_ID != 0) && (rs2_ID == rd_MEM));
    
    //L����ָ������⻯����Ҫʹ���жϣ�mem writeΪ1��
    //����bubble��ǰ��
    //if/if_id��ҪΪ0���ж���Ҫ����0��nop_idexҲ��Ҫ����-
    //��Ҫһ��������
    wire data_stallA = (rs1use_ID && (rs1_ID != 0) && (rs1_ID == rd_EXE));
    wire data_stallB = (rs2use_ID && (rs2_ID != 0) && (rs2_ID == rd_EXE));
    wire data_stall = (data_stallA || data_stallB) && record[3:2] == 2'b10;
    
    //��Ҫд���ڴ�ļĴ������ݿ��ܱ���д
    //����Sָ��ĳ�ͻ
    wire Shazard = (rs2_EXE == rd_MEM) && (rs2_EXE != 0) && (record[5:4] == 2'b0);
    
    //������ת��أ��ȼ���һֱ���������������
    //JAL��B��ָ��
    wire control_stall = Branch_ID; 
    
    //ѡ��mux������
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
    //���������жϻ��ƣ�Ӧ�ò���Ҫ��������ƶ���
    //�Ҳ�����Ҫflush ID�׶κ�ļĴ���
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
        //���ڲ���bubble�Ĵ���
        if(data_stall && ~control_stall)begin
            flagen <= 5'b11110;
            flagcon <= 5'b11110;
            flagstall <= 1'b1;
        end
        //������ת�����⴦��
        else if(control_stall)begin
            flagcon <= 5'b11111;
            flagen <= 5'b11111;
            flagflush <= 1'b1;
        end
        //S���������������
        else if(Shazard)begin
            fcl <= 1'b1;
        end
        else begin 
            if(exehazardA && record[1:0] == 2'b01)begin
                fca <= 2'b01;
            end
            else if(memhazardA && record[1:0] == 2'b01)begin
            //10��ʾ�ѵ�mem�׶εļ���ǰ��
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
    
    //forward��ѡһ*2
    assign forward_ctrl_A = fca;
    assign forward_ctrl_B = fcb;
    //write to reg��ѡ����
    assign forward_ctrl_ls = fcl;
    
    
    
    
    
    
    
    
    
endmodule