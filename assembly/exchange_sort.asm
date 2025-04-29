; ---------- prologue -------------------------------------------------
ILOAD   R1, 0x00010000, 8      ; N  = 2048
ILOAD   R7, 0x00010010, 8      ; ONE = 1
ILOAD   R0, 0x00010020, 8      ; base pointer A
ILOAD   R2, 0x00010018, 8      ; i = 0   (ZERO)

; ---------- outer loop ----------------------------------------------
LOOP_I_CMP:
CMP     R2, R1                 ; i == N ?
JE      DONE_SORT              ; yes -> finished

; j = i + 1 -----------------------------------------------------------
ILOAD   R3, 0x00010018, 8      ; j = 0
ADD     R3, R2                 ; j = i
ADD     R3, R7                 ; j = i + 1

; ---------- inner loop ----------------------------------------------
LOOP_J_CMP:
CMP     R3, R1                 ; j == N ?
JE      INC_I                  ; break inner loop

; ------- ptr = base + (idx<<3)  (uses R6 as scratch) ----------------
; ptr_i in R6 ---------------------------------------------------------
ILOAD   R6, 0x00010018, 8      ; R6 = 0
ADD     R6, R2                 ; copy i
LSHIFT  R6, 3, 0               ; *8
ADD     R6, R0                 ; + base
ILOAD   R4, R6, 8              ; A[i]

; ptr_j in R5 (reuse register) ---------------------------------------
ILOAD   R5, 0x00010018, 8      ; R5 = 0
ADD     R5, R3                 ; copy j
LSHIFT  R5, 3, 0
ADD     R5, R0
ILOAD   R5, R5, 8              ; A[j] now in R5

; compare & maybe swap -----------------------------------------------
CMP     R4, R5
JL      NO_SWAP                ; A[i] < A[j]
JE      NO_SWAP                ; equal
ISTORE  R5, R6, 8              ; A[i] = A[j]
; recompute ptr_j (destroyed above) ----------------------------------
ILOAD   R6, 0x00010018, 8
ADD     R6, R3
LSHIFT  R6, 3, 0
ADD     R6, R0
ISTORE  R4, R6, 8              ; A[j] = old A[i]

NO_SWAP:
ADD     R3, R7                 ; ++j
JMP     LOOP_J_CMP

INC_I:
ADD     R2, R7                 ; ++i
JMP     LOOP_I_CMP

DONE_SORT:
HLT
