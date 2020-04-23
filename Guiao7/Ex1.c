#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void){
    T3CONbits.TCKPS = 7;
    PR3 = 39063;
    TMR3 = 0;
    T2CONbits.TON = 1;
    
    while(1){
        while(IFS0.T3IF == 0);
        IFS0bits.T3IF = 0;
        putChar('.');
    }
    return 0;
}

2 = 20000000/(2^n * (PR3 + 1))
1 = 10000000/(2^n * (PR3 + 1))
10000000 = 256 * (PR3 + 1)
10000000/256 - 256/156 = PR3 
PR3 = 9999744/256


