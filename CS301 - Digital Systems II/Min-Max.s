;The semicolon is uded to lead an inline documentation
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
;Max 	DCD 0
MaxP 	DCD Max
;Min	DCD 0
MinP	DCD Min

N 	DCD 12
NUM1 	DCD 3, -7, 2, -2, 10, 20, 30, 15, 32, 8, 64, 66
POINTER DCD NUM1

 AREA    MYRAM, DATA, READWRITE
Max 	DCD 0
Min 	DCD 0
 
; The program
; Linker requires Reset_Handler
 
          AREA    MYCODE, CODE, READONLY
 
   	  ENTRY
   	  EXPORT Reset_Handler
 
 
Reset_Handler

;;;;;;;;;;User Code Start from the next line;;;;;;;;;;;;;;;;;

;	Add code below to find the maximum value and 
;	the minimum value in the number array NUM1.
;	You can use the example in the notes as a reference.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LDR R0, N ; R0 = num elements
	LDR R1, POINTER ; R1 = ptr to array
	MOV R2, #0      ; R2 = min, default to 0 
	MOV R3, #0      ; R3 = max, default to 0
	
LOOP
	LDR R4, [R1], #4; R4 = current element 
	
	CMP R4, R2 ; If less than min, new min
	BLT NEWMIN
	
	CMP R4, R3 ; If more than max, new max
	BGT NEWMAX

	SUBS R0, R0, #1 ; Decrement count 
	CMP R0, #0
	BGT LOOP ; If not at end of array, loop 
	
	B STOP 

NEWMIN
	MOV R2, R4
	SUBS R0, R0, #1
	BGT LOOP
	
NEWMAX
	MOV R3, R4
	SUBS R0, R0, #1
	BGT LOOP 
	
	
STOP
	B STOP
	
	END
	
	
	
	
	
	
	

