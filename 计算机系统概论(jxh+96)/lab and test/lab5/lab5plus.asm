.orig x3000
;**************************************************************
;input total number mode
        trap  x20               ;input the total number
        trap  x21
        ld    r1,midigit        ;minis 48, transfer the ASCII code to digit
        add   r0,r0,r1
        add   r4,r0, #0         ;contain the original input
        trap  x20               ;judge the second input 
        trap  x21
        ld    r3,data1          ;if the second input is enter, then the only input maybe is 1
        add   r6,r3,r0
        brz   maybe
        add   r4,r0,r1          ;if the second input is also a digit
        add   r4,r4, #10        ;add the first multiply ten and the second one
        trap  x20               ;absord the enter
        trap  x21
        brnzp start
        
maybe   add   r2,r4, #-1        ;if the input is 1, branch to the only mode
        brz   only
        brnzp start             ;else, operate it as normaldigit
      
only    trap  x20               ;absorb the enter and the one
        trap  x21
        trap  x20
        trap  x21
        and   r0,r0, #0         ;putput the only digit 1 and stop the program
        add   r0,r0, #1
        ld    r1,pldigit        ;transfer the digit number to ASCII code 
        add   r0,r0,r1
        trap  x21
        trap  x25
        
;*****************************************************************  
;input the different chapters each page contain
start   st    r4,stonum         ;use r4 to record the total number
        ld    r4,stonum
        ld    r6,stack1         ;use r6 to contain the first stack
        ld    r3,stack2         ;use r3 to contain the second stack
        and   r7,r7, #0

;judge the kind of the input      
loop1   trap  x20       
        trap  x21
        ld    r1,data2          ;if a space follow the digit, branch to the first
        add   r1,r1,r0
        brz   first
        ld    r1,data1          ;if a enter follow the digit, branch to the second
        add   r1,r1,r0
        brz   second
        brnzp input             ;other the input is a digit
        
first   add   r1,r7, #0
        ld    r2,judge2         ;in the space mode, we set r2 to negative number
        jsr   store             ;after set the flag r2, call the store function
        and   r7,r7, #0         ;initialize the sum number
        brnzp loop1
        
second  add   r1,r7, #0
        ld    r2,judge1         ;in the enter mode, we set r2 to zero
        jsr   store             ;after set the flag r2, call the store function
        add   r4,r4, #-1        ;count the total number of enter
        brz   opera             ;if the number reach the total, we start to operate it
        and   r7,r7, #0         ;initialize the sum number
        brnzp loop1
    
input   ld    r1,midigit        ;transfer the ASCII code to digit number
        add   r1,r0,r1
        and   r0,r0, #0
        add   r0,r0, #9
        st    r4,save4
        add   r4,r7, #0
loop2   add   r7,r7,r4          ;if there input two successive number, we should use loop to add them in circle
        add   r0,r0, #-1
        brp   loop2
        ld    r4,save4
        add   r7,r7,r1          ;add the current number to sum number
        brnzp loop1             ;deal the next input

store   add   r2,r2, #0         ;if the flag is negative number
        brn   store1            ;we store the digit in stack1
        brz   store2            ;else we store the digit in stack2
done    ret
        
store1  add   r6,r6, #1
        str   r1,r6, #0
        brnzp done 
        
store2  add   r3,r3, #1
        str   r1,r3, #0
        brnzp done  
        
;*****************************************************************
;the main function
opera   ld    r4,stonum             ;r4 contain the total number
        ld    r5,stack1             ;load the start location of every used stack
        ld    r6,stack2
        ld    r0,stack3
        add   r3,r4, #0
        jsr   main                  ;call the recursion function
        trap  x25
        
;*****************************************************************
;the body of recursion
main    add   r0,r0, #1             ;store the register, avoid covering the original number
        str   r3,r0, #0
        add   r0,r0, #1
        str   r2,r0, #0
        add   r0,r0, #1
        str   r4,r0, #0
        add   r0,r0, #1
        str   r7,r0, #0
        
        add   r3,r3, #0             ;r3 contain the current number, if the number equals to 0, make a output
        brz   test
        add   r2,r3, #-1            ;r2 equals to the n-1 in every circle
        
        add   r3,r2, #0             ;load the r2 to r3
        jsr   main                  ;call the main in size n-1
        jsr   swap                  ;swap the current data in stack1 and stack2
        jsr   main
        jsr   swap                  ;recover the original data structure

then    ldr   r7,r0, #0             ;load the original number
        add   r0,r0, #-1
        ldr   r4,r0, #0
        add   r0,r0, #-1
        ldr   r2,r0, #0
        add   r0,r0, #-1
        ldr   r3,r0, #0
        add   r0,r0, #-1
        ret 
        
test    jsr   check                 ;if the current number is 0, check the stack1
        jsr   outex                 ;if each element is differet from another, output it in order
        brnzp then                  ;continue the recursion

;***************************************************************
;check function
check   st    r2,save2              ;store the original number
        st    r3,save3
        st    r4,save4
        st    r5,save5
        st    r6,save6
        ld    r4,stonum             ;load the total number to r4
        
        and   r1,r1, #0             ;use r1 to contain the state of a function 
        ld    r5,stack1             ;judge the point is reach the end of the stack
        add   r6,r5,r4
        not   r5,r5
        add   r5,r5, #1
        add   r3,r6, #0             ;make a copy of the stack point
        add   r6,r6, #-1
loop3   add   r2,r5,r6              ;ergodic the stack, to find if there exists a pair of same number
        brz   loop4
        ldr   r2,r6, #0
        not   r2,r2
        add   r2,r2, #1
        ldr   r4,r3, #0
        add   r2,r2,r4
        brz   fail                  ;if there exists, set r1 to 1
        add   r6,r6, #-1            ;the condition of inner circle
        brnzp loop3

loop4   add   r3,r3, #-1            ;the condition of outer circle 
        add   r2,r3,r5
        brz   suces
        add   r6,r3, #-1
        brnzp loop3
        
fail    add   r1,r1, #1             ;else remain r1 0
suces   ld    r2,save2              ;load the original number
        ld    r3,save3
        ld    r4,save4
        ld    r5,save5
        ld    r6,save6
        ret
        
;****************************************************************
;output function
outex   add   r1,r1, #0             ;if the r1 is 0, take the output mode
        brp   over2                 ;else exit
        st    r0,save0              ;store the original number
        st    r2,save2
        st    r3,save3
        st    r4,save4
        st    r5,save5
        ld    r4,stonum
        ld    r5,stack1
loop5   add   r5,r5, #1             ;load number from the stack
        ldr   r0,r5, #0             ;make a copy
        ld    r2,miten
        add   r2,r2,r0              ;judge the number is larger than 10 or not
        brzp  deal
        ld    r3,pldigit            ;transfer the digit to ASCII code
        add   r0,r0,r3
next    trap  x21                   ;reduce the line number
        add   r4,r4, #-1            ;if the number is larger than 0, output a space in each pair of digit
        brz   over1
        ld    r0,data4              
        trap  x21
        brnzp loop5
        
deal    ld    r2,miten              ;if the digit is larger than 10
        add   r3,r0,r2
        ld    r0,pldigit
        add   r0,r0, #1             ;first we output the first numebr 1
        trap  x21
        ld    r0,pldigit
        add   r0,r0,r3              ;then we calculate the second number
        brnzp next
    
over1   ld    r0,data3              ;else start a new line
        trap  x21                   ;load the original number
        ld    r0,save0
        ld    r2,save2
        ld    r3,save3
        ld    r4,save4
        ld    r5,save5
        ld    r4,stonum
over2   ret
        
;****************************************************************    
;swap data function
swap   ld    r5,stack1              ;reload the start location of stack in r5,r6 
       ld    r6,stack2 
       st    r3,save3               ;store the original data
       st    r4,save4
       ld    r3,stonum              ;use r3 to store the total number
       add   r6,r6,r3
       add   r5,r5,r3
       not   r4,r2                  ;we use r2 to store the number of current i
       add   r4,r4, #1
       add   r6,r6,r4               ;offset the start location and load the data to r3 and r4
       add   r5,r5,r4
       ldr   r3,r5, #0
       ldr   r4,r6, #0
       str   r4,r5, #0              ;swap the data in different stack
       str   r3,r6, #0
       ld    r3,save3               ;load the data
       ld    r4,save4
       ret
  
;*********************************************************************
;the data used in this program 
data1   .fill xfff6         ;store the negative ASCII code of enter
data2   .fill xffe0         ;store the negative ASCII code of space
data3   .fill x000a         ;store ASCII code of enter
data4   .fill x0020         ;store ASCII code of space
stack1  .fill x33ff         ;store the start location of first input stack
stack2  .fill x34ff         ;store the start location of second input stack
stack3  .fill x35ff         ;store the data in each recursion processing
judge1  .fill x0000         ;use 0 or 1 to set the flag
judge2  .fill x8000
stonum  .blkw 1             ;make room for total number
midigit .fill #-48          ;the coefficient transfer ASCII code to digit number
pldigit .fill #48           ;the coeffivient transfer digit number to ASCII code
miten   .fill #-10          ;judge the output is larger than 10 or not
save0   .blkw 1             ;make room for storing corresponding register data
save2   .blkw 1
save3   .blkw 1
save4   .blkw 1
save5   .blkw 1
save6   .blkw 1

.end