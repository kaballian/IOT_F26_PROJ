#include "include/eusart1.h"
#include <pic16f18124.h>


void EUSART1_init(void)
{
    TX1REG      = 0x0;
    BAUD1CON    = 0x40;
    RC1STA      = 0x0;
    TX1STA      = 0x22;
    SP1BRGL     = 0x0;
    SP1BRGH     = 0x0;
}