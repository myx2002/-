.orig x3000
        and  r2,r2, #0            ;initialize r1=r2=0
        ld   r5,stack             ;record the bottom point of the stack
        ld   r6,stack             ;record the top point of the stack
        ld   r7,ouput             ;record the point of the output stack 
 
test    trap x20                  ;display the input of operate
        trap x21
        ld   r2,enter             ;load the minis hex number of enter
        add  r2,r0,r2             ;if input equals to enter, branch to output mode 
        brz  output
        ld   r2,minis
        add  r2,r0,r2
        brz  opemi                ;if input equals to minis, branch to minis mode 
        ld   r2,plus
        add  r2,r0,r2
        brz  opepl                ;if input equals to plus, branch to plus mode 
        ld   r2,left
        add  r2,r0,r2
        brz  opele                ;if input equals to left brackets, branch to left brackets mode 
        ld   r2,right
        add  r2,r0,r2
        brz  operi                ;if input equals to right brackets, branch to right brackets mode 
        brnzp test                ;other input, no processing, continue the loop

opemi   and  r4,r4, #0            ;judge the location fo r5 and r6, if r5 equals to r6, the structure is empty
        not  r4,r5
        add  r4,r4, #1
        add  r4,r6,r4
        brz  addmr                ;if the structure is empty, branch to the add-to-output mode
        brnzp admi                ;if the structure is not empty, branch to the other output mode
        
opepl   trap x20                  ;display the character be inputed
        trap x21
        add  r2,r0, #0            ;load r0 to r2 to add to the output stack
        brnzp adpl

opele   trap x20                  ;display the character be inputed
        trap x21
        add  r2,r0, #0            ;load r0 to r2 to add to the output stack
        brnzp adle

operi   and  r4,r4, #0            ;compare the location of r5 and r6, similar to minis mode
        not  r4,r5
        add  r4,r4, #1
        add  r4,r6,r4
        brz  addmr                ;if the structure is empty, branch to the add-to-output mode
        brnzp adri                ;if the structure is not empty, branch to the other output mode

output  ld   r2,mark              ;at the end of output stack, add x0000 to stop the trap x22
        str  r2,r7, #0
        ld   r0,ouput             ;output the output stack
        trap x22
        trap x25                  ;halt

addmr   ld   r2,unline            ;output the '_' 
addmr2  str  r2,r7, #0            ;add it to output stack, move r7 to next location
        add  r7,r7, #1
        brnzp test                ;continue the loop
        
admi    add  r5,r5, #1            ;if we pop in the left, we first move the point, then load the data
        ldr  r2,r5, #0
        brnzp addmr2              ;add the data into the output stack

adpl    str  r2,r5, #0            ;if we store in the left, we first store the data, then move the point
        add  r5,r5, #-1           
        brnzp test                ;continue the loop
        
adri    ldr  r2,r6, #0            ;if we pop in the right, we first load the data, then move the point
        add  r6,r6, #-1
        brnzp addmr2              ;add the data into the output stack

adle    add  r6,r6, #1            ;if we store in the right, we first move the point, then store the data
        str  r2,r6, #0
        brnzp test                ;continue the loop

stack  .fill x4000                ;store the start location of input stack
ouput  .fill x5000                ;store the start location of output stack
enter  .fill xfff6                ;store the minis of enter
minis  .fill xffd3                ;store the minis of minis
plus   .fill xffd5                ;store the minis of plus
left   .fill xffa5                ;store the minis of left brackets
right  .fill xffa3                ;store the minis of right brackets
unline .fill x005f                ;store the minis of underline
mark   .fill x0000

.end