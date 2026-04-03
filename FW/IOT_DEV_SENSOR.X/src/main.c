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







/* HARDWARE INSTANTIATIONS*/

int main(void) {
    
    /*set clocks and pins*/
    SYSTEM_init(); 

    // static fan_t fan1; 
    // static fan_t fan2; 
    static FSM_t sm;


    

    FSM_init(&sm);
    
    if(sm.CTX.init_flags == (INIT_PWM1 | INIT_PWM2 | INIT_ENS160))
    {
        FSM_dispatch(&sm, INIT_COMP);
    }

    return 1;
}


/*

TODO:

FAN driver
    - PWM setup (check)
    - PWM abstraction (check)
        - store setting (check)

Analog switch
    - HW abstraction

Tach reader / converter logic
    - TMR1 counter reader
    - TMRn reading period
    - (2 poles on fan) -> divide by 2
    - store reading
import I2C library for sensors 
    - ENS160 (made from scratch, check)
    - BME280 (coming)


UART ISR RX/TX

*/