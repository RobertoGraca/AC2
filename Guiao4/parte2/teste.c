#include <stdio.h>

void delay(){
    long n = 50000000;
    while(n>0){
        n--;
    }
}

int main(void){
    unsigned char c;

    while(1){
        printf("%5d\n",c);
        c = !c;
        delay();
    }
    return 0;
}