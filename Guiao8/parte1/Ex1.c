#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int baudrate = 115200;

void putc(char byte2send){
    while(U1STAbits.UTXBF==1);
    U1TXREG = byte2send;
}

int main(void){

    U1BRG = (PBCLK / (16 * baudrate)) - 1;
    U1MODEbits.BRGH = 0;

    U1MODEbits.PDSEL = 00;
    U1MODEbits.STSEL = 0;

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    U1MODEbits.ON = 1;

}