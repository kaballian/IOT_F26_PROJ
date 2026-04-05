#ifndef TMR0_H
#define TMR0_H

/*abstraction layer for timer 0
used as system tick and alternate mode window time
for timer 1's counting sequence*/

#include "utils.h"

// extern volatile uint32_t    g_sys_ms;
// extern volatile uint8_t     g_fsm_tick_flag;
// extern volatile uint8_t     g_fan_gate_flag;

volatile uint8_t g_tmr0_1ms_flag;
volatile uint32_t g_sys_ms;
volatile uint8_t g_fan_f;

void TMR0_init(void);
void TMR0_ISR(void);

#endif