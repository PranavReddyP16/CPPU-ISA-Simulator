; exchange_sort_numeric.asm
; --------------------------
; ILOAD reg, addr, len
; ADD   rd, rs2           (uses rd=rs1; rs2)
; CMP   rs1, rs2
; JE    label
; JL    label
; ISTORE reg, addr, len
; JMP   label
; HLT   ; end execution

; ---- prologue ----
ILOAD 1, 0x00010000, 8    ; N   = 2048
ILOAD 7, 0x00010010, 8    ; ONE = 1
ILOAD 0, 0x00010020, 8    ; base pointer A
ILOAD 2, 0x00010018, 8    ; i   = 0

; ---- outer loop ----
LOOP_I_CMP:
CMP   2, 1                ; if i == N?
JE    DONE_SORT

; j = i+1
ILOAD 3, 0x00010018, 8    ; j = 0
ADD   3, 2                ; j = i
ADD   3, 7                ; j++

; ---- inner loop ----
LOOP_J_CMP:
CMP   3, 1                ; if j == N?
JE    INC_I

; compute ptr_i → R6
ILOAD 6, 0x00010018, 8    ; tmp = 0
ADD   6, 2                ; tmp = i
LSHIFT 6, 3, 0            ; tmp <<= 3
ADD   6, 0                ; tmp += base
ILOAD 4, 6, 8             ; R4 = A[i]

; compute ptr_j → R5
ILOAD 5, 0x00010018, 8
ADD   5, 3
LSHIFT 5, 3, 0
ADD   5, 0
ILOAD 5, 5, 8             ; R5 = A[j]

; compare & swap if needed
CMP   4, 5
JL    NO_SWAP
JE    NO_SWAP

; swap: A[i] = R5
ISTORE 5, 6, 8
; A[j] = R4
ILOAD 6, 0x00010018, 8
ADD   6, 3
LSHIFT 6, 3, 0
ADD   6, 0
ISTORE 4, 6, 8

NO_SWAP:
ADD   3, 7                ; j++
JMP   LOOP_J_CMP

INC_I:
ADD   2, 7                ; i++
JMP   LOOP_I_CMP

DONE_SORT:
HLT
