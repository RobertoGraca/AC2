#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void){
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    T3CONbits.TON = 1;
    
    while(1){
        while(IFS0.T3IF == 0);
        IFS0bits.T3IF = 0;
        putChar('.');
    }
    return 0;
}




