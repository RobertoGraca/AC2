    .data
    .equ print_int,6
    .equ read_core_timer,11
    .equ reset_core_timer,12
    .text
    .globl main

main:   li $t0,0
while1: li $v0,print_int
        move $a0,$t0
        li $a1,10
        syscall
        addi $t0,$t0,1
        li $a0,100
        jal delay
        j while1
        jr $ra

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
