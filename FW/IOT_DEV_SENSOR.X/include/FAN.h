#ifndef FAN_H
#define FAN_H

#define FAN_BUF_LEN 8

#include <xc.h>
#include "TMR1.h"
#include "PWM.h"
/*
FAN module 
makes use of a linked PWM slice to control 
*/
// typedef void (*fan_set_duty_fn_t)(void *CTX, uint16_t duty);

typedef struct 
{
    // fan_set_duty_fn_t set_duty;
    // void *CTX;
    pwm_chan_t *pwm;
    uint8_t  duty_percent;
    uint16_t tach_count;
    uint8_t pulses_pr_rev;
    uint16_t RPM;
}fan_t;

void FAN_init(fan_t *fan,
            pwm_chan_t *pwm,
            uint8_t duty_percent);

void FAN_set_duty(fan_t *fan, uint8_t duty_percent);

// void FAN_CNT(fan_t *fan);
uint16_t FAN_RPM_CONV(uint16_t pulses, uint16_t game_ms, uint8_t pulses_pr_rev);

void FAN_CNT_start(fan_t *fan);
void FAN_CNT_stop(fan_t *fan);


#endif