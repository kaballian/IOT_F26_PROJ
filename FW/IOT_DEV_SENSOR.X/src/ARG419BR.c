#include "include/ADG419BR.h"



// typedef struct{
//     pin_t               channelSelector;
//     switch_chl_t        status;
// }ADG419_t;

// static ADG419_t AnalogSwitch = {
//     .channelSelector        = PIN_RA2,
//     .status                 = CHL_1,
// };

void ADG419_init(ADG419_t *dev, const pin_t select_pin)
{
    if(dev == 0)
    {
        return;
    }

    dev->channelSelector    = select_pin;
    dev->status             = CHL_1;

    pin_output(&dev->channelSelector);
    ADG419_CHL_SELECT(dev, CHL_1);
}

void ADG419_CHL_SELECT(ADG419_t *dev, switch_chl_t chl)
{
    switch (chl)
    {
    case CHL_1:
        pin_clear(&dev->channelSelector);
        break;
    
    case CHL_2:
        pin_set(&dev->channelSelector);
        break;

    default:
        break;
    }

}

