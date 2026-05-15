#ifndef BME_280_H
#define BME_280_H


#include "utils.h"



/*
driver and abstraction for BME280 temperature and humidity sensor

https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf

IMPORTANT DETAIL
page 14
It is prohibited to keep any interface pin (SDI, SDO, SCK or CSB) at a logical high level when
Vddio is switched off

side note
page 23
In I2C mode, even if preassure was not measured, reading unused registers is faster
than reader temperature and humidity data separately

data readyout is done by start a burst read from 0xF7 to 0xFC (temp and pressure)
or from 0xF7 to 0xFE (temp, pressure and humidity)


*/

/*keep in mind that results are in some kind of fixed point format*/

#define BME280_ADDR0 0x76 //SDO -> GND (most likely this)
#define BME280_ADDR1 0x77 //SDO -> VDDIO 



#define BME280_hum_lsb      0xFE
#define BME280_hum_msb      0xFD
#define BME280_temp_xlsb    0xFC
#define BME280_temp_lsb     0xFB
#define BME280_temp_msb     0xFA
/*
pressure data
[19:16] extended LSB   data sheet says 0xF9 (bit 7,6,5,4) -> press_xlsb [3:0] ??
[15:8] LSB
[7:0] MSB
*/
#define BME280_press_xlsb   0xF9
#define BME280_press_lsb    0xF8
#define BME280_press_msb    0xF7
/*
[7:5] control inactive time in normal mode 
[4:2] control time constant of IIR filter 
[0]     enable 3 wire SPI interface (NOT USED)*/
#define BME280_config       0xF5 

/*[7:5] osrs_t(oversampling temp), [4:2] osrs_p(oversampling pres), [1:0] mode(mode of device)*/   
#define BME280_ctrl_meas    0xF4 
#define BME280_status       0xF3 /*[3] '1' when conv is running. [1] when nvm is written to*/
#define BME280_ctrl_hum     0xF2 /*sets sets humi data acq options, only active after ctrl_meas is written.  oversampling of humi [2:0] s^n*/
#define BME280_reset        0xE0 /*if 0xB6 is written, device resets*/
#define BME280_id           0xD0 /*should read as 0x60*/


/*calib 26-41 0xE1 - 0xF0*/

// READ ONLY
#define BME280_calib_00       0x88
#define BME280_calib_01       0x89
#define BME280_calib_02       0x8A
#define BME280_calib_03       0x8B
#define BME280_calib_04       0x8C
#define BME280_calib_05       0x8D
#define BME280_calib_06       0x8E
#define BME280_calib_07       0x8F
#define BME280_calib_08       0x90
#define BME280_calib_09       0x91
#define BME280_calib_10       0x92
#define BME280_calib_11       0x93
#define BME280_calib_12       0x94
#define BME280_calib_13       0x95
#define BME280_calib_14       0x96
#define BME280_calib_15       0x97
#define BME280_calib_16       0x98
#define BME280_calib_17       0x99
#define BME280_calib_18       0x9A
#define BME280_calib_19       0x9B
#define BME280_calib_20       0x9C
#define BME280_calib_21       0x9D
#define BME280_calib_22       0x9E
#define BME280_calib_23       0x9F
#define BME280_calib_24       0xA0
#define BME280_calib_25       0xA1
#define BME280_calib_26       0xE1
#define BME280_calib_27       0xE2
#define BME280_calib_28       0xE3
#define BME280_calib_29       0xE4
#define BME280_calib_30       0xE5
#define BME280_calib_31       0xE6
#define BME280_calib_32       0xE7
#define BME280_calib_33       0xE8
#define BME280_calib_34       0xE9
#define BME280_calib_35       0xEA
#define BME280_calib_36       0xEB
#define BME280_calib_37       0xEC
#define BME280_calib_38       0xED
#define BME280_calib_39       0xEE
#define BME280_calib_40       0xEF
#define BME280_calib_41       0xF0




typedef struct 
{
    uint8_t temporary_storage;
    // uint32_t 
}BME280_t;



bool BME280_init(BME280_t *dev);



































#endif