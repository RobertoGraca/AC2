        .equ sfr_base_hi,0xbf88
        .equ trise,0x6100
        .equ late,0x6120
        .equ get_char,2
        .equ reset_core_timer,12
        .equ read_core_timer,11
        .data
        .text
        .globl main

main:   lui $t0,sfr_base_hi
        lw $t1,trise($t0)
        andi $t1,$t1,0xFFE0
        sw $t1,trise($t0)

while:	li $v0,get_char
        syscall
        beq $v0,'0',zero
        beq $v0,'1',one
        beq $v0,'2',two
        beq $v0,'3',three
        
        lui $t0,sfr_base_hi
        lw $t1,late($t0)
        ori $t1,$t1,0x0010
        sw $t1,late($t0)

        li $a0,2000
        jal delay
        
        lui $t0,sfr_base_hi
        lw $t1,late($t0)
        andi $t1,$t1,0xFFE0
        sw $t1,late($t0)
        j while

zero:	lui $t0,sfr_base_hi
        lw $t1,late($t0)
        ori $t1,$t1,0x0001
        sw $t1,late($t0)
        j while

one: 	lui $t0,sfr_base_hi
        lw $t1,late($t0)
        ori $t1,$t1,0x0002
        sw $t1,late($t0)
        j while

two:	lui $t0,sfr_base_hi
        lw $t1,late($t0)
        ori $t1,$t1,0x0004
        sw $t1,late($t0)
        j while

three:	lui $t0,sfr_base_hi
        lw $t1,late($t0)
        ori $t1,$t1,0x0008
        sw $t1,late($t0)
        j while

        li $v0,0
        jr $ra

delay:	addiu $sp,$sp,-4
	sw $ra,0($sp)	
for:	ble $a0,0,end
        li $v0,reset_core_timer
        syscall
read:	li $v0,read_core_timer
        syscall
        blt $v0,20000,read
        addi $a0,$a0,-1
        j for
end:	lw $ra,0($sp)
        addiu $sp,$sp,4
	jr $ra