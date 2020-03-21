        .data
        .equ SFR_BASE_HI,0xBF88
        .equ TRISE,0x6100
        .equ PORTE,0x6110
        .equ LATE,0x6120
        .equ RESET_CORE_TIMER,12
        .equ READ_CORE_TIMER,11
        .equ K,20000
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
        andi $s2,$s2,0xFFFE
        sw $s2,TRISE($s1)

true:  lw $t4,LATE($s1)
        srl $t4,$t4,1
        sll $t4,$t4,1
        or $t4,$t4,$s0
        sw $t4,LATE($s1)
        li $a0,500
        jal delay
        xori $s0,$s0,0x0001
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
