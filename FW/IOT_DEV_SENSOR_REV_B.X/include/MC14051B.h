#ifndef MC14051_H
#define MC14051_H

#include "include/utils.h"

//number of channels
#define MC14051_CHLS 4
//pin selections (naming convention from schematic)
#define TAC_SEL_INH     ((pin_t){.lat=&LATA, .tris=&TRISA, .bitmask=(1u<<RA0)})
#define TAC_SEL_C       ((pin_t){.lat=&LATA, .tris=&TRISA, .bitmask=(1u<<RA1)})
#define TAC_SEL_B       ((pin_t){.lat=&LATA, .tris=&TRISA, .bitmask=(1u<<RA2)})
#define TAC_SEL_A       ((pin_t){.lat=&LATA, .tris=&TRISA, .bitmask=(1u<<RA3)})

#define TAC_SEL_MASK \ (TAC_SEL_A.bitmask | TAC_SEL_B.bitmask | TAC_SEL_C.bitmask)

typedef enum{
    CHL_1 = 0,
    CHL_2 = 1,
    CHL_3 = 2,
    CHL_4 = 3,
}switch_chl_t;

typedef struct{
    switch_chl_t    status;
    pin_t           inhibit;
    pin_t           sel_A;
    pin_t           sel_B;
    pin_t           sel_C;

}MC14051_t;


void MC14051_init(MC14051_t *dev);
void MC14051_CHL_SELECT(MC14051_t *dev, switch_chl_t chl);
void MC14051_INHIB_ON(MC14051_t *dev);
void MC14051_INHIB_OFF(MC14051_t *dev);

#endif