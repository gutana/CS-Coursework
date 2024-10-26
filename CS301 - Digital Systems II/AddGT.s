;The semicolon is used to lead an inline documentation
;When you write your program, you could have your info at the top document lock
;For Example:  Your Name, Student Number, what the program is for, and what it does etc.

;;; Directives
          PRESERVE8
          THUMB       
 
; Vector Table Mapped to Address 0 at Reset
; Linker requires __Vectors to be exported
 
          AREA    RESET, DATA, READONLY
          EXPORT  __Vectors
 
__Vectors 
	  DCD  0x20001000     ; stack pointer value when stack is empty
          DCD  Reset_Handler  ; reset vector
  
          ALIGN
		  
;Your Data section
	;AREA DATA
SUMP    DCD SUM
N 	DCD 7
NUM1 	DCD 3, -7, 2, -2, 10, 20, 30
POINTER DCD NUM1

 AREA    MYRAM, DATA, READWRITE
SUM 	DCD 0
 
; The program
; Linker requires Reset_Handler
 
          AREA    MYCODE, CODE, READONLY
 
   	  ENTRY
   	  EXPORT Reset_Handler
 
 
Reset_Handler

;;;;;;;;;;User Code Start from the next line;;;;;;;;;;;;

;	Please complete the program to add up all the 
;	numbers in the array NUM1 that are greater than 5.
;	Put the sum in the register R0.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LDR R1, N		 ; R1 = array size
	LDR R2, POINTER  ; R2 = Pointer to array
	MOV R0, #0 		 ; R0 = accumulator 
	
LOOP
	
	LDR R3, [R2], #4
	CMP R3, #5
	BLT DONTADD
	ADD R0, R0, R3

DONTADD
	SUBS R1, R1, #1
	BGT LOOP
	

STOP
	B STOP
	
	END
