#include <detpic32.h>

volatile unsigned char voltage = 0;

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

unsigned char toBcd(unsigned char value){
    return ((value/10) << 4) + (value%10);
}

void send2displays(unsigned char value){
    static int flag = 0;

    if(!flag){
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x00FF) | (display7Scodes[value >> 4] << 8);
    }else{
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x0F] << 8);
    }
    flag = !flag;
}

void init_adc(){
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 8 - 1;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;
}

void init_interrupt(){
    IPC6bits.AD1IP = 2;

    IFS1bits.AD1IF = 0;

    IEC1bits.AD1IE = 1;
}

void _int_(27) isr_adc(void){
    int* p = (int*) (&ADC1BUF0);
    int v = 0;
    for(;p<(int*)(&ADC1BUF8);p+=4){
        v+=*p;
    }
    v = v/8;

    if(PORTBbits.RB0 == 1 && PORTBbits.RB1 == 0){
        putChar('.');
    }else{
        voltage = ((v*33)+511)/1023;
    }
    

    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
}

void timer3_conf(){
    T3CONbits.TCKPS = 7;
    PR3 = 19531;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;
}

void timer1_conf(){
    T1CONbits.TCKPS = 2;
    PR1 = 49999;
    TMR1 = 0;
    T1CONbits.TON = 1;

    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(){
    send2displays(toBcd(voltage));
    IFS0bits.T3IF = 0;
}

void _int_(4) isr_T1(){
    AD1CON1bits.ASAM = 1;
    IFS0bits.T1IF = 0;
}

int main(void){
    init_adc();
    init_interrupt();
    timer1_conf();
    timer3_conf();
    EnableInterrupts();

    TRISB = (TRISB & (0x00FF)) | 0x0003;
    TRISD = TRISD & 0xFF9F;

    while(1){
    }
    return 0;
}
