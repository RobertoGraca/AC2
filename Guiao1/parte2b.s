        .data
        .text
        .globl main
main:   nop
while:  li $v0,1
        syscall
        beq $v0,$0,while
        move $t0,$v0
        beq $t0,'\n',endif
        move $a0,$t0
        li $v0,3
        syscall
endif:  bne $t0,'\n',while
        li $v0,0
        jr $ra