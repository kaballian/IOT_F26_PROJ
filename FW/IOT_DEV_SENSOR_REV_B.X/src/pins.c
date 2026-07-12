#include "include\pins.h"
#include <pic16f18156.h>

void PIN_MANAGER_init(void)
{
    /*LATx registers*/

    LATA = 0x0;
    LATB = 0x0;
    LATC = 0x0;

    /*TRISx registers
    1: input
    0: output
    
    PORT    = |FUNCTION|DIRECTION|VAL|PPS
    RA0     = TAC_SEL_INH   OUT X
    RA1     = TAC_SEL_C     OUT
    RA2     = TAC_SEL_B     OUT
    RA3     = TAC_SEL_A     OUT
    RA4     = RX            IN  1   PPS
    RA5     = TX            OUT 0   PPS

    RB0     = PWM21         OUT 0   PPS
    RB1     = PWM22         OUT 0   PPS
    RB2     = I_FAN_3       IN  1   
    RB3     = I_FAN_4       IN  1
    RB4     = PWR_MEAS_12V  IN  1
    RB5     = T1CLKIN       IN  1   PPS
    
    RC0     = unconnected           
    RC1     = unconnected
    RC2     = I2C SDA       BI      PPS
    RC3     = I2C SCL       BI      PPS
    RC4     = PWM11         OUT 0   PPS
    RC5     = PWM12         OUT 0   PPS
    RC6     = I_FAN_1       IN  1    
    RC7     = I_FAN_2       IN  1


    */

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 0;

    TRISBbits.TRISB0 = 0;// RB0     = PWM21         OUT 0   PPS
    TRISBbits.TRISB1 = 0;// RB1     = PWM22         OUT 0   PPS
    TRISBbits.TRISB2 = 1;// RB2     = I_FAN_3       IN  1   
    TRISBbits.TRISB3 = 1;// RB3     = I_FAN_4       IN  1
    TRISBbits.TRISB4 = 1;// RB4     = PWR_MEAS_12V  IN  1
    TRISBbits.TRISB5 = 1;// RB5     = T1CLKIN       IN  1   PPS
    
    // TRISCbits.TRISC// RC0     = unconnected  MAKE HI Z           
    // TRISCbits.TRISC// RC1     = unconnected  MAKE HI Z
    TRISCbits.TRISC2 = 1;// RC2     = I2C SDA       BI      PPS
    TRISCbits.TRISC3 = 1;// RC3     = I2C SCL       BI      PPS
    TRISCbits.TRISC4 = 0;// RC4     = PWM11         OUT 0   PPS
    TRISCbits.TRISC5 = 0;// RC5     = PWM12         OUT 0   PPS
    TRISCbits.TRISC6 = 1;// RC6     = I_FAN_1       IN  1    
    TRISCbits.TRISC7 = 1;// RC7     = I_FAN_2       IN  1

    /*ANSELx registers
    analog control
    1 = analog input
    0 = digital i/o*/
    ANSELAbits.ANSELA0 = 0;
    ANSELAbits.ANSELA1 = 0;
    ANSELAbits.ANSELA2 = 0;
    ANSELAbits.ANSELA3 = 0;
    ANSELAbits.ANSELA4 = 0;
    ANSELAbits.ANSELA5 = 0;

    ANSELBbits.ANSELB0 = 0;
    ANSELBbits.ANSELB1 = 0;
    ANSELBbits.ANSELB2 = 1; //ADC
    ANSELBbits.ANSELB3 = 1; //ADC
    ANSELBbits.ANSELB4 = 1; //ADC
    ANSELBbits.ANSELB5 = 0;

    // ANSELCbits.ANSELC0 = 0; unconnected
    // ANSELCbits.ANSELC1 = 0; unconnected
    ANSELCbits.ANSELC2 = 0;
    ANSELCbits.ANSELC3 = 0;
    ANSELCbits.ANSELC4 = 0;
    ANSELCbits.ANSELC5 = 0;
    ANSELCbits.ANSELC6 = 1; //ADC
    ANSELCbits.ANSELC7 = 1; //ADC

    /* weak pull ups*/
    WPUA = 0x0;
    WPUB = 0x0;
    WPUC = 0x0;

    /*open drain control*/
    ODCONA = 0x0;
    ODCONB = 0x0;
    ODCONC = 0x0;

    /*slew rate*/
    SLRCONBbits.SLRB2 = 0;
    SLRCONBbits.SLRB3 = 0;

    /*input level: schmitt trigger*/
    INLVLBbits.INLVLB2 = 1;
    INLVLBbits.INLVLB3 = 1;

    // PPS
    /*note for I2C: bidirectional pins, SDA/SCL must be mapped to same pin*/

    /*unlock sequence*/
    INTCONbits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    INTCONbits.GIE = 1;
    /*remap sequence*/
    /*
    note section 18.8.1 xxxPPS ("XXX" = any PPS inpuit register from table 18-1)
    xxxPPS[5:3] => PORT[2:0] -> peripheral input PORT selection 
    xxxPPS[2:0] => PIN[2:0] -> peripheral input PIN selection*/
    /*inputs*/
    RX1PPS      = 0b00000100; // 0b00|[000:PORTA]|[100:PORTxPin4]
    T1CKIPPS    = 0b00001101; // 0b00|[001:PORTB]|[101:PORTxPin5]
    SSP2DATPPS  = 0b00010010; // 0b00|[010:PORTC]|[010:PORTxPin2]
    SSP2CLKPPS  = 0b00010011; // 0b00|[010:PORTC]|[011:PORTxPin3]

    /*outputs*/
    /*RxyPPS[5:0] <- entry from table[18-2] */
    RA5PPS = 0x13; //TX
    RC4PPS = 0x0B; //PWM11
    RC5PPS = 0x0C; //PWM12
    RB0PPS = 0x0D; //PWM13
    RB1PPS = 0x0E; //PWM14
    RC2PPS = 0x1E; //SDA2
    RC3PPS = 0x1D; //SCL2


    /*lock sequence*/
    INTCONbits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
    INTCONbits.GIE = 1;

};