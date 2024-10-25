##
## 	Program Name:	loop1.s 
##
##		- will calculate the sum of the elements in "array".
##		- "count" holds the number of elements in "array".
##		
##		
##		- Output format must be 
##		  "sum = 15"
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
__start:                # execution starts here

#	Put your answer between dashed lines.
#
#------------------Your code starts next line---------------

    addi $t2, $zero, 0 # ensure that sum starts at 0
    lw $t1, count
    la $t0, array
loop:
    lw $t3, ($t0)
    add $t2, $t2, $t3
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
	array:	.word 3, 4, 2, 6
	count:	.word 4
	ans1:	.asciiz "sum = "
	endl:	.asciiz "\n"	   

##
## 	end of file loop1.s