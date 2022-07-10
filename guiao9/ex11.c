#include <detpic32.h>

#define DisableUart2ReceiverInterrupt() IEC1bits.U2RXIE = 0;
#define EnableUart2ReceiverInterrupt() IEC1bits.U2RXIE = 1;
#define DisableUart2TransmiterInterrupt() IEC1bits.U2TXIE = 0;
#define EnableUart2TransmiterInterrupt() IEC1bits.U2TXIE = 1;
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb;
volatile circularBuffer rxb;

void comDrv_flushRx(void){
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;
}

void comDrv_flushTx(void){
    txb.head = 0;
    txb.tail = 0;
    txb.count = 0;
}

char comDrv_getc(char* pchar){
    if(rxb.count == 0)return 0;
    DisableUart2ReceiverInterrupt();
    *pchar = rxb.data[rxb.head];
    rxb.count--;
    rxb.head = (rxb.head + 1) % BUF_SIZE;
    EnableUart2ReceiverInterrupt();
    return 1;
}

void comDrv_putc(char ch){
    while(txb.count == BUF_SIZE);
    txb.data[txb.tail] = ch;

    txb.tail = (txb.tail + 1) % BUF_SIZE;

    DisableUart2TransmiterInterrupt();
    txb.count++;
    EnableUart2TransmiterInterrupt();
}

void comDrv_puts(char* s){
    while(*s != '\0'){
        comDrv_putc(*s);
        s++;
    }
}


void _int_(32) isr_uart2(void){
    if(IFS1bits.U2TXIF == 1){
        if(txb.count > 0){
            U2TXREG = txb.data[txb.head];
            txb.head = (txb.head + 1) % BUF_SIZE;
            txb.count--;
        }else if(txb.count == 0){
            DisableUart2TransmiterInterrupt();
        }
        IFS1bits.U2TXIF = 0;
    }
    if(IFS1bits.U2RXIF == 1){
        rxb.data[rxb.tail] = U2RXREG;

        rxb.tail = (rxb.tail + 1) % BUF_SIZE;

        if(rxb.count < BUF_SIZE)rxb.count++;
        else rxb.head = (rxb.head + 1) % BUF_SIZE;

        IFS1bits.U2RXIF = 0;
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

    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0;
    IPC8bits.U2IP = 2;

    U2MODEbits.ON = 1;
}

int main(void){
    uart_conf(115200,'N',8,1);

    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();
    
    comDrv_puts(" PIC32 UART Device-driver\n");

    char* str = "Boas";

    while(1){
    }

    return 0;
}
