#include "../include/system.h"



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
    
}

/*flag helpers*/


static void st_init_entry(context_t *CTX){}
static transition_t st_init_handle(context_t *CTX, event_t ev){}
static void st_idle_entry(context_t *CTX){}
static transition_t st_idle_handle(context_t *CTX, event_t ev){}
static void st_meas_entry(context_t *CTX){}
static transition_t st_meas_handle(context_t *CTX, event_t ev){}
static void st_comm_entry(context_t *CTX){}
static transition_t st_comm_handle(context_t *CTX, event_t ev){}








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

}

void FSM_dispatch(FSM_t *sm, event_t ev)    
{
    const state_ops_t *s = &OPS[sm->state];
    transition_t tr = stay();

    if(s->handle)
    {
        tr = s->handle(&sm->CTX, ev);
    }
    if(tr.changed)
    {
        FSM_transition(sm, tr.next);
    }
}