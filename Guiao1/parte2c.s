        .data
str1:   .asciiz "\nIntroduza um numero (sinal e módulo): ";
str2:   .asciiz "\nValor lido, em base 2: ";
str3:   .asciiz "\nValor lido, em base 16: ";
str4:   .asciiz "\nValor lido, em base 10 (unsigned): ";
str5:   .asciiz "\nValor lido, em base 10 (signed): ";
        .text
        .globl main
main:   la $a0,str1
        li $v0,8
        syscall
        li $v0,5
        syscall
        move $t0,$v0
        la $a0,str2
        li $v0,8
        syscall
        move $a0,$t0
        li $a1,2
        li $v0,6
        syscall
        la $a0,str3
        li $v0,8
        syscall
        move $a0,$t0
        li $a1,16
        li $v0,6
        syscall
        la $a0,str4
        li $v0,8
        syscall
        move $a0,$t0
        li $a1,10
        li $v0,6
        syscall
        la $a0,str5
        li $v0,8
        syscall
        move $a0,$t0
        li $v0,7
        syscall
        j main
        li $v0,0
        jr $ra