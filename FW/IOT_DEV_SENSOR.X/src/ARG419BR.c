#include "include/ADG419BR.h"
#include "include/utils.h"


#define PIN_RA2     ((pin_t){.lat=&LATA, .tris=&TRISA, .bitmask=(1u<<2)})


typedef struct{
    pin_t               channelSelector;
    switch_chl_t        status;
}ADG419_t;

static ADG419_t AnalogSwitch = {
    .channelSelector        = PIN_RA2,
    .status                 = CHL_1,
};

void ADG419_init()
{
    pin_output(&AnalogSwitch.channelSelector);
    ADG419_CHL_SELECT(CHL_1);
}

void ADG419_CHL_SELECT(switch_chl_t chl)
{
    switch (chl)
    {
    case CHL_1:
        pin_clear(&AnalogSwitch.channelSelector);
        AnalogSwitch.status = CHL_1;
        break;
    
    case CHL_2:
        pin_set(&AnalogSwitch.channelSelector);
        AnalogSwitch.status = CHL_2;
        break;

    default:
        break;
    }

}

switch_chl_t ADG419_get_chl()
{
    return AnalogSwitch.status;
}
