        .data
        .text
        .globl main
main:   nop
while:  li $v0,2
        syscall
        move $t0,$v0
        beq $t0,'\0',endif
        move $a0,$t0
        li $v0,3
        syscall
endif:  bne $t0,'\0',while
        li $v0,0
        jr $ra
