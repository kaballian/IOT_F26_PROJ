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

int main(void) {
    
    /*set clocks and pins*/
    SYSTEM_init(); 

    // static fan_t fan1; 
    // static fan_t fan2; 
    static FSM_t sm;


    /*PWM INIT*/
    PWM_init();
    /*I2C INIT*/
    I2C2_init();

    FSM_init(&sm);
    


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