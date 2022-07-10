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
        LATBbits.LATB15 = 1;
    }  else{
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x00FF) | (display7Scodes[toBcd(value) & 0x0F] << 8);
    }
    displayFlag = !displayFlag;
}


int main(void){
    adc_init();
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
    LATB = LATB & 0x00FF;
    LATD = (LATD & 0xFF9F) | 0x0020;
    int value = 0,i = 0;

    while(1){
        if(i++ % 25 == 0){
            AD1CON1bits.ASAM = 1;
            while(IFS1bits.AD1IF == 0);
            int* p = (int*) (&ADC1BUF0);
            value = 0;
            for(;p<=(int*)(&ADC1BUFF);p+=4){
                value += *p;
            }
            value = value/4;
            value = ((value*33)+511)/1023;
            IFS1bits.AD1IF = 0;
        }
        
        int esq = value / 10;
        int dir = value % 10;
        printInt(esq,10);
        putChar('.');
        printInt(dir,10);
        putChar('\r');
        send2displays(value);
    }
    return 0;
}