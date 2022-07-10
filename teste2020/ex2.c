#include <detpic32.h>

unsigned volatile int voltage = 0;

static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                                          0x66, 0x6D, 0x7D, 0x07, 
                                          0x7F, 0x6F, 0x77, 0x7C, 
                                          0x39, 0x5E, 0x79, 0x71};

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void config_adc(){
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 4 - 1;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;

    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
}

void conf_timer2(){
    T2CONbits.TCKPS = 2;
    PR2 = 49999;
    TMR2 = 0;
    T2CONbits.TON = 1;

    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
}

int toBCD(int value){
    return ((value/10)<<4) + (value%10);
}

void send2displays(unsigned char value){
    static int flag = 0;

    if(!flag){
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x00FF) | display7Scodes[toBCD(value) >> 4] << 8;
    }else{
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x00FF) | display7Scodes[toBCD(value) & 0x0F] << 8;
    }
    flag = !flag;
}

void _int_(27) isr_adc(void){
    int* p = (int*)(&ADC1BUF0);
    int v = 0;
    for(;p<(int*)(&ADC1BUFF);p+=4){
        v+=*p;
    }
    v /= 4;
    int ledVal = ((v*8)+511)/1023;
    printInt(ledVal,8);
    putChar('\t');
    voltage = ((v*16)+511)/1023;
    printInt(voltage,16);
    putChar('\n');
    int num = 1;
    if(ledVal >= 1 && ledVal <= 8){
        int i;
        for(i=1;i<=ledVal;i++){
            num*=2;
        }
    }
    num--;
    printInt(num,2);
    LATE = (LATE & 0xFF00) | num;
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_t2(){
    send2displays(voltage);
    IFS0bits.T2IF = 0;
}

int main(void){
    config_adc();
    conf_timer2();
    EnableInterrupts();

    TRISE = TRISE & 0xFF00;
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    AD1CON1bits.ASAM = 1;

    while(1){delay(100);}


    return 0;
}
