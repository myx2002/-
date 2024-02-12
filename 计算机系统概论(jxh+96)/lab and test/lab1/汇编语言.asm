.orig x3000
LDI R1, store 
AND R4, R4, #0
ADD R4, R4, #-15
AND R3, R3, #0
AND R2, R2, #0
BRn Ad
TEST ADD R4, R4, #1
     BRp DONE
     ADD R1, R1, R1
     BRn Ad
     AND R3, R3, #0
     BRz TEST
Ad  ADD R3, R3, #1
    ADD R5, R3, #-4
    BRz SET
    BR  TEST
SET ADD R2, R2, #1
DONE TRAP x25
store .FILL x3100
.END