        .data
        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,7
        .text
        .globl main
main:   li $t0,0
while:  li $v0,READ_CORE_TIMER
        syscall
        blt $v0,200000,while            # 10Hz = 2.000.000, 5Hz = 4.000.000, 1Hz = 20.000.000
        li $v0,RESET_CORE_TIMER
        syscall
        li $a0,' '
        li $v0,PUT_CHAR
        syscall
        addi $t0,$t0,1
        move $a0,$t0
        li $a1,10
        li $v0,PRINT_INT
        syscall
        j while
        jr $ra
        