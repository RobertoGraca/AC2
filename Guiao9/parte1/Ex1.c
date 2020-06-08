#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
}   circularBuffer;

volatile circularBuffer rxb;
volatile circularBuffer txb;

void comDrv_flushRx(void){
    rxb.count = 0;
    rxb.head = 0;
    rxb.tail = 0;
}

void comDrv_flushTx(void){
    txb.count = 0;
    txb.head = 0;
    txb.tail = 0;
}

void comDrv_putc(char c){
    while(txb.count == BUF_SIZE);
    txb.data[txb.tail] = c;

    txb.tail = (txb.tail + 1) & INDEX_MASK;

    DisableUart1TxInterrupt();
    txb.count++;
    EnableUart1TxInterrupt();
}

void comDrv_puts(char* s){
    while(s != '\0'){
        comDrv_putc(s);
        s++;
    }
}

char comDrv_getc(char* pchar){
    if(rxb.count == 0)return 0;
    DisableUart1RxInterrupt();
    *pchar = rxb.data[rxb.head];
    rxb.count--;
    rxb.head = (rxb.head + 1) % INDEX_MASK;
    EnableUart1RxInterrupt();
    return 1;
}

void comDrv_config(int baudrate, char parity,int nStopBits){
    if(baudrate < 600 || baudrate > 115200)baudrate = 115200;
    if(nStopBits > 2)nStopBits = 2;
    int dataBits = 8;
    
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

void _int_(24) isr_uart1(void){
    if(IFS0bits.U1TXIF){
        if(txb.count > 0){
            U1TXREG = txb.data[txb.head];
            txb.head = (txb.head + 1) % INDEX_MASK;
            txb.count--;
        }
        else if(txb.count == 0){
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF = 0;
    }
    if(IFS0bits.U1RXIF){
        rxb.data[rxb.tail] = U1RXREG;

        rxb.tail = (rxb.tail + 1) % INDEX_MASK;
        if(rxb.count < BUF_SIZE)rxb.count++;
        else {
            rxb.head = (rxb.head + 1) & INDEX_MASK;
        }
        IFS0bits.U1RXIF = 0;
    }
}

int main(void){
    comDrv_config(115200,'N',1);

    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();

    char* s = "PIC32 UART Device-driver\n";

    comDrv_puts(s);

    while(1){
        if(comDrv_getc(s)){
            comdrv_putc(*s);
        }
    }
}

