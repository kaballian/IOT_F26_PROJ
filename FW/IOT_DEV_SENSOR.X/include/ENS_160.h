#ifndef ENS_160_H
#define ENS_160_H

#include "utils.h"

/*driver for ENS160 I2C gas sensor*/
/*page 17 "IF the CSn pin is asserted low, the interface behaves as an SPI slave
, this condition is maintained until the next power on reset "

"The ENS160 is a I2C slave device with a fixed 7bit address 0x52 if the MISO/ADDR
line is held low at power or 0x53 if the MISO/ADDR line is held high"

Host system can read/write values to one the registers by first sending the single byte
register address. 
Ens160 features autto incrementt, so that it is possible to read/write multiple bytes
in rapid success (single transaction)
*/

/*device address*/
#define ENS_160_ADDR0   0x52
#define ENS_160_ADDR1   0x53
// (*) = only if not self explanatory
/*registers Page 25 |NAME| ADDR| Size (*)|DESCP(*) */ 
#define ENS160_REG_PART_ID      0x00
#define ENS160_PART_ID_EXPECTED 0x0160
/*OPMODE: 
0x00:DEEP SLEEP
0x01:IDLE MODE
0x02:STANDARD GAS SENSING MODE
0xF0:RESET*/
#define ENS160_REG_OPMODE       0x10
#define ENS160_OPMODE_DEEP_S    0x00
#define ENS160_OPMODE_IDLE      0x01    
#define ENS160_OPMODE_STANDARD  0x02
#define ENS160_OPMODE_RESET     0xF0




#define ENS160_REG_CONFIG       0x11
#define ENS160_REG_COMMAND      0x12
#define ENS160_REG_TEMP_IN      0x13    //2
#define ENS160_REG_RH_IN        0x15    //2 host relative humidity
#define ENS160_REG_STATUS       0x20    //operating mode (read)
#define ENS160_REG_DATA_AQI     0x21    //air quality index (read)
#define ENS160_REG_DATA_TVOC    0x22    //2 TVOC concentration
#define ENS160_REG_DATA_ECO2    0x24    //2 equivalent CO2 concentration (ppm)
#define ENS160_REG_DATA_T       0x30    //2 temperature used in calculation
#define ENS160_REG_DATA_RH      0x32    //2 relative humidity used in calculation
#define ENS160_REG_DATA_MISR    0x38    //1 data integrity field(optional)
#define ENS160_REG_DATA_GP_W    0x40    //8 general purpose write registers
#define ENS160_REG_DATA_GP_R    0x48    //8 general purpose read registers





typedef struct 
{
    uint8_t     dev_addr;
    uint16_t    part_id;
    
    uint8_t     dev_status;
    uint8_t     validity_flag;
    uint8_t     present;
    uint8_t     initialized;
    uint8_t     data_valid;


    uint8_t     aqi;   
    uint16_t    tvoc_ppb;
    uint16_t    eco2_ppm;
}ENS160_t;

bool ENS160_probe(uint8_t addr, uint16_t *part_id);
bool ENS160_init(ENS160_t *dev, uint8_t addr);
bool ENS160_read_status(ENS160_t *dev);
bool ENS160_read_data(ENS160_t *dev);
bool ENS160_set_opmode(ENS160_t *dev, uint8_t mode);
bool ENS160_write_env(ENS160_t *dev, int16_t temp_c_x100, uint16_t rh_x100);


#endif