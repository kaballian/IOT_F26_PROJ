#ifndef INTERRUPT_H
#define INTERRUPT_H

/*interrupt service routines for IOT sensor
*/

#include "TMR0.h"
#include "eusart1.h"

extern volatile uint8_t g_tmr0_1ms_flag;


void ISR_init();

#endif