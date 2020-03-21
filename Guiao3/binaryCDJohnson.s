        .data
        .equ SFR_BASE_HI,0xBF88
        .equ TRISE,0x6100
        .equ PORTE,0x6110
        .equ LATE,0x6120
        .equ TRISB,0x6040
        .equ PORTB,0x6050
        .equ LATB,0x6060
        .equ RESET_CORE_TIMER,12
        .equ READ_CORE_TIMER,11
        .equ K,100000
        .text
        .globl main
main:   addiu $sp,$sp,-20
        sw $ra,0($sp)
        sw $s0,4($sp)
        sw $s1,8($sp)
        sw $s2,12($sp)
        sw $s3,16($sp)
        
        li $s0,0

        lui $s1,SFR_BASE_HI
        lw $s2,TRISE($s1)
        andi $s2,$s2,0xFFF0
        sw $s2,TRISE($s1)

        lw $s2,TRISB($s1)
        ori $s2,$s2,0x0008
        sw $s2,TRISB($s1)

true:   lw $t0,PORTB($s1)
        andi $t1,$t0,0x0008
        lw $t4,LATE($s1)
        srl $t4,$t4,4
        sll $t4,$t4,4
        or $t4,$t4,$s0
        sw $t4,LATE($s1)
        li $a0,500
        jal delay
        #xori $s0,$s0,0x0001
        bne $t1,0x0008,other
        andi $t0,$s0,0x08
        bne $t0,$0,right
        sll $s0,$s0,1
        addi $s0,$s0,1
        j r
right:  sll $s0,$s0,1
r:      or $t4,$t4,$s0
        sw $t4,LATE($s1)
        j true


        
other:  andi $t0,$s0,0x01
        bne $t0,$0,left
        srl $s0,$s0,1
        addi $s0,$s0,0x08
        j l
left:   srl $s0,$s0,1
l:      or $t4,$t4,$s0
        or $t4,$t4,$s0
        sw $t4,LATE($s1)
        j true
        lw $ra,0($sp)
        lw $s0,4($sp)
        lw $s1,8($sp)
        lw $s2,12($sp)
        lw $s3,16($sp)
        addiu $sp,$sp,20
        jr $ra


delay:  
while:  ble $a0,0,endw
        li $v0,RESET_CORE_TIMER
        syscall
while2: li $v0,READ_CORE_TIMER
        syscall
        bge $v0,K,endw2
        j while2
endw2:  sub $a0,$a0,1
        j while
endw:   jr $ra