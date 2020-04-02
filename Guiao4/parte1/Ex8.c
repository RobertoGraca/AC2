#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void display(char seg){
    unsigned int i,comp = 0,seg2 = seg;
    for(i=0;i<8;i++){
        comp = seg2 & 0x80;
        switch(i){
            case 0: if(comp==0x80)LATBbits.LATB8  = 1;break;
            case 1: if(comp==0x80)LATBbits.LATB9  = 1;break;
            case 2: if(comp==0x80)LATBbits.LATB10 = 1;break;
            case 3: if(comp==0x80)LATBbits.LATB11 = 1;break;
            case 4: if(comp==0x80)LATBbits.LATB12 = 1;break;
            case 5: if(comp==0x80)LATBbits.LATB13 = 1;break;
            case 6: if(comp==0x80)LATBbits.LATB14 = 1;break;
            case 7: if(comp==0x80)LATBbits.LATB15 = 1;break;
            default : break;
        }
        seg2 = seg2 << 1;
    }
}

int main(void){
    unsigned int num;
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                                          0x66, 0x6D, 0x7D, 0x07, 
                                          0x7F, 0x6F, 0x77, 0x7C, 
                                          0x39, 0x5E, 0x79, 0x71};
    
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    LATB = LATB & 0x00FF;
    TRISB = TRISB | 0x000F;
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    while(1){
        num = PORTB & 0x000F;
        LATB = LATB & 0x00FF;
        display(display7Scodes[num]);
        delay(500);
    }
    return 0;
}
