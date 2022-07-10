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
        andi $t1,$t1,0xfffe
        sw $t1,trise($t0)

        lui $t0,sfr_base_hi
        lw $t1, trisb($t0)
        ori $t1,$t1,1
        sw $t1,trisb($t0)

while:  lui $t0,sfr_base_hi
        lw $t1,portb($t0)
        sw $t1,late($t0)
        j while

        li $v0,0
        jr $ra

