#include <detpic32.h>

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void){
    TRISE = TRISE & 0xFFF0;

    int cont = 0;

    while(1){
        LATE = LATE & 0xFFF0;
        LATE = LATE | cont;
        cont++;
        if(cont>15)cont = 0;
        delay(250);
    }
    return 0;
}