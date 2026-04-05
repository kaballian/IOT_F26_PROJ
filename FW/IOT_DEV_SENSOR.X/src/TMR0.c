#include "include/TMR0.h"




// volatile uint32_t    g_sys_ms           = 0u;           
// volatile uint8_t     g_fsm_tick_flag    = 0u;
// volatile uint8_t     g_fan_gate_flag    = 0u;

// static volatile uint16_t fsm_tick_div   = 0u;
// static volatile uint16_t fan_gate_div   = 0u;
volatile uint8_t g_tmr0_1ms_flag = 0;  //GLOBAL INTERRUPT FLAG FOR TMR0
volatile uint32_t g_sys_ms = 0;
volatile uint16_t g_fan_flag = 0;



void TMR0_init(void)
{
    T0CON0bits.EN       = 0; // diable during setup
    T0CON0bits.MD16     = 0; // 8 bit mode
    T0CON0bits.OUTPS    = 0b0000; //1:1 postscaler

    T0CON1bits.CS       = 0b010; //clock source -> Fosc/4 = 4MHz
    T0CON1bits.ASYNC    = 0; //timer synchronized to Fosc/4
    T0CON1bits.CKPS     = 0b0101; //prescaler 1:32

    TMR0H = 124;        //TMR0L continues couting from 0, when this value is reached.
    TMR0L = 0;              

    T0CON0bits.EN       = 1;
}

void TMR0_ISR(void)
{   
    /*clear the interrupt flag*/
    PIR0bits.TMR0IF = 0;
    /*indicate that a millisecond has elapsed*/
    g_tmr0_1ms_flag = 1;
    g_sys_ms++;
    g_fan_f++;

}