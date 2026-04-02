#ifndef ENS_160_H
#define ENS_160_H

#include "utils.h"

/*driver for ENS160 I2C gas sensor*/

#define ENS_160_ADDR0   0x52u
#define ENS_160_ADDR1   0x53u
#define ENS160_REG_PART_ID      0x00u
#define ENS160_REG_OPMODE       0x10u
#define ENS160_REG_STATUS       0x20u
#define ENS160_REG_DATA_AQI     0x21u
#define ENS160_REG_DATA_TVOC    0x22u
#define ENS160_REG_DATA_ECO2    0x24u
#define ENS160_OPMODE_IDLE      0x01u
#define ENS160_OPMODE_STANDARD  0x02u



typedef struct 
{
    uint8_t     addr;
    uint16_t    part_id;
    uint8_t     status;
    uint8_t     aqi;
    uint16_t    tvoc_ppb;
    uint16_t    eco2_ppm;
    uint8_t     present;
    uint8_t     initialized;
}ENS160_t;

bool ENS160_probe(uint8_t addr, uint16_t *part_id);
bool ENS160_init(ENS160_t *dev, uint8_t addr);
bool ENS160_read_status(ENS160_t *dev);
bool ENS160_read_data(ENS160_t *dev);


#endif