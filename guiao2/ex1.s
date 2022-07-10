        .equ read_core_timer,11
        .equ reset_core_timer,12
        .equ put_char,3
        .equ get_char,2
        .equ print_int,6
        .data
        .text
        .globl main

main:   li $t0,0
while:  li $v0,read_core_timer
        syscall
        blt $v0,200000,while
        li $v0,reset_core_timer
        syscall
        addi $t0,$t0,1
        li $t1,4
        sll $t1,$t1,16
        ori $t1,$t1,10
        li $v0,print_int
        move $a0,$t0
        li $a1,$t1
        syscall
        li $v0,put_char
        la $a0,'\r'
        syscall
        j while
        li $v0,0
        jr $ra
