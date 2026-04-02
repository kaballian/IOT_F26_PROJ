#include "include/I2C.h"
#include <pic16f18124.h>


#define I2C2_TIMEOUT 10000u

bool I2C2_init(void)
{
    SSP2CON1 = 0x00;
    SSP2CON2 = 0x00;
    SSP2CON3 = 0x00;

    /*slew rate control*/
    SSP2STAT = 0x80; //SMP=1 -> standard mode (100KHz)

    /*Baud, 100KHz @ 16MHz*/
    SSP2ADD = 39;
    /*enable host mode */
    SSP2CON1bits.SSPM = 0b1000;
    SSP2CON1bits.SSPEN = 1;
}
static bool I2C2_start(void)
{
    /*initiate start condition*/
    uint16_t timeout = I2C2_TIMEOUT;

    if(!I2C2_wait_idle())
    {
        return false;
    }
    SSP2CON2bits.SEN = 1;

    while((SSP2CON2bits.SEN != 0u) && (timeout > 0u))
    {
        timeout--;
    }

    return (timeout > 0u);
}
static bool I2C2_stop(void)
{
    uint16_t timeout = I2C2_TIMEOUT;
    if(!I2C2_wait_idle())
    {
        return false;
    }
    SSP2CON2bits.PEN = 1;
    while((SSP2CON2bits.PEN != 0u) && (timeout > 0u))
    {
        timeout--;
    }
    return (timeout > 0u);
}
/*probing purposes only*/
static bool I2C2_write(uint8_t data)
{
    SSP2BUF = data;
    while(SSP2STATbits.BF); //wait for transfer complete
    while(SSP2CON2bits.ACKSTAT); //wait for acknowledge

    return (SSP2CON2bits.ACKSTAT == 0); //the the acknowledge condition
}
static bool I2C2_write_byte(uint8_t byte)
{
    /*after writing to SSPxBUF, we wait for transfer to finish end inspect acknowled*/
    uint16_t timeout = I2C2_TIMEOUT;

    if(!I2C2_wait_idle())
    {
        return false;
    }
    SSP2BUF = byte;
    while ((((SSP2STAT & 0x01u) != 0u) || ((SSP2CON2 & 0x1Fu) != 0u)) && (timeout > 0u))
    {
        timeout--;
    }
    if (timeout == 0u)
        return false;

    return (SSP2CON2bits.ACKSTAT == 0u);

}
static bool I2C2_restart()
{
    uint16_t timeout  = I2C2_TIMEOUT;
    if(!I2C2_wait_idle())
    {
        return false;
    }

    SSP2CON2bits.RSEN = 1;

    while((SSP2CON2bits.RSEN != 0u) && (timeout > 0u))
    {
        timeout--;
    }

    return (timeout > 0u);
}

bool I2C2_read_byte(uint8_t *byte, bool ack) 
{
   uint16_t timeout = I2C2_TIMEOUT;

    if (byte == 0)
        return false;

    if (!I2C2_wait_idle())
        return false;

    SSP2CON2bits.RCEN = 1;

    while ((SSP2STATbits.BF == 0u) && (timeout > 0u))
    {
        timeout--;
    }

    if (timeout == 0u)
        return false;

    *byte = SSP2BUF;

    timeout = I2C2_TIMEOUT;

    SSP2CON2bits.ACKDT = (ack ? 0u : 1u);   // 0 = ACK, 1 = NACK
    SSP2CON2bits.ACKEN = 1;

    while ((SSP2CON2bits.ACKEN != 0u) && (timeout > 0u))
    {
        timeout--;
    }

    return (timeout > 0u); 
}


static bool I2C2_wait_idle()
{
    uint16_t timeout = I2C2_TIMEOUT;
    while((((SSP2STAT & 0x04u) != 0u) || ((SSP2CON2 & 0x1Fu) != 0u)) && (timeout > 0u))
    {
        timeout--;
    }

    return (timeout > 0u);
}
/*public write function*/
/*START -> slave addr + write -> register -> payloads -> STOP*/
bool I2C2_write_reg(
    uint8_t addr7,
    uint8_t reg,
    const uint8_t *data, 
    uint8_t len
)
{
    uint8_t i;
    bool ok = true;
    //guard against no data
    if((data == 0) && (len > 0))
    {
        return false;
    }


    ok = I2C2_start(); //get start condition 
    //if we cant start - abort
    if(!ok)
    {
        return false;
    }
    


    //indicate which address we want to write to
    /*addr is 7 bits, but is shifted out MSP first, so must be shifted an addition 1 position*/
    ok = I2C2_write_byte((uint8_t)(addr7 << 1));
    /*if not ok, abort*/
    if(!ok)
    {
        I2C2_stop();
        return false;
    }
    //indicate which register
    ok = I2C2_write_byte(reg);
    if(!ok)
    {
        I2C2_stop();
        return false;
    }

    //send payload
    for(i = 0; i < len; i++)
    {
        ok = I2C2_write_byte(data[i]);
        if(!ok)
        {
            I2C2_stop();
            return false;
        }
    }

    ok = I2C2_stop();
    return ok;
}

/*public read function*/
/*START -> slave addr + write -> register -> restart?
 -> slave addr + read -> read N bytes -> STOP */
bool I2C2_read_reg(
    uint8_t addr7,
    uint8_t reg,
    uint8_t *data,
    uint8_t len
)
{
    uint8_t i;
    bool ok = true;
    /*avoid asking for nothing*/
    if((data == 0) || (len == 0))
    {
        return false;
    }

    
    /*START*/
    ok = I2C2_start();
    if(!ok)
    {
        return false;
    }
    /*slave addr + write*/
    ok = I2C2_write_byte((uint8_t)(addr7 << 1)); // same reasoning af writing
    if(!ok)
    {
        I2C2_stop();
        return false;
    }

    /*indicate register*/
    ok = I2C2_write_byte(reg); // same reasoning af writing
    if(!ok)
    {
        I2C2_stop();
        return false;
    }
    /*restart?*/
    ok = I2C2_restart(); // same reasoning af writing
    if(!ok)
    {
        I2C2_stop();
        return false;
    }
    /*slave addr + read */
    ok = I2C2_write_byte((uint8_t)((addr7<<1) |0x01u));
    if(!ok)
    {
        I2C2_stop();
        return false;
    }
    for(i = 0; i < len; i++)
    {
        ok = I2C2_read_byte(&data[i], (i<(len-1)));
        if(!ok)
        {
            I2C2_stop();
            return false;
        }
    }
    ok = I2C2_stop();
    return ok;

}

