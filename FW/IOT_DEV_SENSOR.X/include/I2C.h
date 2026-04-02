#ifndef I2C_H
#define I2C_H

#include <xc.h>
#include "utils.h"
bool I2C2_init(void);
static bool I2C2_start(void);
static bool I2C2_stop(void);
static bool I2C2_write(uint8_t data);
static bool I2C2_wait_idle(void);
static bool I2C2_restart(void);
static bool I2C2_write_byte(uint8_t byte);
bool I2C2_read_byte(uint8_t *byte, bool ack);
bool I2C2_write_reg(
    uint8_t addr7,
    uint8_t reg,
    const uint8_t *data, 
    uint8_t len
); 
bool I2C2_read_reg(
    uint8_t addr7,
    uint8_t reg,
    uint8_t *data,
    uint8_t len
);



#endif

