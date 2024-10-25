##
## 	Program Name:	insertion_sort.s 
##
##		- will sort an array by absolute value
##		
##		- Assume the array has at least two elements (a[0] and a[1]). 
##		- This program will use pointers.
##		
##      - Use at least one subroutine which makes use of stack pointer or return value access. 
##      - Will print an array after each swap, showing the progress of the sort
##
##		$t0 - point to array elements in turn
##		$t1 - contains count of elements
##      $t2 - contains min
##		$t3 - contains max
##
##      $t7 - sorted start
##      $t8 - sorted end 
##

#################################################
#                                               #
#               text segment                    #
#                                               #
#################################################

        .text
        .globl __start
__start:           	   	# execution starts here

	lw $t1,count		# $t1 is the counter and contains array size now.
	la $t0,array		# $t0 contains base address of the array.
			        	# $t0 will point to the array elements.

	la $t2, ($t0)		# $t2 is element we're looking at

    la $t7, array       # start of sorted sub-array
    la $t8, array       # end of sorted sub-array



mainLoop:
	add $t1,$t1,-1		# $t1, the counter is updated.
    beq $t1, $zero, sorted
	add $t0,$t0,4		# $t0 is updated and points to a[1].

    la $a0, array
    lw $a1, 4($t0)
    addi $a1, $t0, 0

    jal sortTwoElements

    jal printArray

	la $a0,endl		# syscal to print out
	li $v0,4		# a new line
	syscall 

    j mainLoop

sorted:
    jal printArray
	li $v0,10		# Exit
	syscall			# Bye!

sortTwoElements: # temp registers 5-7 are free 
    # a0 = address of first  element 
    # a1 = address of second element
    # t3 = absolute of first element
    # t4 = absolute of second element
    
    lw $t3, 0($a0)
    lw $t4, 0($a1)

    bgt $t3, $0, firstIsNotNegative
    sub $t3, $zero, $t3
    j FirstAbsoluteFound

firstIsNotNegative:
    addi $t3, $a0, 0

FirstAbsoluteFound:
    bgt $t4, $zero, secondIsNotNegative # checking if second is negative
    sub $t4, $zero, $t4 # if it is, we make it positive and put it in t4
    j SecondAbsoluteFound

secondIsNotNegative: 
    addi $t4, $a1, 0

SecondAbsoluteFound:
    bgt $t3, $t4, swap
areInOrder:
    jr $ra

swap:
    lw $t8, ($a0)
    sw $a0, ($s1)
    sw $a1, ($t8)

    j areInOrder

printArray:
    la $t5, array
    lw $t6, count 
printLoop:
    lw $a0, ($t5)
    li $v0, 1
    syscall 

    la $a0, space
    li $v0, 4
    syscall

    addi $t6, $t6, -1
    addi $t5, $t5, 4
    bne $t6, $zero, printLoop
    jr $ra 

#################################################
#                                               #
#               data segment                    #
#                                               #
#################################################

        .data
	array:	.word -4,3, -12, 7, 20 # just trying to test with 5 elements
	count:	.word 5
	endl:	.asciiz "\n"	   
    space:  .asciiz " "

##
## 	end of file insertion_sort.s