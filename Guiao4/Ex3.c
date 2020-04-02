#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void){
    
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
    LATB = LATB & 0x00FF;
    LATD = LATD & 0xFF9F;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    while(1){
        LATB = LATB & 0x00FF;
        char c = getChar();
        switch(c){
            case 'a' | 'A': LATBbits.LATB8  = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'b' | 'B': LATBbits.LATB9  = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'c' | 'C': LATBbits.LATB10 = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'd' | 'D': LATBbits.LATB11 = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'e' | 'E': LATBbits.LATB12 = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'f' | 'F': LATBbits.LATB13 = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'g' | 'G': LATBbits.LATB14 = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case '.':       LATBbits.LATB15 = 1;//((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
        }
    }
    return 0;
}
