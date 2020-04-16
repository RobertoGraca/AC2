#include <detpic32.h>

static int dh, dl;

void delay(unsigned int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void send2displays(){
    unsigned char value;
    unsigned int i=0;
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                                          0x66, 0x6D, 0x7D, 0x07, 
                                          0x7F, 0x6F, 0x77, 0x7C, 
                                          0x39, 0x5E, 0x79, 0x71};
    while(i<1000000){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[(value >> 4)] << 8);
        delay(20);

        LATD = LATD ^ 0x0060;
        LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x0F] << 8);
        delay(20);
        i++;
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
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    send2displays();

    return 0;
}
