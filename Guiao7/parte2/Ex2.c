#include <detpic32.h>

void configureInputs(){
    T3CONbits.TCKPS = 5;
    PR3 = 6249;
    TMR3 = 0;

    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 1;
    OC1RS = 1562.5;
    OC1CONbits.ON = 1;

    T3CONbits.TON = 1;
}

void setPWM(unsigned int dutyCycle){
    //duty_cycle must bem in the range [0, 100]
    OC1RS = 1562.5 * dutyCycle /100;
}

int main(void){
    configureInputs();
    int[10] PWMdutyCycle = {10,25,30,40,50,60,70,80,90,100};

    while(1){
        int i;
        for(i=0;i<=8;i++){
            i = i%8;

            int j = 0;
            while(j<5000000){
                setPWM(PWMdutyCycle[i]);
                j++;
            }
        }
    }
    

    return 0;
}