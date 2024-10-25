##
## 	Program Name:	Average.s 
##
##		- will calculate the average of 10 numbers that the user
##        will input
##		
##		
##		- Output format will be 
##		  "average = 15"
##
##		$t0 - point to array elements in turn
##		$t1 - contains count of elements
##      	$t2 - contains sum
##		
##		$t3 - each word from array in turn
##

#################################################
#                                               #
#               text segment                    #
#                                               #
#################################################

        .text
        .globl __start
__start:

    lw $t1, count
    la $t2, array
promptLoop:
    la $a0, prompt
    li $v0, 4
    syscall 

    li $v0, 5
    syscall 

    sw $v0, ($t2)
    addi $t2, $t2, 4
    addi $t1, $t1, -1

    bne $t1, $zero, promptLoop

    la $a0, array
    lw $a1, count
    jal CalAvg # average will be in v0
    move $t5, $v0

    la $a0, ans1
    li $v0, 4
    syscall 

    move $a0, $t5
    li $v0, 1
    syscall

	la $a0,endl	# syscall to print out
	li $v0,4	# a new line
	syscall 

	li $v0,10	# Exit
	syscall		# Bye!

CalAvg: # a0 = array, a1 = current count, t3 = current number, t4 = sum, t5 = unchanged count
    move $t5, $a1
AvgLoop:
    lw $t3, ($a0)
    add $t4, $t4, $t3 

    addi $a0, $a0, 4
    addi $a1, $a1, -1

    bne $a1, $zero, AvgLoop

    div $v0, $t4, $t5
    jr $ra


#################################################
#                                               #
#               data segment                    #
#                                               #
#################################################

        .data
	array:	.space 40
	count:	.word 10
    prompt: .asciiz "Please enter a number: "
	ans1:	.asciiz "average = "
	endl:	.asciiz "\n"	   

##
## 	end of file loop1.s