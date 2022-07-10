#include <detpic32.h>

static const char display7SegCodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value){
    static char flag = 1;

    if(flag){
        LATD = (LATD & 0xFF9F) | 0x0040; 
        LATB = (LATB & 0x00FF) | (display7SegCodes[value >> 4] << 8);
    }else{
        LATD = (LATD & 0xFF9F) | 0x0020; 
        LATB = (LATB & 0x00FF) | (display7SegCodes[value & 0x0F] << 8);
    }
    flag = !flag;
}

int main(void){
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    LATB = LATB & 0x00FF;

    char p = 0,c;
    int i;

    while(1){
        c = inkey();

        if(c != 0)p = c;
        

        switch(p){
            case '0':
                send2displays(0x00);
                delay(10);
                break;
            case '1':
                send2displays(0x01);
                delay(10);
                break;
            case '2':
                send2displays(0x02);
                delay(10);
                break;
            case '3':
                send2displays(0x03);
                delay(10);
                break;
            case 0:
                continue;
            default:
                i = 0;
                do{
                    send2displays(0xFF);
                    delay(10);
                }while(++i < 100);
                LATB = LATB & 0x00FF;
                p = 0;
                break;
        }
    }



    return 0;
}