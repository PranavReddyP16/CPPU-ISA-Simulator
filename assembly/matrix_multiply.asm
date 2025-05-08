; ------------ prologue ----------------------------------------------
ILOAD   R3, 0x00010008, 8      ; M = 64
ILOAD   R0, 0x00010028, 8      ; base A64
ILOAD   R1, 0x00010030, 8      ; base B64
ILOAD   R2, 0x00010038, 8      ; base C64
ILOAD   R7, 0x00010018, 8      ; ZERO
ISTORE  R7, 0x00010048, 8      ; i = 0

; ------------ outer loop on i ---------------------------------------
LOOP_I:
ILOAD   R4, 0x00010048, 8      ; R4 = i
CMP     R4, R3
JE      DONE_MATMUL

ISTORE  R7, 0x00010050, 8      ; j = 0

; ---------- middle loop on j ----------------------------------------
LOOP_J:
ILOAD   R5, 0x00010050, 8      ; R5 = j
CMP     R5, R3
JE      INC_I

ISTORE  R7, 0x00010040, 8      ; SUM = 0
ISTORE  R7, 0x00010058, 8      ; k = 0

; ---------- inner loop on k -----------------------------------------
LOOP_K:
ILOAD   R6, 0x00010058, 8      ; R6 = k
CMP     R6, R3
JE      STORE_C

; ---- A[i*M+k] into R7 ----------------------------------------------
ILOAD   R7, 0x00010048, 8      ; R7 = i
MUL     R7, R3                 ; i*M   -> high in R7, low in R3 (ignored)
ADD     R7, R6                 ; + k
LSHIFT  R7, 3, 0               ; *8
ADD     R7, R0                 ; + base A
ILOAD   R7, R7, 8              ; a = ...

; ---- B[k*M+j] into R6 ----------------------------------------------
MUL     R6, R3                 ; k*M
ILOAD   R5, 0x00010050, 8      ; R5 = j   (destroys R5 temporarily)
ADD     R6, R5
LSHIFT  R6, 3, 0
ADD     R6, R1
ILOAD   R6, R6, 8              ; b = ...

; ---- sum += a*b -----------------------------------------------------
MUL     R7, R6                 ; product: low bits now in R6
ILOAD   R5, 0x00010040, 8      ; R5 = SUM
ADD     R5, R6
ISTORE  R5, 0x00010040, 8      ; SUM = new value

; ---- ++k ------------------------------------------------------------
ILOAD   R5, 0x00010010, 8      ; ONE
ILOAD   R6, 0x00010058, 8
ADD     R6, R5
ISTORE  R6, 0x00010058, 8
JMP     LOOP_K

; ---------- write C[i*M+j] ------------------------------------------
STORE_C:
; addr = baseC + ((i*M + j)<<3)
ILOAD   R6, 0x00010048, 8      ; i
MUL     R6, R3
ILOAD   R5, 0x00010050, 8      ; j
ADD     R6, R5
LSHIFT  R6, 3, 0
ADD     R6, R2
ILOAD   R5, 0x00010040, 8      ; SUM
ISTORE  R5, R6, 8

; ++j -----------------------------------------------------------------
ILOAD   R5, 0x00010010, 8      ; ONE
ILOAD   R6, 0x00010050, 8
ADD     R6, R5
ISTORE  R6, 0x00010050, 8
JMP     LOOP_J

; ++i -----------------------------------------------------------------
INC_I:
ILOAD   R5, 0x00010010, 8      ; ONE
ILOAD   R4, 0x00010048, 8
ADD     R4, R5
ISTORE  R4, 0x00010048, 8
JMP     LOOP_I

DONE_MATMUL:
HLT
