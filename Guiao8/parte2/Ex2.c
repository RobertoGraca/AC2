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
    int digit_high = value >> 4, digit_low = value & 0x0F;

    if(displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        if(digit_high != 0)LATBbits.LATB15 = 1;
    }
    displayFlag = !displayFlag;
}

void configureAll(){
    // Configure Timer T1 (4 Hz) 
	T1CONbits.TCKPS = 7; 		// 1:256 prescaler
	PR1 = 19530;				// Fout = 4 Hz
	TMR1 = 0;					// Reset timer T1 count register
	T1CONbits.TON = 1;			// Enable timer T1 (must be the last command of the 
								// timer configuration sequence)
	// Configure Timer T3 (100 Hz) 
	T3CONbits.TCKPS = 2; 		// 1:256 prescaler
	PR3 = 49999;				// Fout = 100 Hz
	TMR3 = 0;					// Reset timer T3 count register
	T3CONbits.TON = 1;			// Enable timer T3 (must be the last command of the 
								// timer configuration sequence)

	OC1CONbits.OCM = 6;			// PWM mode on OCx; fault pin disabled
	OC1CONbits.OCTSEL = 1; 		// Use timer T3 as the time base for PWM generation 
	OC1RS = 12500; 				// Ton constant (50,000*0.25 = 12,500)
	OC1CONbits.ON = 1;			// Enable OC1 module

	// Configure interrupts
	// T1
	IPC1bits.T1IP = 2; 			// Interrupt priority (must be in range [1..6]) 
	IEC0bits.T1IE = 1; 			// Enable timer T1 interrupts
	IFS0bits.T1IF = 0; 			// Reset timer T1 interrupt flag
	// T3
	IPC3bits.T3IP = 2; 			// Interrupt priority (must be in range [1..6]) 
	IEC0bits.T3IE = 1; 			// Enable timer T3 interrupts
	IFS0bits.T3IF = 0; 			// Reset timer T3 interrupt flag
	// ADC
	IPC6bits.AD1IP = 2; 	// configure priority of A/D interrupts
	IEC1bits.AD1IE = 1; 	// enable A/D interrupts
	IFS1bits.AD1IF = 0; 	// clear A/D interrupt flag

	// UART1
	IPC6bits.U1IP = 2;		// configure priority of interrupts
	IFS0bits.U1RXIF = 0;	// clear interrupt flag (receive)
	IFS0bits.U1EIF = 0; 	// clear interrupt flag (error)
	IEC0bits.U1RXIE = 1;	// enable interrupts (receive)
	IEC0bits.U1EIE = 1;		// enable interrupts (error)

	// Configure the A/D module and port RB4 as analog input
	TRISBbits.TRISB4 = 1;	  			// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;    			// RB4 configured as analog input (AN4)
	AD1CON1bits.SSRC = 7;				// Conversion trigger selection bits
	AD1CON1bits.CLRASAM = 1;			// Stop conversions when the 1st A/D converter interrupt is generated
	AD1CON3bits.SAMC = 16;				// Sample time is 16 TAD (TAD = 100 ns) 
	AD1CON2bits.SMPI = sample_num-1;	// Interrupt is generated after 8=(7+1) sample
	AD1CHSbits.CH0SA = 4; 				// Selects AN4 as input for the A/D converter
	AD1CON1bits.ON = 1;					// Enable A/D converter

	// Configure 7Seg Display outputs
    LATDbits.LATD6 = 1;         // display high active 
    LATDbits.LATD5 = 0;         // display low inactive

    TRISB = TRISB & 0x00FF;     // configure RB8-RB15 as outputs
    TRISD = TRISD & 0xFF9F;     // configure RD5-RD6 as outputs

	// Configure RB0 and RB1 as inputs
    TRISB = TRISB | 0x0003;
}

void configureUART1(int baudrate, char parity, int dataBits, int nStopBits){
    if(baudrate < 600 || baudrate > 115200)baudrate = 115200;
    if(nStopBits > 2)nStopBits = 2;
    
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
    U1MODEbits.BRGH = 0;

    if(dataBits == 9)U1MODEbits.PDSEL = 00;
    else{
        if(parity == 'E')U1MODEbits.PDSEL = 01;
        else if(parity == 'O')U1MODEbits.PDSEL = 10;
        else U1MODEbits.PDSEL = 11;
    }
    U1MODEbits.STSEL = nStopBits-1;

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    U1MODEbits.ON = 1;
}

void setPWM(unsigned int dutyCycle){
    //duty_cycle must be in the range [0, 100]
    OC1RS = 1562.5 * dutyCycle /100;
}

int main(void){

    configureAll();

    configureUART1(115200,'N',8,1);

    EnableInterrupts();

    int dutyCycle;

    while(1){
        int portVal = PORTB & 0x0003;

        switch(portVal){
            case 0:
                IEC0bits.T1IE = 1;
                setPWM(0);
                break;
            case 1:
                IEC0bits.T1IE = 0;
                setPWM(100);
                break;
            case 2:
                IEC0bits.T1IE = 1;
                dutyCycle = voltage * 3;
                break;
        }

    }
    
    return 0;
}