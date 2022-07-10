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
}

void main(void){
    timer3_conf();
    while(1){
        while(IFS0bits.T3IF == 0);
        IFS0bits.T3IF = 0;
        putChar('.');
    }
}