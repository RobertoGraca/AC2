#include <detpic32.h>

volatile unsigned char voltage = 0; 

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

// Interrupt Handler    
void _int_(27) isr_adc(void)      // Replace VECTOR by the A/D vector                                     
                                  // number - see "PIC32 family data                                      
                                  // sheet" (pages 74-76)    
{                                 // ISR actions       
    printInt(ADC1BUF0,16 | 3<<16);
    int* p = (int *)(&ADC1BUF0);
    int sum = 0;
    for(;p <= (int *)(&ADC1BUF3);p++){
        printInt(*p,16 | 4 <<16);
        printStr(" ");
        sum += *p;
    }
    double val_ad = sum/(AD1CON2bits.SMPI+1);
    voltage = (val_ad*33+511)/1023;
    IFS1bits.AD1IF = 0;           // Reset AD1IF flag    
}

void _int_(4) isr_T1(void){
    AD1CON1bits.ASAM = 1;
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void){
    send2displays(voltage);
    IFS0bits.T3IF = 0;
}

void send2displays( unsigned char value){
   
    unsigned int i=0;
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,
                                          0x66, 0x6D, 0x7D, 0x07, 
                                          0x7F, 0x6F, 0x77, 0x7C, 
                                          0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    int digit_low = value >> 4, digit_high = value & 0x0F;

    if(displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
        if(digit_high != 0)LATBbits.LATB15 = 1;
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    }
    displayFlag = !displayFlag;
}

void configureAll(){
    TRISBbits.TRISB4 = 1;     // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0;     // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;     // Conversion trigger selection bits: in this
                              //  mode an internal counter ends sampling and
                              //  starts conversion
    AD1CON1bits.CLRASAM = 1;  // Stop conversions when the 1st A/D converter
                              //  interrupt is generated. At the same time, 
                              //  hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 7;     // Interrupt is generated after XX samples
                              //  (replace XX by the desired number of  
                              //  consecutive samples)
    AD1CHSbits.CH0SA = 4;     // replace x by the desired input
                              //  analog channel (0 to 15)
    AD1CON1bits.ON = 1;       // Enable A/D converter 
                              //  This must the last command of the A/D
                              //  configuration sequence
    IPC6bits.AD1IP = 2;       // configure priority of A/D interrupts
    
    IEC1bits.AD1IE = 1;       // enable A/D interrupts

    T3CONbits.TCKPS = 5;
    PR3 = 6249;            
    TMR3 = 0;
    
    
    IPC2bits.T3IP = 2;
    IEC0bits.T3IE = 1;

    T1CONbits.TCKPS = 7;
    PR1 = 19500;            
    TMR1 = 0;
    
    
    IPC2bits.T1IP = 2;
    IEC0bits.T1IE = 1;

    T3CONbits.TON = 1;

    T1CONbits.TON = 1;
}

int main(void){
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    configureAll();

    IFS1bits.AD1IF = 0;       // clear A/D interrupt 
    IFS0bits.T3IF = 0;
    IFS0bits.T1IF = 0;

    EnableInterrupts();

    while(1);
    
    return 0;
}