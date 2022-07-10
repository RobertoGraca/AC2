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
    AD1CON2bits.SMPI = 4 - 1;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;
}

int main(void){
    adc_init();

    while(1){
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);
        int* p = (int*) (&ADC1BUF0);
        int value = 0;
        for(;p<=(int*)(&ADC1BUFF);p+=4){
            value += *p;
        }
        value = value/4;
        value = ((value*33)+511)/1023;
        int esq = value / 10;
        int dir = value % 10;
        printInt(esq,10);
        putChar(',');
        printInt(dir,10);
        delay(1000);
        putChar('\r');
        IFS1bits.AD1IF = 0;
    }
    return 0;
}