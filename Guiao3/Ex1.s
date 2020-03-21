        .data
        .equ SFR_BASE_HI,0xBF88
        .equ TRISE,0x6100
        .equ PORTE,0x6110
        .equ LATE,0x6120
        .equ TRISB,0x6040
        .equ PORTB,0x6050
        .equ LATB,0x6060
        .text
        .globl main
main:   
        lui $t1,SFR_BASE_HI
        lw $t2,TRISE($t1)
        andi $t2,$t2,0xFFF0
        sw $t2,TRISE($t1)
        
        lw $t2,TRISB($t1)
        ori $t2,$t2,0x000F
        sw $t2,TRISB($t1)

wi:     bne $0,$0,w
        lw $t2,PORTB($t1)
        lw $t4,LATE($t1)
        andi $t3,$t2,0x000F 
        xori $t3,$t3,0x0006
        srl $t4,$t4,4
        sll $t4,$t4,4
        or $t4,$t4,$t3
zero:   sw $t4,LATE($t1)
        j wi
w:      jr $ra

