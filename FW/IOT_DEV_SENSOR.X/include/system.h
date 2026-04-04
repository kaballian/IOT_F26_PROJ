#ifndef SYSTEM_H
#define SYSTEM_H
#include <xc.h>
#include "utils.h"
#include "include/clock.h"
#include "include/pins.h"



#include "FAN.h"
#include "EXT_SENSOR.h"
#include "include/ENS_160.h"
#include "include/I2C.h"
#include "include/FAN.h"
#include "include/PWM.h"
#include "include/eusart1.h"
#include "include/ADG419BR.h"
#include "include/TMR1.h"
#include "include/interrupt.h"







/*
IOT PROJECT:
AIR PARTICLE SENSOR FIRMWARE
SYSTEM.h
*/
/*simple statemachine*/

void SYSTEM_init(void);


//events
typedef enum{
    None,
    TMRTick,
    UARTTIMEOUT,
    UART,
    MEAS_FAN1_START,
    MEAS_FAN2_START,
    MEAS_FAN1_DONE,
    MEAS_FAN2_DONE,
    MEAS_ENS160,
    MEAS_BME280,
    I2C_ERR,
    INIT_COMP,
    TMR_START,
    CONV_START,
    CONV_DONE,
    PWM_SET
}event_t;
//states
typedef enum{
    ST_INIT = 0,
    ST_IDLE,
    ST_MEAS,
    ST_COMM,
    ST_COUNT
}state_t;

typedef uint8_t flag8_t;

//init flags
typedef enum{
    INIT_INCOMPLETE = 0,
    INIT_PWM1       =(1u<<0),
    INIT_PWM2       =(1u<<1),
    INIT_ENS160     =(1u<<2),
    INIT_BME280     =(1u<<3),
    INIT_STAT_LED   =(1u<<4),
    INIT_UART       =(1u<<5),
}init_f_t;

//faults flags
typedef enum {
    FAULT_NONE      = 0,
    FAULT_FREQ_F1   =(1u<<0),
    FAULT_FREQ_F2   =(1u<<1),
    FAULT_ENS160    =(1u<<2),
    FAULT_BME280    =(1u<<3),
    FAULT_UART      =(1u<<4)
}fault_f_t;
//pwr flags


typedef struct {
    uint16_t humidity;
    uint16_t pressure;
}BME280_meas;

typedef struct{
    uint16_t AQI; //air quality indicator 
}ENS160_meas;



//shared data
typedef struct {
    uint32_t    ms;
    uint32_t    fsm_tick;
    uint16_t    fan_gate_div;

    flag8_t     init_flags;
    flag8_t     fault_flags;
    uint16_t    F1_meas[FAN_BUF_LEN];
    uint16_t    F2_meas[FAN_BUF_LEN];
    BME280_meas room_meas[BME280_BUF_LEN];
    ENS160_meas AQI_meas[ENS160_BUF_LEN];
    
    uint8_t     meas_head;
    uint8_t     meas_count;



    //hardware
    ENS160_t        ENS160;
    fan_t           FAN1;
    fan_t           FAN2;
    // switch_chl_t    FAN_selector;
    ADG419_t        FAN_selector;    
}context_t;

//state transition
typedef struct {
    bool changed;
    state_t next;
}transition_t;

static inline transition_t stay(state_t current){
    // transition_t t = {
    //     .changed    = false,
    //     .next       = 0
    // };
    transition_t t;
    t.changed = false;
    t.next = current;
    return t;
}
static inline transition_t to(state_t s)
{
    transition_t t = {
        .changed    = true,
        .next       = s
    };
    return t;

}

typedef void        (*state_entry_fn)(context_t *CTX);
typedef void        (*state_exit_fn)(context_t *CTX);
typedef transition_t(*state_handle_fn)(context_t *CTX, event_t e, state_t current);

typedef struct {
    state_entry_fn  entry;
    state_exit_fn   exit;
    state_handle_fn handle;
}state_ops_t;
//state machine
typedef struct{
    context_t   CTX;
    state_t     state;
}FSM_t;


void FSM_init(FSM_t *sm);
void FSM_transition(FSM_t *sm, state_t next);
void FSM_dispatch(FSM_t *sm, event_t ev);

#endif
