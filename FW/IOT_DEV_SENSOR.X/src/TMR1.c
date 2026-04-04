#include "include/TMR1.h"


void TMR1_CNT_init(void)
{
    T1CONbits.ON    = 0;        // stop the timer to do the changes
    T1GCONbits.GE   = 0;        // no gate control, only count when on=1

    T1CLKbits.CS    = 0b00000;  //T1CKIPPS
    T1CONbits.CKPS  = 0;        // no prescaler
    T1CONbits.nSYNC = 0;        // sync to internal clock 
    T1CONbits.RD16  = 1;        //1 16 bit register

    /*reset counters*/
    TMR1H = 0;  
    TMR1L = 0;

    // T1CONbits.ON    = 1;        // start the timer.


}
uint16_t TMR1_CNT_meas(void)
{
    uint16_t pulses;

    T1CONbits.ON = 0; //stop the counter
    pulses = TMR1_read16();
    TMR1H = 0;
    TMR1L = 0;
    /*overflow flag from other time should be stopped here*/

    T1CONbits.ON = 1;

    return pulses;
}

static uint16_t TMR1_read16(void)
{
    uint8_t low     = TMR1L;
    uint8_t high    = TMR1H;
    return ((uint16_t)high << 8) | low;
}

void TMR1_CNT_start(void)
{
    T1CONbits.ON = 1; //start the timer
}
void TMR1_CNT_stop(void)
{
    T1CONbits.ON = 0; //stop the timer
}
void TMR1_CNT_reset(void)
{
    T1CONbits.ON = 0;
    
    TMR1H = 0;
    TMR1L = 0;

    PIR1bits.TMR1IF = 0; //optional, clear overflow flag
}