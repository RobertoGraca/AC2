#include <detpic32.h>

void conf_uart(int baudrate,char parity,int databits,int stopbits){
    if(baudrate < 600)baudrate = 600;
    else if(baudrate > 115200)baudrate = 115200;

    U2BRG = ((PBCLK + (8*baudrate)) / (16 * baudrate)) - 1;
    U2MODEbits.BRGH = 0;

    if(databits < 8)databits = 8;
    else if(databits > 9)databits = 9;
    if(databits == 8){
        if(parity == 'N')U2MODEbits.PDSEL = 0;
        else if(parity == 'E')U2MODEbits.PDSEL = 1;
        else if(parity == 'O')U2MODEbits.PDSEL = 2;
    }else if(databits == 9)U2MODEbits.PDSEL = 3;

    if(stopbits < 1)stopbits = 1;
    else if(stopbits > 2)stopbits = 2;
    U2MODEbits.STSEL = stopbits - 1;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    U2MODEbits.ON = 1;
    IPC8bits.U2IP = 2;
    
    IEC1bits.U2RXIE = 1;
    
}

void putc(char byte2send){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}

void puts(char* str){
    while(*str != '\0'){
        putc(*str);
        str++;
    }
}

char getc(){
    if(U2STAbits.OERR == 1)U2STAbits.OERR = 0;
    while(U2STAbits.URXDA == 0);
    char c = U2RXREG;
    return c;
}

void _int_(32) isr_uart2(void){
    static int charCount = 0;
    char newChar;

    newChar = getc();

    if(newChar == 0x0A){
        putc('\n');
        puts("O numero de minusculas digitado foi ");
        putc('0'+charCount);
        charCount=0;
    }

    if(newChar >= 'a' && newChar <= 'z')charCount++;

    putc(newChar);

    IFS1bits.U2RXIF = 0;
}

int main(void){
    conf_uart(115200,'N',8,1);
    EnableInterrupts();
    while(1){}

    return 0;
}