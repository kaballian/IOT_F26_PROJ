/*
 * File:   newmain.c
 * Author: alext
 *
 * Created on March 29, 2026, 10:07 PM
 */


/*
Builds on taillight GOF style state machine
defined in system.h


*/




#include "include/event_queue.h"
#include "include/system.h"
#include <xc.h>
/* HARDWARE INSTANTIATIONS*/
/*statemachine and event queue*/
static FSM_t sm;
static event_q_t ev_q;



volatile uint8_t g_fsm_tick_f   = 0;    //HARDWARE FLAG
volatile uint8_t g_uart_rx_f    = 0;    //HARDWARE FLAG
volatile uint8_t g_fan_f        = 0;    //SOFTWARE FLAG
volatile uint8_t g_ENS160_f     = 0;    //SOFTWARE FLAG
volatile uint32_t g_sys_ms      = 0;    //sys tick counter
volatile uint32_t g_fan_deadline   = 0; //fan deadline check
    


/*handling wrapper*/
static inline void APP_post_event(event_t ev)
{
    (void)EVENT_Q_push(&ev_q, ev);
}

void APP_init(void)
{
    EVENT_Q_init(&ev_q);
    FSM_init(&sm);
}

void APP_service(void)
{
    
    if(g_fsm_tick_f)
    {
        g_fsm_tick_f = 0;
        g_sys_ms++;
        sm.CTX.sys_ms++;

        APP_post_event(TMRTick); // this is where we kick the rock
        
        if(sm.CTX.gate_active && (g_sys_ms >= sm.CTX.gate_deadline))
        {
            sm.CTX.gate_active = 0;

            switch (sm.CTX.gate_owner)
            {
            case GATE_F1:{
                sm.CTX.gate_owner = GATE_NONE;
                APP_post_event(MEAS_FAN1_DONE);
                break;
            }
            case GATE_F2:{
                sm.CTX.gate_owner = GATE_NONE;
                APP_post_event(MEAS_FAN2_DONE);
                break;
            }
            case GATE_ENS160:{
                sm.CTX.gate_owner = GATE_NONE;
                APP_post_event(MEAS_ENS160_DONE);
                break;
            }
            
            default:
                sm.CTX.gate_owner = GATE_NONE;
                break;
            }
        }
    }
    
    if(g_uart_rx_f)
    {
        /*UART receive interrupt flag*/
        g_uart_rx_f = 0;
        APP_post_event(UART);
    }
}

/*dispatch APP events into FSM*/


void APP_dispatch_events(void)
{
    event_t ev;
    while(EVENT_Q_pop(&ev_q, &ev))
    {
        FSM_dispatch(&sm, ev);
    }
}


int main(void) {
    
    /*set clocks and pins*/
    SYSTEM_init(); 
    APP_init();
   
    INTCONbits.GIE = 1;
    
    
    // if(sm.CTX.init_flags == (INIT_PWM1 | INIT_PWM2 | INIT_ENS160))
    // {
    //     FSM_dispatch(&sm, INIT_COMP);
    // }

    while(1)
    {
        APP_service();
        APP_dispatch_events();
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
    - HW abstraction (check)

Tach reader / converter logic
    - TMR1 counter reader (check)
    - TMRn reading period (check)
    - (2 poles on fan) -> divide by 2 (check)
    - store reading 
import I2C library for sensors 
    - ENS160 (made from scratch, check)
    - BME280 (coming)






UART ISR RX/TX
    - still missing 

context_t scheduling step - next_step, decide what comes after a given measurement
right now, the measurement sequence is buried inside the FSM, more specifically
inside the fn_handle when a measurement is complete, the only way to set
a new gate_owner is in a state_entry, which is not ideal.






FAN timing reasoning
FSM starts a measurement
tach starts counting
software stores a deadline (500 sys ticks)
timer 0 counts to this deadline
APP_service advances software time on each tick
deadline reached -> APP_service posts a fan_done event
FSM gets/handles the event and stops the counter
FSM handler reads count and converts to RPM
FSM moves to next measurement step



MACHINE HEIRACRHY
ISR - reports hardware facts, either RX buffer is full or a timer tick happend.
APP_serice - turns flags into events
event_queue - stores the events in order, FIFO, no decision making
FSM, leaves ST_INIT when init is complete, then rests in IDLE until APP_service
determines something needs to be done


*/