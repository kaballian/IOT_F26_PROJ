#include "include/FAN.h"
#include "include/TMR1.h"
#include "include/system.h"

void FAN_init(fan_t *fan, 
    pwm_chan_t *pwm,  
    uint8_t duty_percent)
{
    if(fan == 0) //avoid nullptr
    {
        return;
    }

    fan->pwm            = pwm;
    fan->duty_percent   = 0;
    fan->tach_count     = 0;
    fan->pulses_pr_rev  = 2;
    fan->RPM            = 0;

    FAN_set_duty(fan, duty_percent);
}

void FAN_set_duty(fan_t *fan, uint8_t duty_percent)
{
    if((fan == 0) ||(fan->pwm == 0))
        return;

    if(duty_percent >100)
        duty_percent = 100;

    fan->duty_percent = duty_percent;
    PWM_set_percent(fan->pwm, duty_percent);
    
}


// void FAN_CNT(fan_t *fan)
// {
//     fan->tach_count = TMR1_CNT_meas();
//     fan->RPM        = FAN_RPM_CONV(fan->tach_count);
// }
uint16_t FAN_RPM_CONV(uint16_t pulses,
    uint16_t gate_ms, uint8_t pulses_pr_rev)
{
    // /*RPM = pulses*60 / (gate time * 2) => pulses * 60/500ms * 2 = pulses * 60u */
    // return (uint16_t)(pulses * 60u);
    if(gate_ms == 0 || pulses_pr_rev == 0)
    {
        return 0;
    }

    return (uint16_t)(((uint32_t)pulses * 60000u) / ((uint32_t)gate_ms * pulses_pr_rev));

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
    fan->RPM        = FAN_RPM_CONV(fan->tach_count, 500, fan->pulses_pr_rev);

}

