#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

// Interrupt Handler    
void _int_(VECTOR) isr_adc(void)  // Replace VECTOR by the A/D vector                                     
                                  // number - see "PIC32 family data                                      
                                  // sheet" (pages 74-76)    
{                                 // ISR actions       
    printInt(ADC1BUF0,16 | 3<<16);
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;           // Reset AD1IF flag    
}

int main(void){

    TRISBbits.TRISB4 = 1;     // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0;     // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;     // Conversion trigger selection bits: in this
                              //  mode an internal counter ends sampling and
                              //  starts conversion
    AD1CON1bits.CLRASAM = 1;  // Stop conversions when the 1st A/D converter
                              //  interrupt is generated. At the same time, 
                              //  hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 0;     // Interrupt is generated after XX samples
                              //  (replace XX by the desired number of  
                              //  consecutive samples)
    AD1CHSbits.CH0SA = 4;     // replace x by the desired input
                              //  analog channel (0 to 15)
    AD1CON1bits.ON = 1;       // Enable A/D converter 
                              //  This must the last command of the A/D
                              //  configuration sequence
    IPC6bits.AD1IP = 2;       // configure priority of A/D interrupts

    IFS1bits.AD1IF = 0;       // clear A/D interrupt 
    
    IEC1bits.AD1IE = 1;       // enable A/D interrupts

    EnableInterrupts();

    AD1CON1bits.ASAM = 1;

    while(1){}
    
    return 0;
}