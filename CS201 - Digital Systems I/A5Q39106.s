## filename: A5Q39106.s
##
## This program will implement the following statement: 
##
## switch (S) {
##   case  5: A = A + 1; break;
##   case 25: A = A - 1; break;
##   default: A = A * 2; break;
## }
##
##  S      = $t0
##  A      = $t1

#################################################
#                                               #
#               text segment                    #
#                                               #
#################################################

        .text
        .globl __start
__start:                
    # prompt user for S
    la $a0, S_Prompt  
    li $v0, 4           
    syscall

    li $v0, 5
    syscall
    addi $t0, $v0, 0 # S is in $t0 

    # prompt user for A
    la $a0, A_Prompt  
    li $v0, 4           
    syscall

    li $v0, 5
    syscall
    addi $t1, $v0, 0 # A is in $t1 

    # check if any of the conditions are met 
    beq $t0, 5, s_is_5
    beq $t0, 25, s_is_25

    # do default if we didn't branch
    sll $t1, $t1, 1 # shifting left 1 is multiplying by 2

end:
    la $a0, response
    li $v0, 4
    syscall 

    addi $a0, $t1, 0
    li $v0, 1
    syscall

    la $a0, endl
    li $v0, 4
    syscall 

	li $v0,10
	syscall		# Bye!

s_is_5: 
    addi $t1, $t1, 1
    j end 

s_is_25: 
    addi $t1, $t1, -1
    j end 

#################################################
#                                               #
#               data segment                    #
#                                               #
#################################################

        .data
    S_Prompt: .asciiz "Enter a value for S: "
    A_Prompt: .asciiz "Enter a value for A: "
    response: .asciiz "The result is: "
    endl:     .asciiz "\n"
    
##