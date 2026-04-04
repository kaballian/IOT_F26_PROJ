#include "include/interrupt.h"



void ISR_init()
{
    INTCONbits.GIE = 0; //during setup, disable all interrupts
    INTCONbits.PEIE = 1; // enable peripheral interrupt 
    PIE0bits.TMR0IE = 1; //TMR0 interrupt enable
    PIE4bits.RC1IE = 1; //EUSART1 receive interrupt enable
    PIR0bits.TMR0IF = 0; //clear TMR0 overflow falg (this is what we use to check for overflow)
    INTCONbits.GIE = 1;
}

void __interrupt() ISR(void)
{
    /*change so these functionen are guarded by their interrupt falgs*/
    /*also setup the flags dumbass*/
    if(PIE0bits.TMR0IE && PIR0bits.TMR0IF)
    {
        TMR0_ISR();
    }
    if(PIE4bits.RC1IE && PIR4bits.RC1IF)
    {
        EUSART1_ISR();
    }
    
}