##
## 	Program Name:	loop2.s 
##
##		- will calculate the sum of all elements in the array "numbers"
##		  whose value is less than or equal to 1000.
##		- "numbers" is an array with 5 integer elements.
##		- "count" holds the number of elements in "numbers".
##		
##		
##		- Output format must be 
##		  "sum = 11"
##
##		$t0 - points to array elements in turn
##		$t1 - contains a count of elements
##      	$t2 - contains sum
##		
##		$t3 - each word from the array "numbers" in turn
##

#################################################
#                                               #
#               text segment                    #
#                                               #
#################################################

        .text
        .globl __start
__start:                # execution starts here


#	Put your answer between dashed lines.
#
#------------------Your code starts next line---------------
    addi $t4, $zero, 1000
    addi $t2, $zero, 0 # ensure that sum starts at 0
    lw $t1, count
    la $t0, numbers
loop:
    lw $t3, ($t0)
    bgt $t3, $t4, dontAdd
    add $t2, $t2, $t3
dontAdd:

    addi $t1, $t1, -1
    addi $t0, $t0, 4
    bne $t1, $zero, loop 

printSum:
    la $a0, ans1
    li $v0, 4
    syscall 

    move $a0, $t2
    li $v0, 1
    syscall

#-----------------Your code ends above this line----------------

	la $a0,endl	# syscall to print out
	li $v0,4	# a new line
	syscall 

	li $v0,10	# Exit
	syscall		# Bye!


#################################################
#                                               #
#               data segment                    #
#                                               #
#################################################

        .data
	numbers:	
		.word 3, 2000, 2, 6, 3000
	count:	.word 5
	
	ans1:	.asciiz "sum = "
	endl:	.asciiz "\n"	   

##
## 	end of file loop2.s