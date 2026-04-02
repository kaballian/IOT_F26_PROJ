#include "include/FAN.h"

void FAN_init(fan_t *fan, 
    fan_set_duty_fn_t set_duty, 
    void *CTX, 
    uint16_t min_duty, 
    uint16_t max_duty)
{
    if(fan == 0) //avoid nullptr
    {
        return;
    }
    fan->set_duty = set_duty;
    fan->CTX = CTX;
    fan->min_duty = min_duty;
    fan->max_duty = max_duty;
}

void FAN_set_duty(fan_t *fan, uint16_t duty)
{
    if((fan == 0) ||(fan->set_duty == 0))
    {
        return;
    }

    if(duty < fan->min_duty)
    {
        duty = fan->min_duty;
    }
    if(duty > fan->max_duty)
    {
        duty = fan->max_duty;
    }

    fan->set_duty(fan->CTX, duty);
}