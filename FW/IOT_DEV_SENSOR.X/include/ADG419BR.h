#ifndef ADG419BR_H
#define ADG419BR_H

/*
HW abstraction for ADG419BR analog switch
SPDT (single pole dual throw)

pin#  purpose
1 - I/O
6 - CHL select
2 - IN1 (fan1)
8 - IN2 (fan2)

logic 0 -> IN1
logic 1 -> IN2

it is important to note:
that once this file is included and the accompanying .c file is compiled
to the processor. The device then assumes a ADG419BR is physically connected
to the device, as the .c file owns the singular(Static) instance.
*/

#define ADG419_CHLS 2
typedef enum {
    CHL_1 = 0,
    CHL_2 = 1
}switch_chl_t;

// typedef void (*ADG419BR_CHL_SELECT)(switch_chl_t chl);
void ADG419_init(void);
void ADG419_CHL_SELECT(switch_chl_t chl);
switch_chl_t ADG419_get_chl(void);


#endif