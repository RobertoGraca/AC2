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
    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x00FF;

    LATB = LATB & 0x00FF;

    int counter = 19,i,freq;

    while(1){
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);

        freq = ADC1BUF0;
        freq = 1 + (freq/255);
        freq = 1/freq;
        freq = freq*1000/20;

        printInt(freq,10);

        i = 0;
        do{
            send2displays(counter);
            delay(20);
        }while(++i < freq);
        IFS1bits.AD1IF = 0;
        counter--;
        if(counter<0)counter = 19;

    }





    return 0;
}




