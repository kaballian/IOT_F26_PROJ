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

    


    /*ADG419*/
    ADG419_init(&CTX->FAN_selector, PIN_RA2);
    
    /*set initial duty cycle*/
    FAN_set_duty(&CTX->FAN1, 10);
    FAN_set_duty(&CTX->FAN2, 15);


    /*I2C ENS160*/
    ENS160_init(&CTX->ENS160, ENS_160_ADDR0);

    /*set opmode for ENS160*/
    ENS160_set_opmode(&CTX->ENS160, ENS160_OPMODE_STANDARD);
    

    CTX->init_flags |= (INIT_PWM1 | INIT_PWM2 | INIT_ENS160 | INIT_I2C);
    CTX->comm_i2c_flags = NO_COMM;
}
static transition_t st_init_handle(context_t *CTX, event_t ev, state_t current)
{
    switch (ev)
    {
    case INIT_COMP:
        return to(ST_IDLE);    
    default:
        return stay(current);
    }
}
static void st_idle_entry(context_t *CTX)
{
    /*manipulate a timer here to wait for a few seconds*/
    (void)CTX;
}
static transition_t st_idle_handle(context_t *CTX, event_t ev,state_t current)
{
    /*once the idle time is over dispatch to next state*/
    /*This is where the system is left after ST_INIT, if nothing is in use
    the program automatically start from here after a timer0 tick 
    dispatched from the application layer*/


    switch(ev)
    {
        case MEAS_FAN1_START:
            return to(ST_MEAS_F1);        
        case MEAS_FAN2_START:
            return to(ST_MEAS_F2);
        case MEAS_ENS160_START:
            return to(ST_MEAS_ENS160);
        default:
            return stay(current);
    }
}


static void st_meas_f1_entry(context_t *CTX)
{
    /*check switch position*/
    ADG419_CHL_SELECT(&CTX->FAN_selector, CHL_1);    
    /*measure RPM*/
    FAN_CNT_start(&CTX->FAN1);
    /*indicate ownership of resource*/
    CTX->gate_owner     = GATE_F1;
    CTX->gate_active    = 1;
    CTX->gate_deadline  = g_sys_ms + 500;
    CTX->has_deadline   = 1;

}
static transition_t st_meas_f1_handle(context_t *CTX, event_t ev, state_t current)
{
    switch (ev)
    {
    case MEAS_FAN1_DONE:
        FAN_CNT_stop(&CTX->FAN1);
        CTX->gate_active = 0;
        CTX->has_deadline = 0;
        return to(ST_IDLE);
    default:
        return stay(current);
    }
}
static void st_meas_f2_entry(context_t *CTX)
{
    /*check switch position*/
    ADG419_CHL_SELECT(&CTX->FAN_selector, CHL_2);    
    /*measure RPM*/
    FAN_CNT_start(&CTX->FAN2);
    CTX->gate_owner     = GATE_F2;
    CTX->gate_active    = 1;
    CTX->gate_deadline  = g_sys_ms + 500;
    CTX->has_deadline   = 1;
    
}
static transition_t st_meas_f2_handle(context_t *CTX, event_t ev, state_t current)
{
    switch (ev)
    {
    case MEAS_FAN2_DONE:
        FAN_CNT_stop(&CTX->FAN2);
        CTX->gate_active = 0;
        CTX->has_deadline = 0;
        return to(ST_IDLE);
    default:
        return stay(current);
    }
}




static void st_meas_ens160_entry(context_t *CTX)
{
    CTX->gate_owner         = GATE_ENS160;
    CTX->gate_active        = 1;
    CTX->comm_i2c_flags     = COMM_INIT;
    CTX->has_deadline       = 0;
    
    // CTX->gate_deadline = CTX->sys_ms + 1000; //give it a full second
    /* ens160 communcation should not have a deadline*/

}
static transition_t st_meas_ens160_handle(context_t *CTX, event_t ev, state_t current)
{
    /*
    probe dev
    read status
    read data
     */
    switch(ev)
    {
        case MEAS_ENS160_READ:
        {
            if(!CTX->ENS160.initialized)
            {
                return stay(current);
            }
            if(!ENS160_read_status(&CTX->ENS160))
            {
                CTX->fault_flags |= FAULT_ENS160;
                return to(ST_IDLE);
            }
            if(CTX->ENS160.dev_status & 0x02)
            {
                if(!ENS160_read_data(&CTX->ENS160))
                {
                    CTX->fault_flags |= FAULT_ENS160;
                }
            }
            /*keep the state at let the app layer dispatch again*/   
            CTX->comm_i2c_flags = COMM_COMP;
            return stay(current); 
        }   
        case MEAS_ENS160_DONE:
        {   
            /*indicate the communication with the device is done
            and return FSM to idle mode, let the app queue decide what comes next*/
            CTX->comm_i2c_flags = NO_COMM;
            return to(ST_IDLE);
        } 

        default:
            return stay(current);
    }
}






static void st_comm_entry(context_t *CTX)
{}
static transition_t st_comm_handle(context_t *CTX, event_t ev, state_t current)
{}


static void st_set_f1_entry(context_t *CTX){}
static transition_t st_set_f1_handle(context_t *CTX, event_t ev, state_t current){}
static void st_set_f2_entry(context_t *CTX){}
static transition_t st_set_f2_handle(context_t *CTX, event_t ev, state_t current){}








static const state_ops_t OPS[ST_COUNT] =  
{
    [ST_INIT] = {st_init_entry,0,st_init_handle},
    [ST_IDLE] = {st_idle_entry,0,st_idle_handle},
    [ST_MEAS_F1] = {st_meas_f1_entry, 0, st_meas_f1_handle}, 
    [ST_MEAS_F2] = {st_meas_f2_entry, 0, st_meas_f2_handle},
    [ST_MEAS_ENS160] = {st_meas_ens160_entry, 0 , st_meas_ens160_handle},
    [ST_COMM] = {st_comm_entry,0,st_comm_handle},
    [ST_SET_F1] = {st_set_f1_entry, 0, st_set_f1_handle},
    [ST_SET_F2] = {st_set_f2_entry, 0, st_set_f2_handle}
};

/*states for setting fan speeds should be added here*/


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
    sm->CTX.sys_ms      = 0;
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