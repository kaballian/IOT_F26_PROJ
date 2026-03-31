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
#include <pic16f18124.h>

int main(void) {
    
    /*set clocks and pins*/
    SYSTEM_init(); 
    

    return 1;
}
