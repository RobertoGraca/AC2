#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void){
    unsigned int cont = 0;
    
    TRISE = TRISE & 0xFFF0;

    while(1){
        delay(250);
        LATE = cont & 0x000F;
        cont++;
    }
    return 0;
}