/*
 * File:   newmain.c
 * Author: alext
 *
 * Created on March 29, 2026, 10:07 PM
 */


/*
Builds on taillight GOF style state machine
defined inm system.h


*/



#include "include/system.h"
#include <xc.h>

#include "include/FAN.h"
#include "include/PWM.h"




/* HARDWARE INSTANTIATIONS*/
static fan_t fan1; 
static fan_t fan2; 

int main(void) {
    
    /*set clocks and pins*/
    SYSTEM_init(); 

    /*PWM INIT*/
    PWM_init();
    /*FANS*/
    FAN_init(&fan1, PWM_set_duty, &PWM_FAN1_CH, 0, 499);    
    FAN_init(&fan2, PWM_set_duty, &PWM_FAN2_CH, 0, 499);    

    

    return 1;
}


/*

TODO:

FAN driver
    - PWM setup
    - PWM abstraction
        - store setting
Tach reader / converter logic
    - TMR1 counter reader
    - TMRn reading period
    - (2 poles on fan) -> divide by 2
    - store reading
import I2C library for sensors


UART ISR RX/TX

*/