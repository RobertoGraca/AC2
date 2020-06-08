#include <detpic32.h>

void delay(unsigned int n){
    for(;n>0;n--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void putc(char byte2send){
    while(U1STAbits.UTXBF==1);
    U1TXREG = byte2send;
}

void puts(char* str){
    while(str |= '\0'){
        putc(*str);
        str++;
    }
}

void configureUART1(int baudrate, char parity, int dataBits, int nStopBits){
    if(baudrate < 600 || baudrate > 115200)baudrate = 115200;
    if(dataBits != 8) dataBits = 8;
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

int main(void){
    //configureUART1(115200,'N',8,1);
    //configureUART1(600,'N',8,1);
    //configureUART1(1200,'O',8,2);
    //configureUART1(9600,'E',8,1);
    //configureUART1(19200,'N',8,2);
    configureUART1(115200,'E',8,1);

    long cont=0;

    while(1){
        puts("String de teste %ld\n",cont);
        cont++;
        delay(1000);
    }
   

}