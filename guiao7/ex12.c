#include <detpic32.h>


void timer3_conf(){
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    T3CONbits.TON = 1;

   /* OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 0;
    OC1RS = ;
    OC1CONbits.ON = 1;*/

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T2IF = 0;
}

void _int_(12) isr_T3(){
    putChar('.');
    IFS0bits.T3IF = 0;
}

void main(void){
    timer3_conf();
    EnableInterrupts();
    while(1);
}