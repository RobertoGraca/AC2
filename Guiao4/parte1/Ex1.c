#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void){
    LATDbits.LATD0 = 0;

    TRISDbits.TRISD0 = 0;
    while(1){
        delay(1000);
        LATDbits.LATD0 = !LATDbits.LATD0;
    }
    return 0;
}