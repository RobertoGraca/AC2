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
    unsigned int i,j;
    int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                            0x66, 0x6D, 0x7D, 0x07, 
                            0x7F, 0x6F, 0x77, 0x7C, 
                            0x39, 0x5E, 0x79, 0x71};
    
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    TRISB = TRISB & 0xFF00;
    segment = 0xFFFF;

    while(1){
        for(i=0;i<=15;i++){
            LATB = LATB & 0xFF00;
            segment = 0xFFFF;
            for(j=0;j<=7,j++){
                display(display7Scodes[i] & segment);
                segment--;
            }
            delay(500);   
        }
    }
    return 0;
}
