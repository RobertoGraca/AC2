#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void display(int seg){
    switch(seg){
        case 1: LATBbits.LATB8  = 1;break;
        case 2: LATBbits.LATB9  = 1;break;
        case 3: LATBbits.LATB10 = 1;break;
        case 4: LATBbits.LATB11 = 1;break;
        case 5: LATBbits.LATB12 = 1;break;
        case 6: LATBbits.LATB13 = 1;break;
        case 7: LATBbits.LATB14 = 1;break;
        case 8: LATBbits.LATB15 = 1;break;
        default : break;
    }
}

int main(void){
    unsigned char segment;
    unsigned int i;
    
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    TRISB = TRISB & 0xFF00;
    TRISD = TRISD & 0xF9FF;
    LATD = (LATD & 0xFF9F) | 0x0400;

    while(1){
        LATD = LATD ^ 0x0060;
        segment = 1;
        for(i=0;i<=7;i++){
            display(segment);
            delay(500);
            segment++;    
        }
    }
    return 0;
}
