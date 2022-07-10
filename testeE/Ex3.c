#include <detpic32.h>

void delay(int ms){
	for(;ms>0;ms--){
		resetCoreTimer();
		while(readCoreTimer()<20000);
	}
}

unsigned char toBcd(unsigned char to){
	return ((to/10)<<4)+(to%10);
}

static unsigned char display[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void send2display( unsigned char value){
	unsigned char dh = value >> 4;
	unsigned char dl = value & 0x0F;
	static char flag = 0;
	if(flag){
		LATD = LATD & 0xFF9F | 0x0040;
		LATB = (LATB & 0x00FF) | (display[dh] << 8);
	}else{
		LATD = LATD & 0xFF9F | 0x0020;
		LATB = (LATB & 0x00FF) | (display[dl] << 8);
	}
	flag = !flag;
}

int main(void){
	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 1-1;
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;
	
	TRISB = TRISB & 0x00FF;
	TRISD = TRISD & 0xFF9F;
	LATB = LATB & 0x00FF;
	int cont = 19;
	while(1){
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		int value = ADC1BUF0;
		int freq = 1 + (value/255);
		int ms = ((1/freq)*1000)/20;
		int i = 0;
		do{
			send2display(toBcd(cont));
			delay(20);
		}while(++i<ms);
		cont--;
		if(cont<0)cont=20;
		IFS1bits.AD1IF = 0;
	}
}