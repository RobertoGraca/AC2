#include <detpic32.h>

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void adc_init(){
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;
}

int main(void){
    adc_init();
    TRISE = TRISE & 0xFFFE;

    int aux;

    while(1){
        LATE = LATE | 0x0001;
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);
        LATE = LATE & 0xFFFE;
        aux = ADC1BUF0;
        IFS1bits.AD1IF = 0;
    }
}
