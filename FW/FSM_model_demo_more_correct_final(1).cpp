#include <iostream>
#include <cstdint>
#include <cstdlib>

typedef enum{
    none,
    INIT_COMP,
    MEAS_FAN1,
    MEAS_FAN2,
    MEAS_COMP,
    SEND_DATA,
    SEND_COMP,
    UART
}event_t;

typedef enum{
    ST_INIT = 0,
    ST_MEAS, 
    ST_COMM,
    ST_COUNT
}state_t;

typedef struct 
{
    bool changed;
    state_t next;
}transition_t;

typedef struct{
    uint16_t data;
    uint8_t rpm1;
    uint8_t rpm2;
    uint8_t flags;
}context_t;

static inline transition_t stay(state_t current)
{
    transition_t t;
    t.changed = false;
    t.next = current;
    return t;
}
static inline transition_t to(state_t next)
{
    transition_t t = 
    {
        .changed = true,
        .next = next
    };

    return t;
}

typedef void (*state_entry_fn)(context_t *CTX);
typedef void (*state_exit_fn)(context_t *CTX);
typedef transition_t (*state_handle_fn)(context_t *CTX, event_t ev, state_t current);

typedef struct{
    state_entry_fn entry;
    state_exit_fn exit;
    state_handle_fn handle;
}state_ops_t;

typedef struct{
    state_t state;
    context_t CTX;
}fsm_t;

static void st_init_entry(context_t *CTX)
{
    CTX->flags = 0;
    CTX->data = 0;
    CTX->rpm1 = 0;
    CTX->rpm2 = 0;

    std::cout << "st init entry" << std::endl;
}
static transition_t st_init_handle(context_t *CTX, event_t ev, state_t current)
{
    (void)CTX;
    switch(ev)
    {
        case INIT_COMP:
            std::cout << "INIT_COMP EV" << std::endl;
            return to(ST_MEAS);
        default:
            return stay(current);
    }
}
static void st_meas_entry(context_t *CTX)
{
    std::cout << "meas entry, init and stuff" << std::endl;
    (void)CTX;
}
static transition_t st_meas_handle(context_t *CTX, event_t ev, state_t current)
{
    (void)CTX;
    switch(ev)
    {
        case MEAS_FAN1:
            std::cout << "measuring fan 1" << std::endl;
            CTX->rpm1 += 20;
            std::cout << "fan1 rpm: " <<(int)(CTX->rpm1) << std::endl;
            return stay(current);

        case MEAS_FAN2:
            std::cout << "measuring fan 2" << std::endl;
            CTX->rpm2 += 10;
            std::cout << "fan2 rpm: " << (int)(CTX->rpm2) << std::endl;
            return stay(current);
        
        case MEAS_COMP:
            std::cout << "measurement complete, advancing to next state" << std::endl;
            return to(ST_COMM);

        default:
            return stay(current);
    }
}

static void st_comm_entry(context_t *CTX)
{
    std::cout << "communication entry:" << std::endl;
    (void)CTX;
}

static transition_t st_comm_handle(context_t *CTX, event_t ev, state_t current)
{
    switch(ev)
    {
        case SEND_DATA:
            std::cout << "sending data: \n";
            std::cout << "fan1: " <<(int) (CTX->rpm1) << "\n";
            std::cout << "fan2: " <<(int) (CTX->rpm2) << "\n";
            std::cout << "flags: " <<(int) (CTX->flags)  << std::endl;
            return stay(current);

        case SEND_COMP:
            std::cout << "data transfer complete" << std::endl;
            return to(ST_MEAS);

        default:
            return stay(current);
    }
}

static const state_ops_t OPS[ST_COUNT] =  {
    [ST_INIT] = {st_init_entry, 0, st_init_handle},
    [ST_MEAS] = {st_meas_entry, 0, st_meas_handle},
    [ST_COMM] = {st_comm_entry, 0, st_comm_handle}
};

void FSM_init(fsm_t *sm)
{
    sm->state = ST_INIT;

    if(OPS[sm->state].entry != 0)
    {
        OPS[sm->state].entry(&sm->CTX);
    }
}
void FSM_transition(fsm_t *sm, state_t next)
{
    if(next==sm->state) return;

    if(sm->state < ST_COUNT && OPS[sm->state].exit)
    {
        OPS[sm->state].exit(&sm->CTX);
    }

    sm->state = next;

    if(sm->state < ST_COUNT && OPS[sm->state].entry)
    {
        OPS[sm->state].entry(&sm->CTX);
    }
}


void FSM_dispatch(fsm_t *sm, event_t ev)
{
    
    transition_t tr;

    if(sm->state >= ST_COUNT)
    {
        return;
    }
    if(OPS[sm->state].handle == 0)
    {
        return;
    }

    tr = OPS[sm->state].handle(&sm->CTX, ev, sm->state);

    if(tr.changed)
    {
        FSM_transition(sm, tr.next);
    }

}


int main(void)
{

    fsm_t stm;

    FSM_init(&stm);

    FSM_dispatch(&stm, INIT_COMP);

    FSM_dispatch(&stm, MEAS_FAN1);
    FSM_dispatch(&stm, MEAS_FAN2);

    FSM_dispatch(&stm, MEAS_COMP);

    FSM_dispatch(&stm, SEND_DATA);
    FSM_dispatch(&stm, SEND_COMP);

    
    return 1;
}