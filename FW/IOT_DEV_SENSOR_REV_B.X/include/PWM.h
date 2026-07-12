#ifndef PWM_H
#define PWM_H

#include "utils.h"

/*
PWM driver
A PWM module can have up to output slices per PWM channel

consisting of 
PWMx_SaP1_out
PWMx_SaP2_out

they have independent duty cycles, but share the same bare frequency

polarity: controlled by POL bits, this 
inverts the output active state to Low True

operating mode:
left aligned mode:
active part of the duty cycle is at the beggning of the periode and the 
inactive part of the cycle is the remainder, this is classic PWM mode

period timer:
all slices in a pwm instance operator with the same period. Value written to
PWMxPR register is one less than the number of prescaled PWM clock periods (PWM_clk) in the pwm period.
The registers is double buffered. 

Clock source:
time base for PWM period presclaer is selected with the CLK bits. 
clearing tthe EN bit before making sources changes is recommended.

*/

/* freqency for PWM output is:
Fpwm = (Fosc)/(Prescaler * (PWM_cnt + 1))

rearranged for PWM_cnt

PWM_cnt = Fosc / (prscaler * Fpwm)  - 1

16MHz / 1 * 32KHz  - 1 = 499

RECALCULATE THIS FOR 20MHz
*/

typedef struct 
{
    volatile unsigned short *pR;    //pointer to parameter register
    volatile uint8_t        *con;   //control register
    uint8_t                 ld_mask;
    uint16_t                period; 
}pwm_chan_t;

void PWM_init(void);
void PWM_set_raw(pwm_chan_t *chan, uint16_t counts);
void PWM_set_percent(pwm_chan_t *chan, uint16_t percent);


extern pwm_chan_t PWM_FAN_CHL1;
extern pwm_chan_t PWM_FAN_CHL2;
extern pwm_chan_t PWM_FAN_CHL3;
extern pwm_chan_t PWM_FAN_CHL4;


#endif

