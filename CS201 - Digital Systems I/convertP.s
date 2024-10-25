##
## 	The program --- convertP.s 
##
##		- will ask the user whether temp is in F or C,
##		- take in a temp, convert it to the opposite, and
##		- print the result.
##
##		Here is the formula of the conversion:
##           
##              C = 5 * (F - 32) / 9
##
##

#################################################
#                                               #
#               text segment                    #
#                                               #
#################################################

        .text
        .globl __start
__start:                # execution starts here
    la $a0, typePrompt  # Ask user what type of conversion they want
    li $v0, 4           # system call to print string
    syscall

    la $a0, inputtedChar
    li $a1, 3
    li $v0, 8
    syscall             # get input char from user 

    la $t2, inputtedChar
    lb $s0, ($t2)   # load the first byte of the inputted text into $s0
    
    
    la $a0, tempPrompt  # Ask user to input temperature integer
    li $v0, 4           
    syscall             

    li $v0,5	# syscall 5 reads an integer
    syscall     # integer is stored in $v0 

    addi $a0, $v0, 0 # integer is now in $a0

    # call procedure depending on what user inputted
    beq $s0, 'f', FtoC 
    beq $s0, 'F', FtoC
    beq $s0, 'c', CtoF
    beq $s0, 'C', CtoF
    # temp should be in s1

    # if we get here, it wasn't one of the chars we were looking for

    la $a0, wrongLetter
    li $v0, 4
    syscall

    j __start # loop back around and try to get a good input

printString:
    la $a0, ans
    li $v0, 4
    syscall

    li $v0, 1
    addi $a0, $s1, 0
    syscall 

endOfProgram:
    la $a0,endl	# syscall to print out
	li $v0,4	# a new line
	syscall 

	li $v0,10
	syscall		# Bye!
############################################################
##
##	The following procedures will confert F to C, or vice 
##  versa. 
##
############################################################

FtoC:
    jal formulaFtoC # temperature should be in $v0
    move $s1, $v0
    j printString

CtoF:
    jal formulaCtoF
    move $s1, $v0
    j printString # temperature should be in $v0

formulaFtoC:


    sub $t0, $a0, 32 # subtract 32
    mul $t0, $t0, 5  # multiply by 5
    div $t0, $t0, 9  # divide by 9

    addi $v0, $t0, 0 # move to return register

    jr $ra

formulaCtoF:

    div $t0, $a0, 5
    mul $t0, $t0, 9
    add $t0, $t0, 32

    addi $v0, $t0, 0 # move to return register

    jr $ra


#################################################
#                                               #
#               data segment                    #
#                                               #
#################################################

        .data
    typePrompt: .asciiz "Is your temperature in (C) or (F)? "
	tempPrompt:	.asciiz "Enter temperature (integer): "
	ans:	.asciiz "The temperature is: "
    wrongLetter: .asciiz "Wrong Letter!\n"
	endl:	.asciiz "\n"	   
    inputtedChar: .space 3
    
##
## 	end of file convertP.s