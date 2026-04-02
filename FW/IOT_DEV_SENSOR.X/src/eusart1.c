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

    //baud generator 16 bit high speed
    BAUD1CONbits.BRG16 = 1;
    TX1STAbits.BRGH = 1;

    //async mode
    TX1STAbits.SYNC = 0;

    //enable serial port
    RC1STAbits.SPED = 1;

    //enable TX and RX
    TX1STAbits.TXEN = 1;
    RC1STAbits.CREN = 1;

    //8 bits mode
    TX1STAbits.TX9 = 0;
    RC1STAbits.RX9 = 0;
    //9600 baud @ 16 MHZ
    SP1BRGH = 0x01;
    SP1BRGL = 0xA0; //~416



}