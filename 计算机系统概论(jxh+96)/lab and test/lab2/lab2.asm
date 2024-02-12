.orig x3000

and r2,r2,#0                    ;initialize the register,r1=r2=r3=r4=0
and r3,r3,#0
and r4,r4,#0
ld  r5,enter                    ;r5=xfff6,the negative ASCII code of enter

get trap x20                    ;get input
    add r1,r5,r0                ;judge whether the input is enter or not
    trap x21                    ;output the input we get
    brz trans                   ;if the input is enter,than branch to the transfer part
    add r2,r2,#9                ;let r2=9,as a loop counter
    add r1,r3,#0                ;let r1=r3
loop1 add r3,r1,r3              ;if there exists other single digit input, we should multiply 10 with r3 and then add to the new input 
      add r2,r2,#-1
      brp loop1                 ;continue the loop until r2=0
    add r4,r0,#-16              ;trap x21 receive ASCII code,we need transfer it into decimal digit which equals to minis 48
    add r4,r4,#-16
    add r4,r4,#-16
    add r3,r4,r3
    brnzp get                   ;after have operated current input,we receive the next input

trans lea r4,high               ;load the lacation of current bit digit
      ldr r1,r4,#0              ;load the data
      add r3,r3,#0              ;if r3>xefff,we need adjust it first
      brn adjust

loop2 add r3,r3,r1              ;compare the r3 and r1,if r3<r1,then we need stop the transfer of current four bits and output it
      brn looprestart           ;we use r2 to record the current hex number
      add r2,r2,#1              ;if r3>r1,we need operate r3-r1,and start the next loop
      brnzp loop2
looprestart  jsr output         ;call the output function
             add r4,r4,#1       ;let r4 point to the nect parameter to calculate the nect four bits
             ldr,r0,r4,#0       ;if the data equals to 0,then stop the program
             brz done
             and r2,r2,#0       ;after one circle,we initialize the r2
             add r1,r1,#-1      ;and minis r1 to r3 to make up the overminis
             not r1,r1
             add r3,r3,r1
             ldr r1,r4,#0       ;load the data of new location
             brnzp loop2

done    trap x25

adjust  ld  r5,data3            ;if the intput is larger than x1000,we can use 0 to mask the first bit of it
        add r2,r2,#8            ;and add 8 to its first r2
        and r3,r3,r5
        brnzp loop2

output  add r0,r2,#-9           ;compare r2 with 9,if r2<9,we can transfer it to ASCII code and output it at once
        brp select              ;else we need transfer it to hex character number
        ld  r5,data1
return  add r0,r2,r5            ;load the ASCII code to r0,then output it
        trap x21                
        ret
select  ld  r5,data2            ;add x0037 to the decimal digit to transfer it to ASCII code A B C D E F
        brnzp return

high  .fill xf000               ;store the minis of x1000
mid   .fill xff00               ;store the minis of x0100
low   .fill xfff0               ;store the minis of x0010
last  .fill xffff               ;store the minis of x0001
deal  .fill x0000               ;judge the transferment complete or not
data1 .fill x0030               ;store the transfer hex number of decimal number
data2 .fill x0037               ;store the special transfer hex number,as A B C D E F
data3 .fill x7fff               ;judge whether the unsigned number is a negative number in LC-3 or not
enter .fill xfff6               ;store the minis of the ASCII code of enter

.end