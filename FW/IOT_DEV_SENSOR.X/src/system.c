#include "../include/system.h"


#include "include/eusart1.h"
#include "include/system.h"
#include <pic16f18124.h>



/*
main driver for IOT sensor firmware
statemachine is expanded here
*/




// void SYSTEM_init(void)
// {
//     /*for testing purposes, this is moved to main*/
//     // //clock manager
//     // CLOCK_init();
//     // //pin manager
//     // PIN_MANAGER_init();
//     // /*interrupts*/
//     // ISR_init();
//     // /*PWM INIT*/
//     // PWM_init();
//     // /*I2C INIT*/
//     // I2C2_init();
//     // /*UART*/
//     // EUSART1_init();
//     // /*TMR1*/
//     // TMR1_CNT_init();
//     // /*TMR0*/
//     // TMR0_init();

    


// }

/*flag helpers*/


// static void st_init_entry(context_t *CTX)
// {
//      /*FANS*/
//     // FAN_init(&CTX->FAN1, PWM_set_duty, &PWM_FAN1_CH, 20);    
//     // FAN_init(&CTX->FAN2, PWM_set_duty, &PWM_FAN2_CH, 25);    
//     //FAN_init(&CTX->FANS[FAN_1], &PWM_FAN1_CH, 20);
//     //FAN_init(&CTX->FANS[FAN_2], &PWM_FAN2_CH, 30);


    


//     /*ADG419*/
//     //ADG419_init(&CTX->FAN_selector, PIN_RA2);
    
//     /*set initial duty cycle*/
//     //FAN_set_duty(&CTX->FAN1, 10);
//     //FAN_set_duty(&CTX->FAN2, 15);


//     /*I2C ENS160*/
//     //ENS160_init(&CTX->ENS160, ENS_160_ADDR0);

//     /*set opmode for ENS160*/
//     //ENS160_set_opmode(&CTX->ENS160, ENS160_OPMODE_STANDARD);
    

//     CTX->init_flags |= (INIT_PWM1 | INIT_PWM2 | INIT_ENS160 | INIT_I2C);
//     CTX->comm_i2c_flags = NO_COMM;
   
// }
// static transition_t st_init_handle(context_t *CTX, event_t ev, state_t current)
// {
//     switch (ev)
//     {
//     case INIT_COMP:
//         return to(ST_IDLE);    
//     default:
//         return stay(current);
//     }
// }
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
        case MEAS_FAN2_START:
        case MEAS_FAN_START:
            return to(ST_MEAS_FAN);
        case MEAS_ENS160_START:
            return to(ST_MEAS_ENS160);
       
            /*because setting a fan is regarded as instantaneous and
        not reserving any actual hardware, it is handled in the IDLE state*/
        case SET_FAN:{
            fan_t *fan = &CTX->FANS[CTX->active_fan];
            FAN_set_duty(fan, fan->duty_percent);
            return stay(current);
        }
        


        /*UART TX CASES*/
        case UART_RESP:
            return to(ST_COMM);

        default:
            return stay(current);
    }
}


static void st_meas_fan_entry(context_t *CTX)
{
    /*check position*/
    // if(CTX->active_fan == FAN_1)
    // {
    //     CTX->gate_owner = GATE_F1;
    //     CTX->gate_active = 1;
    //     CTX->gate_deadline = g_sys_ms + 500;
    //     CTX->has_deadline = 1;
    //     ADG419_CHL_SELECT(&CTX->FAN_selector, CHL_1);
    //     FAN_CNT_start(&CTX->FAN1);

    // }
    // else if(CTX->active_fan == FAN_2)
    // {
    //     CTX->gate_owner = GATE_F2;
    //     CTX->gate_active = 1;
    //     CTX->gate_deadline = CTX->sys_ms + 500;
    //     CTX->has_deadline = 1;
    //     ADG419_CHL_SELECT(&CTX->FAN_selector, CHL_2);
    //     FAN_CNT_start(&CTX->FAN2);
    // }

    CTX->gate_owner = GATE_FAN;
    CTX->gate_active = 1;
    CTX->gate_deadline = g_sys_ms + 500;
    CTX->has_deadline = 1;
    /* 
    enum match with fan_sel_t and switch_sel_t
    chl1 = 0 -> fan1 = 0
    chl2 = 1 -> fan2 = 1
    */
    ADG419_CHL_SELECT(&CTX->FAN_selector, FAN_TO_CHL(CTX->active_fan));
    FAN_CNT_start(&CTX->FANS[CTX->active_fan]);
    
    
}
static transition_t st_meas_fan_handle(context_t *CTX, event_t ev, state_t current)
{
    switch(ev)
    {
        case MEAS_FAN_DONE:{
            FAN_CNT_stop(&CTX->FANS[CTX->active_fan]);
            CTX->gate_active = 0;
            CTX->has_deadline = 0;
            return to(ST_IDLE);
            }
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
            //LATCbits.LATC5 ^= 1; //debug: works
            if(!CTX->ENS160.initialized)
            {
                CTX->fault_flags |= FAULT_ENS160;
                CTX->comm_i2c_flags = COMM_COMP;
                return stay(current);
            }
            if(!ENS160_read_status(&CTX->ENS160))
            {
                CTX->fault_flags |= FAULT_ENS160;
                CTX->comm_i2c_flags = COMM_COMP;
                return stay(current);
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
            //LATCbits.LATC5 ^= 1; //Debug works
            /*indicate the communication with the device is done
            and return FSM to idle mode, let the app queue decide what comes next*/
            CTX->comm_i2c_flags = NO_COMM;
            CTX->gate_active = 0;
            CTX->gate_owner = GATE_NONE;
            return to(ST_IDLE);
        } 

        default:
            return stay(current);
    }
}





static void st_comm_entry(context_t *CTX)
{
    CTX->gate_owner     = GATE_COMM;
    CTX->gate_active    = 1;



    /*entering this state assumes that there is something in the RX
    buffer, thus the entry should check if the RX buffer has enought 
    data to parse*/
    
    /*should maybe clear the TX payload buffer*/

    switch(CTX->comm_req.type)
    {
        // case COMM_RESP_PING:{
            
        //     break;
        // }
        // case COMM_RESP_STAT:{

        //     break;
        // }

        case COMM_RESP_F1:
        case COMM_RESP_F2:
        {   
            
            fan_sel_t fan_id = (CTX->comm_req.type == COMM_RESP_F1) ? FAN_1 : FAN_2;
            fan_t *fan = &CTX->FANS[fan_id];

            CTX->tx_msg.cmd = (fan_id == FAN_1) ? CMD_GET_F1 : CMD_GET_F2;
            CTX->tx_msg.len = 3;
            CTX->tx_msg.payload[0] = fan->duty_percent;
            CTX->tx_msg.payload[1] = (uint8_t)(fan->RPM >> 8); 
            CTX->tx_msg.payload[2] = (uint8_t)(fan->RPM);
            break;
        }
        case COMM_RESP_SENSOR:{
            CTX->tx_msg.cmd = CMD_GET_SENSOR;
            CTX->tx_msg.len = 7; /* dev addr(8) + aqi(8) + tvoc_ppb(16) + eco2_ppm(16)*/
            CTX->tx_msg.payload[0] = CTX->ENS160.dev_addr;
            CTX->tx_msg.payload[1] = CTX->ENS160.aqi;
            CTX->tx_msg.payload[2] = (uint8_t)(CTX->ENS160.tvoc_ppb>>8);
            CTX->tx_msg.payload[3] = (uint8_t)(CTX->ENS160.tvoc_ppb);
            CTX->tx_msg.payload[4] = (uint8_t)(CTX->ENS160.eco2_ppm>>8);
            CTX->tx_msg.payload[5] = (uint8_t)(CTX->ENS160.eco2_ppm);
            CTX->tx_msg.payload[6] = CTX->ENS160.dev_status;
            break;
        }


        case COMM_RESP_NONE:
        default:
            break;
    }

    /*assemble frame - add SOF, CMD, checksum, END1,END2 and so on*/
    COMM_assemble_frame(&CTX->tx_msg);
    /*start TX*/
    COMM_TX_start(&CTX->tx_msg);
}
static transition_t st_comm_handle(context_t *CTX, event_t ev, state_t current)
{   
    
    switch(ev)
    {
        case COMM_TX_DONE: {
            CTX->comm_req.type      = COMM_RESP_NONE;
            CTX->gate_active        = 0;
            CTX->has_deadline       = 0;
            CTX->gate_owner         = GATE_NONE;
            return to(ST_IDLE);
        }
        default:
            return stay(current);   
    }
}



static const state_ops_t OPS[ST_COUNT] =  
{
    [ST_IDLE]           = {st_idle_entry,           st_idle_handle},
    [ST_MEAS_FAN]       = {st_meas_fan_entry,       st_meas_fan_handle},
    [ST_MEAS_ENS160]    = {st_meas_ens160_entry,    st_meas_ens160_handle},
    [ST_COMM]           = {st_comm_entry,           st_comm_handle},
};

/*states for setting fan speeds should be added here*/


void FSM_transition(FSM_t *sm, state_t next)
{
    if(next==sm->state) return; //if the next state is the same, stay

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
    // sm->state           = ST_INIT;
    sm->state           = ST_IDLE;
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
    state_t old_state = sm->state;
    transition_t tr = OPS[sm->state].handle(&sm->CTX, ev, old_state);
    //FSM_UART_debug_transmission(&sm->CTX, old_state,ev,tr.next);

    if(tr.changed)
    {
        // FSM_UART_debug_transmission(&sm->CTX, old_state,ev,tr.next);
        FSM_transition(sm, tr.next);
    }
}

//remove for final version
// void FSM_UART_debug_transmission(context_t *CTX, state_t old, event_t ev, state_t next)
// {
//     if(!COMM_tx_done()) // this might be risky?
//         return;

//     COMM_clear_tx_done();

//     CTX->tx_msg.cmd = CMD_DBG_ST;
//     CTX->tx_msg.len = 3;
//     CTX->tx_msg.payload[0] = (uint8_t) old; //state
//     CTX->tx_msg.payload[1] = (uint8_t) ev; // event
//     CTX->tx_msg.payload[2] = (uint8_t) next; // state 

//     COMM_assemble_frame(&CTX->tx_msg);
//     COMM_TX_start(&CTX->tx_msg);
// }
