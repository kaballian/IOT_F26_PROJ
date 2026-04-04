#include "include/FAN.h"
#include "include/TMR1.h"

void FAN_init(fan_t *fan, 
    fan_set_duty_fn_t set_duty, 
    void *CTX, 
    uint8_t duty)
{
    if(fan == 0) //avoid nullptr
    {
        return;
    }
    fan->set_duty       = set_duty;
    fan->CTX            = CTX;
    fan->duty           = duty;
    fan->tach_count     = 0;
    fan->pulses_pr_rev  = 2;
    fan->RPM            = 0;
}

void FAN_set_duty(fan_t *fan, uint16_t duty)
{
    if((fan == 0) ||(fan->set_duty == 0))
    {
        return;
    }
    fan->duty = duty;
    fan->set_duty(fan->CTX, duty);
}

void FAN_CNT(fan_t *fan)
{
    fan->tach_count = TMR1_CNT_meas();
    fan->RPM        = FAN_RPM_CONV(fan->tach_count);
}
uint16_t FAN_RPM_CONV(uint16_t pulses)
{
    /*RPM = pulses*60 / (gate time * 2) => pulses * 60/500ms * 2 = pulses * 60u */
    return (uint16_t)(pulses * 60u);
}

void FAN_CNT_start(fan_t *fan)
{
    if(fan == 0)
    {
        return;
    }

    fan->tach_count = 0;
    fan->RPM = 0;
    TMR1_CNT_reset();
    TMR1_CNT_start();
}
void FAN_CNT_stop(fan_t *fan)
{
    if(fan == 0)
    {
        return;
    }
    TMR1_CNT_stop();

    fan->tach_count = TMR1_CNT_meas();
    fan->RPM        = FAN_RPM_CONV(fan->tach_count);

}

