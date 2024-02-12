Main        .ORIG x3000
            
            JSR InputName
            JSR Traverse
            HALT
            
DataAddr    .FILL x4000
Name        .BLKW 20
            
; ******************************************************************** ;
;

InputName   ST R1, IN_R1Store
            ST R2, IN_R2Store
            LEA R2, Name
            LEA R0, Tips
            TRAP x22
            
Input_LOOP  TRAP x20
            TRAP x21
            LD R1, IN_ASCII2
            ADD R1, R0, R1
            BRz Input_Fine
            STR R0, R2, #0
            ADD R2, R2, #1
            BRnzp Input_LOOP
            
Input_Fine  AND R0, R0, #0
            STR R0, R2, #0
            
            LD R1, IN_R1Store
            LD R2, IN_R2Store
            RET

Tips        .STRINGZ "Input a Name: "
IN_R1Store  .BLKW 1
IN_R2Store  .BLKW 1
IN_ASCII1   .FILL x000A ; ascii of enter
IN_ASCII2   .FILL xFFF6


; ******************************************************************** ;

Traverse    ST R1, T_R1Store
            ST R2, T_R2Store
            ST R3, T_R3Store
            ST R4, T_R4Store
            ST R7, T_R7Store
            LEA R3, Name
            LDI R1, DataAddr
            
T_Loop      ADD R1, R1, #0
            BRz T_END
            LDR R4, R1, #2
            JSR CompStr
            ADD R5, R5, #0
            BRnp Print
            LDR R4, R1, #3
            JSR CompStr
            ADD R5, R5, #0
            BRz T_Loop2
            
Print       LDR R0, R1, #2
            TRAP x22
            LD R0, T_ASCII1
            TRAP x21
            LDR R0, R1, #3
            TRAP x22
            LD R0, T_ASCII1
            TRAP x21
            LDR R0, R1, #1
            TRAP x22
            LD R0, T_ASCII2
            TRAP x21

T_Loop2     LDR R1, R1, #0
            BRnzp T_Loop
            
T_END       LD R1, T_R1Store
            LD R2, T_R2Store
            LD R3, T_R3Store
            LD R4, T_R4Store
            LD R7, T_R7Store
            RET

T_R1Store   .BLKW 1
T_R2Store   .BLKW 1
T_R3Store   .BLKW 1
T_R4Store   .BLKW 1
T_R7Store   .BLKW 1
T_ASCII1    .FILL x0020
T_ASCII2    .FILL x000A


; ******************************************************************** ;
;; R3, R4 - address of the 2 strings
;; R5 - 0 - inequal, 1 - equal

CompStr     ST R1, CS_R1Store
            ST R2, CS_R2Store
            ST R3, CS_R3Store
            ST R4, CS_R4Store
            AND R5, R5, #0
            
CS_Loop     LDR R1, R3, #0
            LDR R2, R4, #0
            NOT R2, R2
            ADD R2, R2, #1
            ADD R2, R1, R2
            BRnp CS_Inequal
            ADD R1, R1, #0
            BRz CS_Equal
            ADD R3, R3, #1
            ADD R4, R4, #1
            BRnzp CS_Loop

CS_Equal    ADD R5, R5, #1
CS_Inequal  LD R1, CS_R1Store
            LD R2, CS_R2Store
            LD R3, CS_R3Store
            LD R4, CS_R4Store
            RET

CS_R1Store  .BLKW 1
CS_R2Store  .BLKW 1
CS_R3Store  .BLKW 1
CS_R4Store  .BLKW 1

            .END
            
            
            