        .ORIG x3000
        
        LD R4, length
        ADD R4, R4, #-1
        NOT R4, R4
        ADD R4, R4, #1
        ST R4, NEG_L
        
        LD R6, STACK
        LEA R0, string
        AND R1, R1, #0
        
        JSR RECUR
        HALT
        
string  .STRINGZ "123456"
length  .FILL #6

STACK   .FILL x6000
ENTER   .FILL x000A
NEG_L   .BLKW 1

; SWAP() swap the value indexed on R2 and R3 in the string
SWAP    ST R2, SAVER2
        ST R3, SAVER3
        ST R4, SAVER4
        
        ADD R2, R0, R2
        LDR R4, R2, #0
        ADD R3, R0, R3
        LDR R3, R3, #0
        STR R3, R2, #0
        LD R3, SAVER3
        ADD R3, R0, R3
        STR R4, R3, #0
        
        LD R2, SAVER2
        LD R3, SAVER3
        LD R4, SAVER4
        RET

SAVER2  .BLKW 1
SAVER3  .BLKW 1
SAVER4  .BLKW 1

; ***********************************************
; R1 - n

RECUR   ADD R6, R6, #-1
        STR R7, R6, #0
        
        ADD R2, R1, #0 ; n
        ADD R3, R1, #0 ; i
        
        LD R4, NEG_L
        ADD R4, R1, R4
        BRn NO_EXIT
        
        PUTS
        LD R0, ENTER
        OUT
        LEA R0, string
        BRnzp DONE
        
NO_EXIT JSR SWAP
        ADD R1, R1, #1
        ADD R6, R6, #-1
        STR R2, R6, #0
        ADD R6, R6, #-1
        STR R3, R6, #0
        JSR RECUR
        LDR R3, R6, #0
        ADD R6, R6, #1
        LDR R2, R6, #0
        ADD R6, R6, #1
        ADD R1, R1, #-1
        JSR SWAP
        
        ADD R3, R3, #1
        LD R4, NEG_L
        ADD R4, R4, R3
        BRnz NO_EXIT
        
DONE    LDR R7, R6, #0
        ADD R6, R6, #1
        RET
        
        .END