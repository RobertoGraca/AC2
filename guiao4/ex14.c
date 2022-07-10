#include <detpic32.h>

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void){
    unsigned char segment;
    int i;

    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    LATB = LATB & 0x00FF;
    LATD = (LATD & 0xFF9F) | 0x0020;

    while(1){
        LATD = LATD ^ 0x0060;
        segment = 1;
        for(i=0;i<7;i++){
            LATB = (LATB & 0x00FF) | (segment << 8);
            delay(500);
            segment = segment << 1;
        }
    }
    return 0;
}
