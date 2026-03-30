#include "include/pins.h"

void PIN_MANAGER_init(void)
{
 
    /*LATx registers*/
    /*LATCHES  simiar to AVR ports*/

    LATA = 0x0;
    LATC = 0x0;

    /*
    TRISx registers
    1: input
    0: output

    PORT    =   |FUNCTION|DIRECTION|VAL|PPS
    RA0     =   SWDIO               X               X
    RA1     =   SWCLK               X               X
    RA2     =   FAN_TACH_SELECT     OUT             0   GPIO
    RA3     =   RESET               X               X
    RA4     =   UART RX             IN              1   PPS
    RA5     =   UART TX             OUT             0   PPS
    RC0     =   T1CLKIN             IN              1   PPS
    RC1     =   PWM11               OUT             0   PPS
    RC2     =   PWM12               OUT             0   PPS
    RC3     =   I2C SDA             BIDIRECTIONAL       PPS
    RC4     =   I2C SCL             BIDIRECTIONAL       PPS
    RC5     =   STAT_LED            OUT             0   PPS (TMR2)
    */


    // TRISAbits.TRISA0
    // TRISAbits.TRISA1
    TRISAbits.TRISA2 = 0;
    //TRISA3 -> RESET
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 0;

    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3
    TRISCbits.TRISC4
    TRISCbits.TRISC5 = 0;

}