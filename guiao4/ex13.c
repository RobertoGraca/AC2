#include <detpic32.h>

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void){
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    LATB = LATB & 0x00FF;
    LATD = (LATD & 0xFF9F) | 0x0020;

    while(1){
        char c = getChar();

        LATB = LATB & 0x00FF;

        switch(c){
            case 'a' | 'A': LATBbits.LATB8 = 1;break;
            case 'b' | 'B': LATBbits.LATB9 = 1;break;
            case 'c' | 'C': LATBbits.LATB10 = 1;break;
            case 'd' | 'D': LATBbits.LATB11 = 1;break;
            case 'e' | 'E': LATBbits.LATB12 = 1;break;
            case 'f' | 'F': LATBbits.LATB13 = 1;break;
            case 'g' | 'G': LATBbits.LATB14 = 1;break;
            //default: LATB = LATB & 0x00FF;break;
        }

    }
    return 0;
}