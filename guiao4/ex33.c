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

unsigned char toBcd(unsigned char value){
    return ((value/10) << 4) + (value%10);
}

void send2displays(unsigned char value){
    static char displayFlag = 0;
    
    if(!displayFlag){
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x00FF) | (display7Scodes[toBcd(value) >> 4] << 8);
    }  else{
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x00FF) | (display7Scodes[toBcd(value) & 0x0F] << 8);
    }
    displayFlag = !displayFlag;
}

int main(void){
    TRISB = (TRISB & 0x00FF) | 0x000F ;
    TRISD = TRISD & 0xFF9F;

    LATB = LATB & 0x00FF;
    LATD = (LATD & 0xFF9F) | 0x0020;

    int cont = 0;
    int i;
    int j;

    while(1){
        i = 0;
        do{
            send2displays(cont);
            delay(10);
        }while(++i < 100);
        cont++;
        cont = cont%60;
        if(cont==0){
            LATD = (LATD & 0xFF9F) | 0x0060;
            for(j=0;j<5;j++){
                LATB = (LATB & 0x00FF) | (display7Scodes[0] << 8);
                delay(500);
                LATB = (LATB & 0x00FF);
                delay(500);
            }
        }
    }
}
                                    