#include "include/PWM.h"

pwm_chan_t PWM_FAN_CHL={
    .pR         = &PWM1S1P1,
    .con        = &PWM1CON,
    .ld_mask    = (1u<<2), //reload period and DC register on next period event section 29.9.9
    .period     = 624, // (20e6 / (1*32e3)) - 1
};
pwm_chan_t PWM_FAN_CHL2={
    .pR         = &PWM1S1P2,
    .con        = &PWM1CON,
    .ld_mask    = (1u<<2),
    .period     = 624, // (20e6 / (1*32e3)) - 1
};
pwm_chan_t PWM_FAN_CHL3={
    .pR         = &PWM2S1P1,
    .con        = &PWM2CON,
    .ld_mask    = (1u<<2),
    .period     = 624, // (20e6 / (1*32e3)) - 1
};
pwm_chan_t PWM_FAN_CHL4={
    .pR         = &PWM2S1P2,
    .con        = &PWM2CON,
    .ld_mask    = (1u<<2),
    .period     = 624, // (20e6 / (1*32e3)) - 1
};


void PWM_init(void)
{
    PWM1ERS     = 0; // disable external reset source select
    PWM2ERS     = 0;
    PWM1CLK     = 0b00000010; // Fosc
    PWM2CLK     = 0b00000010;
    PWM1LDS     = 0; // autpload disable
    PWM2LDS     = 0;
    PWM1PR      = 624;
    PWM2PR      = 624;
    PWM1CPRE    = 0;  /*prescaler register does n+1*/
    PWM2CPRE    = 0; // =||=

    /*disable interrupts*/
    PWM1GIEbits.S1P1 = 0;
    PWM1GIEbits.S1P2 = 0;
    PWM2GIEbits.S1P1 = 0;
    PWM2GIEbits.S1P2 = 0;

    PWM1CONbits.EN = 1;
    PWM2CONbits.EN = 1;

    PWM1CONbits.LD = 1;
    PWM2CONbits.LD = 1;

    /*preset values*/
    PWM1S1P1 = 100;
    PWM1S1P2 = 120;
    PWM2S1P1 = 150;
    PWM2S1P2 = 180;

}
void PWM_set_raw(pwm_chan_t *chan, uint16_t counts)
{
    if(chan == 0)
    {
        return;
    }
    if(counts > chan->period)
    {
        counts = chan->period;
    }
    *(chan->pR) = counts;
    *(chan->con) |= chan->ld_mask;

}
void PWM_set_percent(pwm_chan_t *chan, uint16_t percent)
{
    if(chan == 0)
    {
        return;
    }
    if(percent > 100)
    {
        percent = 100;
    }
    uint16_t counts = (uint16_t)(((uint32_t)chan->period * percent) / 100u);
    PWM_set_raw(chan, counts);
}