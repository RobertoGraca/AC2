#include <detpic32.h>

void _int_(4) isr_t1(void){
    putChar('1');
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_t3(void){
    putChar('3');
    IFS0bits.T3IF = 0;
}

int main(void){
    T3CONbits.TCKPS = 5;
    PR3 = 62499;            //T3 = 10 Hz, para 20 Hz -> PR3 = 31249
    TMR3 = 0;
    
    
    IPC2bits.T3IP = 2;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;

    T1CONbits.TCKPS = 7;
    PR1 = 39062;            //T1 = 2 Hz
    TMR1 = 0;
    
    
    IPC2bits.T1IP = 2;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    T3CONbits.TON = 1;

    T1CONbits.TON = 1;

    EnableInterrupts();

    while(1);

    return 0;
}