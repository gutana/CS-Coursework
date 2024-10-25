##
## 	The program --- num-vowel.s will count number of vowels in a string.
##
##		- It will ask the user for a string
##		  
##		- a loop calls a procedure ("vowelp") to check if a letter is a vowel.
##		- It will return a value to the calling function, 
##		  0 means no, and 1 means yes,
##		- and then print a message from the calling program.
##		
##		$v0 - used for syscalls
##		    - holds 0 or 1 after the call to "vowelp"
##		$t0 - holds the returned value from the procedure call
## 		$a0 - points to strings to be printed 
##		    - also used to pass a letter to "vowelp" 
## note: This program will consider anything that isn't a vowel a 'non-vowel'. 

#################################################
#						#
#               text segment			#
#						#
#################################################

	.text
	.globl __start
__start:                # execution starts here

	la $a0,prompt1  # print prompt on terminal
	li $v0,4        # system call to print
	syscall         # out a string

	la $a0,string
	li $a1,80
	li $v0, 8	# syscall 8 reads string/letter
	syscall

	la $a0, string	# going to pass the string into the loop
	jal vowelLoop	# procedure call

	move $t0,$v0	# save the value returned from the procedure call
	move $t1,$v1	# save the value returned from the procedure call

	la $a0,ans0	# syscall to print out
	li $v0,4	
	syscall 

    addi $a0, $t0, 0
    li $v0, 1             # print vowels 
    syscall 

    la $a0,ans1	# syscall to print
	li $v0,4	
	syscall 

    addi, $a0, $t1, 0      # print non-vowels
    li $v0, 1
    syscall

	la $a0,ans2	# syscall to print
	li $v0,4	
	syscall 

	j End

End:	
    la $a0,endl	# syscall to print out
	li $v0,4	# a new line
	syscall 

	li $v0,10
	syscall		# Bye!

##############################################################
## Define the procedure vowelp
##############################################################

vowelLoop: 
    li $t1, 0
    move $t3, $a0
vowelLoopInner:
    lb $t4, ($t3)
    beq $t4, 10, endLoop

    move $a0, $t4
    j vowelp
    addi $s2, $v0, 0
    addi $s3, $v1, 0 

    addi $t3, $t3, 8
    j vowelLoopInner

endLoop: 
    addi $v0, $s2, 0
    addi $v1, $s3, 0

    jr $ra

vowelp:
	beq $a0,'a',yes
	beq $a0,'A',yes
	beq $a0,'e',yes
	beq $a0,'E',yes
	beq $a0,'i',yes
	beq $a0,'I',yes
	beq $a0,'o',yes
	beq $a0,'O',yes
	beq $a0,'u',yes
	beq $a0,'U',yes
	li $v1, 1	# assign 1 to $v1 
	jr $ra
yes:	
    li $v0, 1	# assign 1 to $v0
	jr $ra
	

#################################################
#						#
#               data segment			#
#						#
#################################################

        .data
	string:		.space 80
	prompt1:	.asciiz "Enter a string: "
	
	ans0:	.asciiz "The string contains "
	ans1:		.asciiz " vowels and "
    ans2:       .asciiz " non-vowels."
	endl:		.asciiz "\n"	   

##
## 	end of file num-vowel.s