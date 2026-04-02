#include "include/PWM.h"
#include <pic16f18124.h>

pwm_chan_t PWM_FAN1_CH = {
    .pR         = &PWM1S1P1,
    .con        = &PWM1CON,
    .ld_mask    = (1u<<2), 
    .period     = 499,
};

pwm_chan_t PWM_FAN2_CH = {
    .pR         = &PWM1S1P2,
    .con        = &PWM1CON,
    .ld_mask    = (1u<<2),
    .period     = 499,
};

void PWM_init(void)
{
    PWM1ERS     = 0; //disabled external reset source select.
    PWM1CLK     = 0b00000011;  //high frequency internal oscillator
    PWM1LDS     = 0;
    PWM1PR      = 499; /*32KHz frequency*/
    PWM1CPRE    = 0; /* prescaler register does n+1 */
    // PWM1GIR     =
    /*disabled interrupts*/
    PWM1GIEbits.S1P2 = 0;
    PWM1GIEbits.S1P1 = 0;

    PWM1CONbits.EN = 1;
    PWM1CONbits.LD = 1; /*rreload the registers? not sure if necessary*/

    /*
    slice A configuration 
    */
    /* output polarity is high true*/
    PWM1S1CFGbits.POL2 = 0;
    PWM1S1CFGbits.POL1 = 0;
    PWM1S1CFGbits.PPEN = 0;
    PWM1S1CFGbits.MODE = 0;  //left aligned mode

    //preload with ~20% duty cycle, 1 slightly faster than the other
    /* 499 = 100%*/
    PWM1S1P1 = 90;  //18%
    PWM1S1P2 = 120; //24%

}
void PWM_set_duty_P1(void *CTX, uint16_t duty)
{
    pwm_chan_t *chan = (pwm_chan_t *)CTX;
    
    if(chan == 0) //check for nullptr
    {
        return;
    }
    if(duty >= 100) //check if 100 or above.
    {
        return;
    }
    /* 
    MAP:
    [a:b] -> [c:d]   assuming a!=b
    f(t) = c + ((d-c) / (b-a)) * (t-a)
    */
    *(chan->pR) = (uint16_t)(499/100)*(duty); //this could be precomputed

    *(chan->con) |= chan->ld_mask;
}
