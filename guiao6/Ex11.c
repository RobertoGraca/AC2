#include <detpic32.h>

void init_adc(){
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;
}

void init_interrupt(){
    IPC6bits.AD1IP = 2;

    IFS1bits.AD1IF = 0;

    IEC1bits.AD1IE = 1;

    EnableInterrupts();
}

void _int_(27) isr_adc(void){
    printInt(ADC1BUF0, 10 | 4 << 16);
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
}

int main(void){
    init_adc();
    init_interrupt();
    
    AD1CON1bits.ASAM = 1;
    while(1){}
    return 0;
}
