#include <detpic32.h>


static int dh, dl;

void delay(unsigned int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void send2displays( unsigned char value){
   
    unsigned int i=0;
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                                          0x66, 0x6D, 0x7D, 0x07, 
                                          0x7F, 0x6F, 0x77, 0x7C, 
                                          0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    int digit_low = value >> 4, digit_high = value & 0x0F;

    if(displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
        if(value%2 != 1)LATBbits.LATB15 = 1;
        delay(20);
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        if(value%2 != 0)LATBbits.LATB15 = 1;
        delay(20);
    }
    displayFlag = !displayFlag;
}

unsigned char toBCD(unsigned char value){
    return ((value/10)<<4)+(value%10);
}

void pisca(){
    int i=0;
    do{
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | 0x3F00;
        delay(500);
        LATB = LATB & 0x00FF;
        delay(500);
    }
    while(++i<5);
}

int main(void){
    unsigned int num;
    unsigned char value=0;
    
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    LATB = LATB & 0x00FF;
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
    
    num = 0;
    while(1){
        do{
            if(value==0)pisca();
            send2displays(toBCD(value));
            delay(10);
        }
        while(++i<100);
        value = (value+1)%60;
    }
    return 0;
}
