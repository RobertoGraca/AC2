#include <detpic32.h>


void uart_conf(int baudrate,char parity,int databits,int stopbits){
    U2BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
    U2MODEbits.BRGH = 0;

    if(databits == 8){
        if(parity == 'N')U2MODEbits.PDSEL = 0;
        else if(parity == 'E')U2MODEbits.PDSEL = 1;
        else if(parity == 'O')U2MODEbits.PDSEL = 2;
    }else if(databits == 9){
        U2MODEbits.PDSEL = 3;
    }

    U2MODEbits.STSEL = stopbits - 1;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;
}

int main(void){
    uart_conf(115200,'N',8,1);



    return 0;
}
