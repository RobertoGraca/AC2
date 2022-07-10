        .equ read_core_timer,11
        .equ reset_core_timer,12
        .equ sfr_base_hi,0xbf88
        .equ trise,0x6100
        .equ porte,0x6110
        .equ late,0x6120
        .equ trisb,0x6040
        .equ portb,0x6050
        .equ latb,0x6060
        .equ print_int,6
        .data
        .text
        .globl main

main:   lui $t0,sfr_base_hi
        lw $t1, trise($t0)
        andi $t1,$t1,0xfff0
        sw $t1,trise($t0)

        lui $t0,sfr_base_hi
        lw $t1, trisb($t0)
        ori $t1,$t1,1
        sw $t1,trisb($t0)

        li $t2,0
        sw $t2,late($t0)
        li $a0,500
        jal delay

while1: andi $t3,$t2,8
        sll $t2,$t2,1
        beq $t3,8,cont
        addi $t2,$t2,1


cont:   sw $t2,late($t0)
        li $a0,666
        jal delay
        j while1

        li $v0,0
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