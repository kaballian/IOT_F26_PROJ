#include "include/ENS_160.h"
#include "include/I2C.h"




/*convert little endian to  16 bit unsigned integer*/
static uint16_t ENS160_u16_from_le(const uint8_t *buf)
{
    return (uint16_t)buf[0] | ((uint16_t)buf[1] <<8);
}
bool ENS160_probe(uint8_t addr, uint16_t *part_id)
{
    /*probes the device for PART ID at register 0x00*/
    uint8_t buf[2];
    if(part_id == 0) //check for nullptr
    {
        return false;
    }
    /*read from device */
    if(!I2C2_read_reg(addr, ENS160_REG_PART_ID, buf, 2))
    {
        return false;
    }

    /*convert*/
    *part_id = ENS160_u16_from_le(buf);
    /*resource at part_id pointer now contains the part id info*/
    return true;

}
bool ENS160_init(ENS160_t *dev, uint8_t addr)
{
    /*initialized the device, extracts the part id
    and leaves it in stardard mode*/
    uint16_t part_id;
    if(dev == 0) //check for nullptr
    {
        return false;
    }

    dev->dev_addr       = addr;
    dev->present        = 0u;
    dev->initialized    = 0u;
    dev->data_valid     = 0u;

    //probe the device for PART id
    if(!ENS160_probe(addr, &part_id))
    {
        return false;
    }
    //see if the part id matches what is expected
    if(part_id != ENS160_PART_ID_EXPECTED)
    {
        return false;
    }
    dev->part_id = part_id; //transfer the ID
    dev->present = 1; //mark the device as "enabled"

    //set to idle
    if(!ENS160_set_opmode(dev, ENS160_OPMODE_IDLE))
    {
        return false;
    }
    //then set to standard 
    if(!ENS160_set_opmode(dev, ENS160_OPMODE_STANDARD))
    {
        return false;
    }

    dev->initialized = 1u;
    return true;
}
bool ENS160_read_status(ENS160_t *dev)
{
    /*reads the status register at 0x20 at writes it at
    the device status location in memory*/
    uint8_t status;

    if(dev == 0)
    {
        return false;
    }
    if(!I2C2_read_reg(dev->dev_addr, ENS160_REG_STATUS, &status, 1))
    {
        return false;
    }
    /*
    bits: (page 30 in datasheet)
    7 - STATAS - indicates OPMODE running
    6 - STATER - indicates an error has been detected, invalid operating mode selected
    5 - reserved
    4 - reserved
    2-3 - validity flag
    1 - NEWDAT - indicates new data is available in DATA_x, auto cleared at DATA_x read
    0 - NEWGPR - indicates new data is available in GPR_READx regs
    */
    dev->dev_status = status; //write status to memory
    /*extract validity flags (bits [2:3] in register)
    shift status twice and & wit 11 to extract 
    so is done to easier compare opmodes which are 0-3*/
    dev->validity_flag = (status >> 2) & 0b00000011;

    return true;

}
bool ENS160_read_data(ENS160_t *dev)
{
    uint8_t buf[5];
    
    if(dev == 0)
    {
        return false;
    }
    /*reads data from device.
    This makes use of the auto increment function implemented
    in device
    The first data register is 0x21 and after each read, the next is transmitted
    0x21:AQI
    0x22:TVOC 1/2   (little endian)
    0x23:TVOC 2/2   (little endian)
    0x24:CO2ppm 1/2 (little endian)
    0x25:CO2ppm 2/2 (little endian)

    data is written to buf
    */
    if(!I2C2_read_reg(dev->dev_addr, ENS160_REG_DATA_AQI, buf, 5))
    {
        return false;
    }

    /*shift for endianness*/
    dev->aqi        = buf[0];
    dev->tvoc_ppb   = (uint16_t)buf[1] | ((uint16_t)buf[2] << 8);
    dev->eco2_ppm   = (uint16_t)buf[3] | ((uint16_t)buf[4] << 8);

    //set the validity flag

    dev->data_valid = (dev->validity_flag != 3u);

}
bool ENS160_set_opmode(ENS160_t *dev, uint8_t mode)
{
    if(dev == 0) //check for nullptr
    {
        return false;
    }

    /*send the device address from pointer, the opmode register (0x10),
    reference to the mode, which is a single byte*/
    return I2C2_write_reg(dev->dev_addr, ENS160_REG_OPMODE, &mode, 1);
    /*expect a return true from function*/
}
bool ENS160_write_env(ENS160_t *dev, int16_t temp_c_x100, uint16_t rh_x100)
{
    /*
    {
    uint8_t buf[2];
    int32_t temp_k_x64;
    uint32_t rh_x512;

    if (dev == 0)
        return false;

    temp_k_x64 = ((int32_t)temp_c_x100 + 27315) * 64 / 100;
    rh_x512    = ((uint32_t)rh_x100 * 512u) / 100u;

    buf[0] = (uint8_t)(temp_k_x64 & 0xFF);
    buf[1] = (uint8_t)((temp_k_x64 >> 8) & 0xFF);

    if (!I2C2_write_reg(dev->addr, ENS160_REG_TEMP_IN, buf, 2))
        return false;

    buf[0] = (uint8_t)(rh_x512 & 0xFF);
    buf[1] = (uint8_t)((rh_x512 >> 8) & 0xFF);

    if (!I2C2_write_reg(dev->addr, ENS160_REG_RH_IN, buf, 2))
        return false;

    return true;
    }
    */
}