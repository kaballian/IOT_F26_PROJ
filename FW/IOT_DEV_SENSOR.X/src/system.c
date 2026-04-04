#include "../include/system.h"
#include "include/ENS_160.h"



/*
main driver for IOT sensor firmware
statemachine is expanded here
*/


void SYSTEM_init(void)
{
    //clock manager
    CLOCK_init();
    //pin manager
    PIN_MANAGER_init();
    /*interrupts*/
    ISR_init();
    /*PWM INIT*/
    PWM_init();
    /*I2C INIT*/
    I2C2_init();
    /*UART*/
    EUSART1_init();
    /*TMR1*/
    TMR1_CNT_init();
    /*TMR0*/
    TMR0_init();



}

/*flag helpers*/


static void st_init_entry(context_t *CTX)
{
     /*FANS*/
    FAN_init(&CTX->FAN1, PWM_set_duty, &PWM_FAN1_CH, 20);    
    FAN_init(&CTX->FAN2, PWM_set_duty, &PWM_FAN2_CH, 25);    

    /*I2C ENS160*/
    ENS160_init(&CTX->ENS160, ENS_160_ADDR0);


    /*ADG419*/
    ADG419_init(&CTX->FAN_selector, PIN_RA2);
    
    /*set initial duty cycle*/
    FAN_set_duty(&CTX->FAN1, 10);
    FAN_set_duty(&CTX->FAN2, 15);
    /*set opmode for ENS160*/
    ENS160_set_opmode(&CTX->ENS160, ENS160_OPMODE_STANDARD);
    

    CTX->init_flags |= (INIT_PWM1 | INIT_PWM2 | INIT_ENS160);
}
static transition_t st_init_handle(context_t *CTX, event_t ev, state_t current)
{
    switch (ev)
    {
    case INIT_COMP:
        return to(ST_MEAS);    
    default:
        return stay(current);
    }
}
static void st_idle_entry(context_t *CTX)
{
    /*manipulate a timer here to wait for a few seconds*/
}
static transition_t st_idle_handle(context_t *CTX, event_t ev,state_t current)
{
    /*once the idle time is over dispatch to next state*/
}
static void st_meas_entry(context_t *CTX)
{   
    /*FAN1 
    check switch position
    check duty cycle
    measure RPM
    write necessary data
    */

    /*repeat for FAN2*/

    /*acquire data from ENS160
    do calculations 
    write to memory*/


    /*indicate */
}
static transition_t st_meas_handle(context_t *CTX, event_t ev,state_t current)
{
    switch(ev)
    {
        case MEAS_FAN1_START:
        {
            /*check switch position*/
            ADG419_CHL_SELECT(&CTX->FAN_selector, CHL_1);    
            /*measure RPM*/
            FAN_CNT_start(&CTX->FAN1);
            /*return and wait for timing window to be done*/
            return stay(current);
        }
        case MEAS_FAN1_DONE:{
            FAN_CNT_stop(&CTX->FAN1);
            return stay(current);
        }
        case MEAS_FAN2_START:{
            ADG419_CHL_SELECT(&CTX->FAN_selector, CHL_2);
            FAN_CNT_start(&CTX->FAN2);
            return stay(current);
        }
        case MEAS_FAN2_DONE:{
            FAN_CNT_stop(&CTX->FAN2);
            return stay(current);
        } 
        case MEAS_ENS160:{

            if(!CTX->ENS160.initialized)
            {
                return stay(current);
            }
            if(!ENS160_read_status(&CTX->ENS160))
            {
                CTX->fault_flags |= FAULT_ENS160; //AMBIGUOUS FAULT FLAGS
                return stay(current);
            }
            if(CTX->ENS160.dev_status & 0x02)
            {
                if(!ENS160_read_data(&CTX->ENS160))
                {
                    CTX->fault_flags |= FAULT_ENS160;  //AMBIGUOUS FAULT FLAGS
                }
            }
            
            return stay(current);
        }

        default:
            return stay(current);

    }
}
static void st_comm_entry(context_t *CTX){}
static transition_t st_comm_handle(context_t *CTX, event_t ev, state_t current){}



static const state_ops_t OPS[ST_COUNT] =  
{
    [ST_INIT] = {st_init_entry,0,st_init_handle},
    [ST_IDLE] = {st_idle_entry,0,st_idle_handle},
    [ST_MEAS] = {st_meas_entry,0,st_meas_handle},
    [ST_COMM] = {st_comm_entry,0,st_comm_handle},
};

void FSM_transition(FSM_t *sm, state_t next)
{
    if(next==sm->state) return; //if the next state is the same, stay

    //exit old state
    if(OPS[sm->state].exit) //if an exit function exists
    {
        OPS[sm->state].exit(&sm->CTX); //they are allways 0, but can be added if necessary
    }

    //advance to next state
    sm->state = next;

    //call the entry of the next state
    if(OPS[sm->state].entry)
    {
        OPS[sm->state].entry(&sm->CTX);
    }
}

void FSM_init(FSM_t *sm)
{
    sm->state           = ST_INIT;
    sm->CTX.ms          = 0;
    sm->CTX.fault_flags = 0;
    sm->CTX.init_flags  = 0;
    sm->CTX.meas_head   = 0;
    sm->CTX.meas_count  = 0;


   

    if(OPS[sm->state].entry)
    {
        OPS[sm->state].entry(&sm->CTX);
    }


}

void FSM_dispatch(FSM_t *sm, event_t ev)    
{
    // const state_ops_t *s = &OPS[sm->state];
    // transition_t tr = stay();

    // if(s->handle)
    // {
    //     tr = s->handle(&sm->CTX, ev);
    // }
    // if(tr.changed)
    // {
    //     FSM_transition(sm, tr.next);
    // }

    transition_t tr = OPS[sm->state].handle(&sm->CTX, ev, sm->state);

    if(tr.changed)
    {
        FSM_transition(sm, tr.next);
    }
}