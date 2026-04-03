#ifndef UTILS_H
#define UTILS_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct {
    volatile uint8_t *lat;
    volatile uint8_t *tris;
    uint8_t bitmask;
}pin_t;

/*
p points to a pin_t the describes a specific pin
p->lat is a pointer to the correct LAT register ( ex. &LATA)
*p->lat dereferences that pointer -> the actual LATA register
|= p->bitmask sets the actual bit in the register 
*/

/*set the output of the pin HIGH*/
static inline void pin_set(const pin_t *p)      {*p->lat |= p->bitmask;}
/*clears the output of the pin, sets it to 0*/
static inline void pin_clear(const pin_t *p)    {*p->lat &= (uint8_t)~p->bitmask;}
 /* tris register controls output drivers  analog pins always read '0',
TRIS = 0 -> output, TRIS = 1 -> input*/
static inline void pin_output(const pin_t *p)   {*p->tris &= (uint8_t)~p->bitmask;}


/*
given:
typedef struct {
    volatile uint8_t *lat;
    volatile uint8_t *tris;
    uint8_t bitmask;
} pin_t;

const pin_t *p

p->lat  = (*p).lat, p is a pointer to a struct, access the lat field 
that field is a pointer to a register itself so
p->lat = volatile uint8_t * (pointer to a hardware register)

*p->lat = *(p->lat)  it deferences the pointer which lat is pointer to
p is a pointer to a struct
lat is a pointer to a register,
this is double level access.

*/




#endif