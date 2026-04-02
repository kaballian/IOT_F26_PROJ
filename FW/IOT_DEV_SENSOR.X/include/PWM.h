#ifndef PWM_H
#define PWM_H


#include <xc.h>
/*
PWM driver 
A PWM module cna have up to four(16f18124 (2)) output slices.
each consisting of (2) PWM outputs:

PWMx_SaP1_out 
PWMx_SaP2_out

they have independent duty cycles, but share the same base frequency.

polarity: controlled by POL1/2 bits. setting the polarity bit
inverts the output active state to Low true.

Operating mode:
left aligned mode:
ACtive part of the duty cycle is at the beginning of the period and the inactive
part of the cycle is the remainder. The is thje classic pwm signal.

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
*/

typedef struct 
{
    // volatile uint8_t *pL;
    // volatile uint8_t *pH;
    // volatile uint8_t *con;
    volatile unsigned short *pR;    //pointer to parameter register
    volatile uint8_t        *con;   //control register
    uint8_t         ld_mask;
    uint16_t        period;
}pwm_chan_t;


void PWM_init(void);
void PWM_set_duty(void *CTX, uint16_t val);
// void PWM_set_period(pwm_chan_t *chan, uint16_t period);

extern pwm_chan_t PWM_FAN1_CH;
extern pwm_chan_t PWM_FAN2_CH;

#endif  