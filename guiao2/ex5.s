        .equ read_core_timer,11
        .equ reset_core_timer,12
        .equ print_int,6
        .equ print_str,8
        .equ put_char,3
        .equ inkey,1
        .data
        .text
        .globl main

main:   addiu $sp,$sp,-4
        sw $ra,0($sp)
        li $s0,0
        li $s1,0
        li $s3,0
        li $s2,0
        li $s4,0
        li $s5,100


while1: li $t0,5
        sll $t0,$t0,16
        ori $t0,$t0,10

        li $v0,inkey
        syscall

        beq $v0,'n',n
        beq $v0,'a',a
        beq $v0,'s',s
        j print

n:      li $s5,100
        j print
a:      div $s5,$s5,2
        j print
s:      li $v0,inkey
        syscall
        beq $v0,'r',while1
        j s



print:  li $v0,print_int #print 1st number
        move $a0,$s0
        move $a1,$t0
        syscall

        li $v0,put_char
        la $a0,'\t'
        syscall

        li $v0,print_int #print 2nd number
        move $a0,$s1
        move $a1,$t0
        syscall

        li $v0,put_char
        la $a0,'\t'
        syscall

        li $v0,print_int #print 3rd number
        move $a0,$s2
        move $a1,$t0
        syscall

        addi $s0,$s0,1

        bne $s3,5,cnt5
        li $s3,0
        addi $s1,$s1,1
cnt5:   addi $s3,$s3,1

        bne $s4,10,cnt10
        li $s4,0
        addi $s2,$s2,1
cnt10:  addi $s4,$s4,1

        li $v0,put_char
        la $a0,'\r'
        syscall

        move $a0,$s5
        jal delay
        j while1


        lw $ra,0($sp)
        addiu $sp,$sp,4



delay:  addiu $sp,$sp,-8
        sw $ra,0($sp)
        sw $s0,4($sp)
        move $s0,$a0
for:    ble $s0,0,d_end
        li $v0,reset_core_timer
        syscall
while:  li $v0,read_core_timer
        syscall
        blt $v0,20000,while
        addi $s0,$s0,-1
        j for
d_end:  lw $ra,0($sp)
        lw $s0,4($sp)
        addiu $sp,$sp,8
        jr $ra