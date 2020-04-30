#include <detpic32-h>

void _int_(12) isr_t3(void){
    putChar('.');
    IFS0bits.T3IF = 0;
}

int main(void){
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    
    
    IPC2bits.T3IP = 2;
    IEC0bits.T3IE = 1;

    T3CONbits.TON = 1;

    EnableInterrupts();
    
    while(1);

    return 0;
}