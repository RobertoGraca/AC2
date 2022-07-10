#include <detpic32.h>

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                                          0x66, 0x6D, 0x7D, 0x07, 
                                          0x7F, 0x6F, 0x77, 0x7C, 
                                          0x39, 0x5E, 0x79, 0x71};

void send2displays(unsigned char value){
    int cont = 0;
    
    while(cont<10){

        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x00FF) | (display7Scodes[value >> 4] << 8);
        delay(10);

        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x0F] << 8);
        delay(10);

        cont++;
    }
}

int main(void){
    TRISB = (TRISB & 0x00FF) | 0x000F ;
    TRISD = TRISD & 0xFF9F;

    LATB = LATB & 0x00FF;
    LATD = (LATD & 0xFF9F) | 0x0020;

    int cont = 0;

    while(1){
        send2displays(cont++);
        cont = cont%256;
    }
}
                                    