#ifndef UTILS_H
#define UTILS_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct{
    volatile uint8_t *lat;
    volatile uint8_t *tris;
    uint8_t bitmask;
}pin_t;


/*
pin_t is a struct pointing to a specific data pin and related registers for it
There is no direct physical hardware connected, only references

p->lat = pointer to a LAT register (latch)
derefering this pointer, gets the actual LAT register
|= p->bitmask, sets that actual bit in that register

*/
/*
i have a lot more code space this time around, so proper abstracton is actually a possibility*/

static inline void PIN_HI(pin_t pin)
{
    *pin.lat |= pin.bitmask;
}
static inline void PIN_LO(pin_t pin)
{
    *pin.lat &= (uint8_t)~pin.bitmask;
}

#endif