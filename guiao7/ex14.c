#include <detpic32.h>

void timer3_conf(){
    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;

   /* OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 0;
    OC1RS = ;
    OC1CONbits.ON = 1;*/

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;
}

void timer1_conf(){
    T1CONbits.TCKPS = 7;
    PR1 = 39062;
    TMR1 = 0;
    T1CONbits.TON = 1;

   /* OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 0;
    OC1RS = ;
    OC1CONbits.ON = 1;*/

    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(){
    putChar('3');
    IFS0bits.T3IF = 0;
}

void _int_(4) isr_T1(){
    putChar('1');
    IFS0bits.T1IF = 0;
}




int main(void){
    timer3_conf();
    timer1_conf();
    EnableInterrupts();
    while(1);
    return 0;
}