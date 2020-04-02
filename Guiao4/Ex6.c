#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void display(int seg){
    unsigned int i,comp = 0;
    for(i=0;i<8;i++){
        comp = seg & 0x8000;
        switch(i){
            case 0: if(comp==0x8000)LATBbits.LATB8  = 1;break;
            case 1: if(comp==0x8000)LATBbits.LATB9  = 1;break;
            case 2: if(comp==0x8000)LATBbits.LATB10 = 1;break;
            case 3: if(comp==0x8000)LATBbits.LATB11 = 1;break;
            case 4: if(comp==0x8000)LATBbits.LATB12 = 1;break;
            case 5: if(comp==0x8000)LATBbits.LATB13 = 1;break;
            case 6: if(comp==0x8000)LATBbits.LATB14 = 1;break;
            case 7: if(comp==0x8000)LATBbits.LATB15 = 1;break;
            default : break;
        }
        comp = comp << 1;
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
    LATB = LATB & 0xFF00;
    TRISB = TRISB & 0xFF00;
    segment = 0xFFFF;

    while(1){
        for(i=0;i<=15;i++){
            LATB = LATB & 0xFF00;
            for(j=0;j<=7,j++){
                display(display7Scodes[i]);
            }
            delay(500);   
        }
    }
    return 0;
}
