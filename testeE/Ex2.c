#include <detpic32.h>

void delay(int ms){
	for(;ms>0;ms--){
		resetCoreTimer();
		while(readCoreTimer()<20000);
	}
}

static const char display[] =    {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void send2display( unsigned char value){
	unsigned char dh = value >> 4;
	unsigned char dl = value & 0x0F;
	static char flag = 0;
	if(flag){
		LATD = (LATD & 0xFF9F) | 0x0040;
		LATB = (LATB & 0x00FF) | (display[dh] << 8);
	}else{
		LATD = (LATD & 0xFF9F) | 0x0020;
		LATB = (LATB & 0x00FF) | (display[dl] << 8);
	}
	flag = !flag;
}

int main(void){
	TRISB = TRISB & 0x00FF;
	TRISD = TRISD & 0xFF9F;
	LATB = LATB & 0x00FF;
	char key = 0;
	int i;
	while(1){
		char c = inkey();
		if(c != 0) key = c;
		switch (key){
			case '0':
				send2display(00);
				delay(10);
				break;
			case '1':
				send2display(01);
				delay(10);
				break;
			case '2':
				send2display(02);
				delay(10);
				break;
			case '3':
				send2display(03);
				delay(10);
				break;
			case 0:
				break;
			default:
				i = 0;
				do{
					send2display(0xFF);
					delay(10);
				}while(++i<100);
				LATB = LATB & 0x00FF;
				key = 0;
				break;
		}
	}
			
	return 0;
}