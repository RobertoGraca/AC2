#include <detpic32.h>

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

    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 2;

    U2MODEbits.ON = 1;
}

char getc(void){
    if(U2STAbits.OERR == 1)U2STAbits.OERR = 0;
    while(U2STAbits.URXDA == 0);
    if(U2STAbits.FERR == 1 || U2STAbits.PERR == 1){
        char c = U2RXREG;
        return 0;
    }
    return U2RXREG;
}

void putc(char byte2send){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}

void _int_(32) isr_uart2(void){
    putc(U2RXREG);
    IFS1bits.U2RXIF = 0;
}

int main(void){
    uart_conf(115200,'N',8,1);
    EnableInterrupts();

    while(1){}

    return 0;
}