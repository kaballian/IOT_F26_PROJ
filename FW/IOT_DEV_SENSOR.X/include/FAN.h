#ifndef FAN_H
#define FAN_H

#define FAN_BUF_LEN 16

#include <xc.h>
#include "TMR1.h"
/*
FAN module 
makes use of a linked PWM slice to control 
*/
typedef void (*fan_set_duty_fn_t)(void *CTX, uint16_t duty);

typedef struct 
{
    fan_set_duty_fn_t set_duty;
    void *CTX;
    uint16_t min_duty;
    uint16_t max_duty;
    uint16_t tach_count;
    uint16_t pulses_pr_rev;
    uint16_t RPM;
}fan_t;

void FAN_init(fan_t *fan,
            fan_set_duty_fn_t set_duty,
            void *CTX, 
            uint16_t min_duty,
            uint16_t max_duty);

void FAN_set_duty(fan_t *fan, uint16_t duty);

void FAN_CNT(fan_t *fan);
uint16_t FAN_RPM_CONV(uint16_t pulses);

#endif