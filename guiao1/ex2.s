    .data
    .equ getChar,2
    .equ putChar,3
    .text
    .globl main

main:   li $v0,getChar
        syscall
        beq $v0,'\n',end
        addi $a0,$v0,1
        li $v0,putChar
        syscall
        j main
end:    li $v0,0
        jr $ra
