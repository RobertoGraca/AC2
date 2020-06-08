#include <detpic32.h>

void configureInputs(){
    T3CONbits.TCKPS = 5;
    PR3 = 6249;
    TMR3 = 0;

    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 1;
    OC1RS = 1562.5;
    OC1CONbits.ON = 1;

    T3CONbits.TON = 1;
}

int main(void){
    configureInputs();

    

    return 0;
}