#ifndef TMR1_H
#define TMR1_H

/*timer1 abstraction for using timer 1 as a counter*/
#include "utils.h"

void TMR1_CNT_init(void);
uint16_t TMR1_CNT_meas(void);

static uint16_t TMR1_read16(void);

#endif
