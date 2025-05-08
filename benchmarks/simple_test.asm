; simple_immediate_test.asm

; load-immediate into R0 and R1
LI     0, 5                ; R0 ← 5
ISTORE 0, 0x00000010, 8    ; MEM[0x10] ← 5
LI     1, 7                ; R1 ← 7
ISTORE 1, 0x00000018, 8    ; MEM[0x18] ← 7

; now reload and add
ILOAD  0, 0x00000010, 8    ; R0 ← 5
ILOAD  1, 0x00000018, 8    ; R1 ← 7
ADD    0, 1                ; R0 ← 12
HLT                        ; stop
