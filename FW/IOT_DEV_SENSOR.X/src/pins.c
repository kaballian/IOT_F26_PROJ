#include "include/pins.h"
#include <pic16f18124.h>

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
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 0;


    /*
    ANSELx registers
    analog behavior, since were all GPIO , everthing i 0
    1 = analog input : 0 = digital I/O
    */

    ANSELAbits.ANSELA0 = 0;
    ANSELAbits.ANSELA1 = 0;
    ANSELAbits.ANSELA2 = 0;
    ANSELAbits.ANSELA4 = 0;
    ANSELAbits.ANSELA5 = 0;

    ANSELCbits.ANSELC0 = 0;
    ANSELCbits.ANSELC1 = 0;
    ANSELCbits.ANSELC2 = 0;
    ANSELCbits.ANSELC3 = 0;
    ANSELCbits.ANSELC4 = 0;
    ANSELCbits.ANSELC5 = 0;
    

    /*weak pull wups */
    
    WPUA = 0x0;
    WPUC = 0x0;

    /*open drain control*/
    ODCONA = 0x0;
    ODCONC = 0x0;

    /*slew rate*/
    SLRCONCbits.SLRC3 = 0;
    SLRCONCbits.SLRC4 = 0;

    /*input level: schmitt trigger*/
    INLVLCbits.INLVLC3 = 1;
    INLVLCbits.INLVLC4 = 1;


    //PPS
    /*note for I2C, bidirectinal pins, SDA SCL must be mapped to
    the same pin*/

    /*unlock sequence*/
    INTCONbits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    INTCONbits.GIE = 1;

    /*remap sequence*/
    /*
    NOTE:
    section 18.8.1 xxxPPS ("xxx" = any PPS input register from table 18-1)
    xxxPPS[5:3] => PORT[2:0] -> peripheral input PORT selection
    xxxPPS[2:0] => PIN[2:0] -> peripheral input PIN selection

    pin overview
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
    
    /* 
    PPS INPUTS
    RA4     =   UART RX             IN
    RC0     =   T1CLKIN             IN
    RC3     =   I2C SDA             BIDIRECTIONAL
    RC4     =   I2C SCL             BIDIRECTIONAL
    */
    /*          0b00 |[000:PORTA]|[100:PORTxPin4]*/
    RX1PPS      = 0b00000100;
    /*          0b00 |[010:PORTC]|[000:PORTxPin0] */
    T1CKIPPS    = 0b00010000;
    /*          0b00 |[010:PORTC]|[011:PORTxPin3] */
    SSP1DATPPS  = 0b00010011;
    /*          0b00 |[010:PORTC]|[100:PORTxPin4] */
    SSP1CLKPPS  = 0b00010100;

    /*
    PPS OUTPUS [table 18-2]
    RA5     =   UART TX             OUT
    RC1     =   PWM11               OUT
    RC2     =   PWM12               OUT
    RC3     =   I2C SDA             BIDIRECTIONAL
    RC4     =   I2C SCL             BIDIRECTIONAL
    RC5     =   STAT_LED            OUT (TMR0)
    */

    /*RxyPPS[5:0] <- entry from table [18-2]*/
    RA5PPS  = 0x13; //UART TX
    RC1PPS  = 0x0B; //PWM11
    RC2PPS  = 0x0C; //PWM12
    RC3PPS  = 0x1C; //SCA
    RC4PPS  = 0x1B; //SCL
    RC5PPS  = 0x01; // CLC1_out / status LED

    /*lock sequence*/
    INTCONbits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
    INTCONbits.GIE = 1;  


    /*interrupts?*/
}