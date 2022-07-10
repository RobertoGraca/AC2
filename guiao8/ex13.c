#include <detpic32.h>

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void putc(char byte2send){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}

void puts(char* s){
    while(*s != '\0'){
        putc(*s);
        s++;
    }
}

void uart_conf(int baudrate,char parity,int databits,int stopbits){
    if(baudrate < 600)baudrate = 600;
    else if(baudrate > 115200)baudrate = 115200;
    U2BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
    U2MODEbits.BRGH = 0;

    if(databits == 8){
        if(parity == 'N')U2MODEbits.PDSEL = 0;
        else if(parity == 'E')U2MODEbits.PDSEL = 1;
        else if(parity == 'O')U2MODEbits.PDSEL = 2;
    }else if(databits == 9){
        U2MODEbits.PDSEL = 3;
    }
    
    if(stopbits > 2)stopbits = 2;
    else if(stopbits < 1)stopbits = 1;
    U2MODEbits.STSEL = stopbits - 1;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;
}

int main(void){
    uart_conf(115200,'N',8,1);

    while(1){
        puts("String de teste\n");
        delay(1000);
    }

    return 0;
}
