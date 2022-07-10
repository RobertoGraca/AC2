        .data
        .equ read_int,5
        .equ print_int,6
        .equ print_int10,7
        .equ print_str,8
text:   .asciiz "\nIntroduza um numero:"
num2:   .asciiz "\nValor em base 2: "
num16:  .asciiz "\nValor em base 16: "
num10un:.asciiz "\nValor em base 10 unsigned: "
num10s: .asciiz "\nValor em base 10 signed: "
        .text
        .globl main

main:   li $v0,print_str
        la $a0,text
        syscall
        li $v0,read_int
        syscall
        move $s0,$v0


        # Base 2
        li $v0,print_str
        la $a0,num2
        syscall
        li $v0,print_int
        move $a0,$s0
        li $a1,2
        syscall


        # Base 16
        li $v0,print_str
        la $a0,num16
        syscall
        li $v0,print_int
        move $a0,$s0
        li $a1,16
        syscall


        # Base 10 Unisgned
        li $v0,print_str
        la $a0,num10un
        syscall
        li $v0,print_int
        move $a0,$s0
        li $a1,10
        syscall


        # Base 10 Signed
        li $v0,print_str
        la $a0,num10s
        syscall
        li $v0,print_int10
        move $a0,$s0
        syscall

        li $v0,0
        jr $ra




    