#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void){
    
    TRISB = TRISB & 0xFF00;
    TRISD = TRISD & 0xF9FF;

    while(1){
        char c = getChar();
        switch(c){
            case 'a' | 'A': LATBbits.LATB8  = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'b' | 'B': LATBbits.LATB9  = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'c' | 'C': LATBbits.LATB10 = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'd' | 'D': LATBbits.LATB11 = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'e' | 'E': LATBbits.LATB12 = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'f' | 'F': LATBbits.LATB13 = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case 'g' | 'G': LATBbits.LATB14 = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            case '.':       LATBbits.LATB15 = ((LATDbits.LATD5==1) & (LATDbits.LATD6==0))?1:!((LATDbits.LATD5==0) & (LATDbits.LATD6==1));
                            break;
            default: continue;
        }
    }
    return 0;
}
