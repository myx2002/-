.orig   x0180
.fill   x2000
.end

;直接如此也可以实现x2000向x0180的导入，但是仍然需要操作将KBSR[14]置1

.orig   x2000
        getc
        st    r2,save2              ;save the original data in save_
        st    r3,save3
        st    r5,save5
        
        add   r5,r0, #0             ;judge the input 
        ld    r2,number9            ;if the input is smaller than ASCII code of 9, branch to number mode
        add   r3,r2,r5
        brnz  number
        ld    r2,characa            ;if the input is biger than ASCII code of a, branch to character mode
        add   r3,r2,r5
        brzp  charac
        brnzp over                  ;if the input is illegal, branch to over and ignore it
        
number  ld    r2,number1            ;if the input is illegal, branch to over and ignore it
        add   r3,r5,r2
        brn   over
        ld    r5,minisfe
        add   r5,r0,r5              ;transfer the ASCII code to digit number
        not   r5,r5                 ;add the offset to r4
        add   r4,r5,r4
        ld    r5,numbertest
        add   r5,r5,r4
        brzp  over
        ld    r4,minisset           ;if the result is smaller than initializing condition, initialize the r4
        brnzp over
        
charac  ld    r2,characz            ;if the input is illegal, branch to over and ignore it
        add   r3,r5,r2
        brp   over
        add   r1,r0, #0             ;load r0 to r1, to output it in the next line
        brnzp over
        
over    ld    r2,save2              ;give back the data store in the save_
        ld    r3,save3
        ld    r5,save5
        rti
        
number9     .fill #-57              ;store the ASCII code of -9
number1     .fill #-48              ;store the ASCII code of -1
characa     .fill #-97              ;store the ASCII code of -a
characz     .fill #-122             ;store the ASCII code of -z
numbertest  .fill #17               ;store the condition number 17
minisset    .fill #-17              ;store the condition number -17
minisfe     .fill #-48              ;store the condition number -48
save2       .blkw 1                 ;open up a location for data store
save3       .blkw 1
save5       .blkw 1
.end
    
.orig   x3000 
        ld    r1,first              ;initialize r1 ,let r2=0,r3=3,r4=-17
        ld    r4,minisst
start   ld    r3,loopno
        and   r2,r2, #0
loop    ld    r0,point              ;output point in the line first
        add   r5,r2,r4
        brz   output
        trap  x21
        jsr   delay                 ;delay the output appropriately
        add   r2,r2, #1             ;loop condition
        brnzp loop
        
output  add   r0,r1, #0             ;output the current character three times
        trap  x21
        jsr   delay                 ;delay the output appropriately
        add   r3,r3, #-1
        brz   reloop
        brnzp output

reloop  add   r2,r2, #3             ;after output the character, continue output the point
        ld    r5,ministt
loop2   add   r3,r5,r2              ;judge the total output number in one line
        brz   reset                 ;if the total number reach the max number of each line, branch to reset
        ld    r0,point
        trap  x21
        jsr   delay                 ;delay the output appropriately
        add   r2,r2, #1
        brnzp loop2
        
reset   ld    r0,enter              ;output Newline character to start a new line    
        trap  x21
        add   r4,r4, #1             ;minis one to output the character one location ahead
        brnz  start
        brp   change                
        
change  add   r4,r4, #-1            ;if the first output location is front end, we maintain the location number in r4
        brnzp loop
   
delay   st    r2,savep2             ;delay mode, store the original data in certain place
        ld    r2,delaypart
loop3   add   r2,r2, #-1            ;use large number circle to delay each output state
        brnp  loop3
        ld    r2,savep2             ;load the original data
        ret

first       .fill x0061             ;store the  ASCII code of a
point       .fill x002e             ;store the  ASCII code of .
enter       .fill x000a             ;store the  ASCII code of enter
loopnzp     .fill x0014             ;store the digit number 20
loopno      .fill x0003             ;store the digit number 3
minisst     .fill xffef             ;store the digit number -20
ministt     .fill xffec             ;store the digit number -20
delaypart   .fill x0222             ;set the delay degree
savep2      .blkw 1
.end