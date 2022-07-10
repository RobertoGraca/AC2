.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120
.equ getChar, 2
.equ readCoreTimer, 11
.equ resetCoreTimer, 12
.data
.text
.globl main

main:
lui $t1, SFR_BASE_HI
lW $t2, TRISE($t1)
andi $t2, $t2, 0xFFE0
sw $t2, TRISE($t1)
lw $t2, LATE($t1)
andi $t2, $t2, 0xFFE0
sw $t2, LATE($t1)

while:
li $v0, getChar
syscall
beq $v0, '0', zero
beq $v0, '1', um
beq $v0, '2', dois
beq $v0, '3', tres
ori $t2,$t2, 0x0010
sw $t2, LATE($t1)
li $a0, 2000
jal delay
andi $t2, $t2, 0xFFE0
sw $t2, LATE($t1)
j next

zero:
ori $t2, $t2, 0x0001
j next

um:
ori $t2, $t2, 0x0002
j next

dois:
ori $t2, $t2, 0x0004
j next

tres:
ori $t2, $t2, 0x0008
j next

next:
sw $t2, LATE($t1)
j while

end: 
li $v0, 0
jr $ra

delay:
addiu $sp, $sp, -4
sw $ra, 0($sp)
move $t0, $a0

for:
ble $t0, 0, enddelay
li $v0, resetCoreTimer
syscall

while1:
li $v0, readCoreTimer
syscall
blt $v0, 20000, while1
addi $t0, $t0, -1
j for

enddelay:
lw $ra, 0($sp)
addiu $sp, $sp, 4
jr $ra


