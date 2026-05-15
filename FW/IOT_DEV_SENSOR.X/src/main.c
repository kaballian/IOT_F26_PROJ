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




/*configs?*/
// CONFIG1


#pragma config FEXTOSC = OFF    // External Oscillator Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_1MHz// Reset Oscillator Selection bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config VDDAR = HI       // VDD Range Analog Calibration Selection bit (Internal analog systems are calibrated for operation between VDD = 2.3 - 5.5V)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2
#pragma config MCLRE = EXTMCLR  // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RA3 pin function is MCLR)
#pragma config PWRTS = PWRT_OFF // Power-up Timer Selection bits (PWRT is disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit is ignored)
#pragma config DACAUTOEN = OFF  // DAC Buffer Automatic Range Select Enable bit (DAC Buffer reference range is determined by the REFRNG bit)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection bit (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled; ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCKED One-Way Set Enable bit (The PPSLOCKED bit can be cleared and set only once after an unlocking sequence is executed; once PPSLOCKED is set, all future changes to PPS registers are prevented)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT Operating Mode bits (WDT Disabled, SEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT Input Clock Select bits (Software Control)

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash (SAF) Enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block is NOT write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block is NOT write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register is NOT write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is NOT write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash (SAF) Write Protection bit (SAF is NOT write protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR. MCLRE Configuration bit is ignored)

// CONFIG5
#pragma config CP = OFF         // Program Flash Memory Code Protection bit (Program Flash Memory code protection is disabled)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM code protection is disabled)






#include "include/event_queue.h"

// #include "include/parse.h"
#include "include/eusart1.h"
#include "include/system.h"
#include <pic16f18124.h>
#include <xc.h>


















/*the set list*/
typedef enum{
    STEP_F1 = 0,
    STEP_F2,
    STEP_ENS160,
    STEP_COMM,
    STEP_COUNT
}app_step_t;




/*  HARDWARE INSTANTIATIONS         */
/*  statemachine and event queue    */
static FSM_t sm;
static event_q_t ev_q;
static app_step_t step_index;


// volatile uint8_t g_fsm_tick_f   = 0;    //HARDWARE FLAG

volatile uint8_t g_fan_f        = 0;    //SOFTWARE FLAG
volatile uint8_t g_ENS160_f     = 0;    //SOFTWARE FLAG
// volatile uint32_t g_sys_ms      = 0;    //sys tick counter
volatile uint32_t g_fan_deadline   = 0; //fan deadline check
    
// extern volatile uint8_t g_comm_tx_done_f; //ISR flag

/*helper proto*/
static void APP_handleUART(context_t *CTX);
void APP_dispatch_events(void);

/*handling wrapper*/
// static inline void APP_post_event(event_t ev)
// {   
//     /*this essentially says:
//     something happend, please schedule it for handling*/
//     (void)EVENT_Q_push(&ev_q, ev);
// }




void APP_service(context_t *CTX, event_q_t *ev)
{
    // EVENT_Q_push(ev,MEAS_FAN1_START);
    static uint8_t step = 0;
    static uint32_t last_ms = 0;
    static UART_msg_t rx_msg;

    if(UART_parser_GetMsg(&rx_msg))
    {
        switch(rx_msg.cmd)
        {
            case CMD_SET_F1:
            case CMD_SET_F2:
            {
                fan_sel_t fan_id = (rx_msg.cmd == CMD_SET_F1) ? FAN_1 : FAN_2;
                CTX->active_fan = fan_id;
                CTX->FANS[fan_id].duty_percent = rx_msg.payload[1];
                EVENT_Q_push(ev, SET_FAN);
                break;
            }

            case CMD_GET_SENSOR:{
                CTX->comm_req.type = COMM_RESP_SENSOR;
                EVENT_Q_push(ev, UART_RESP);
                break;
            }

            case CMD_GET_F1:
            {
                CTX->comm_req.type = COMM_RESP_F1;
                EVENT_Q_push(ev, UART_RESP);
                break;
            }
            case CMD_GET_F2:
            {
                CTX->comm_req.type = COMM_RESP_F2;
                EVENT_Q_push(ev, UART_RESP);
                break;
            }
        }
    }

    if(CTX->gate_active && CTX->has_deadline)
    {
        if((CTX->sys_ms - CTX->gate_deadline) < 0x80000000UL)
        {
            if(CTX->gate_owner == GATE_FAN)
            {
                EVENT_Q_push(ev, MEAS_FAN_DONE);
            }

            CTX->gate_active = 0;
            CTX->has_deadline = 0;
        }
        return;
    }

    if(CTX->gate_owner == GATE_ENS160)
    {
        if(CTX->comm_i2c_flags == COMM_INIT)
        {
            CTX->comm_i2c_flags = READ_DATA;
            EVENT_Q_push(ev, MEAS_ENS160_READ);
        }
        else if(CTX->comm_i2c_flags == COMM_COMP)
        {
            EVENT_Q_push(ev, MEAS_ENS160_DONE);
        }
    }


    if((CTX->sys_ms - last_ms) < 2000)
    {
        return;
    }

    last_ms = CTX->sys_ms;

    switch(step)
    {
        case 0:{
            CTX->active_fan = FAN_1;
            EVENT_Q_push(ev, MEAS_FAN_START);
            step = 1;
            break;
        }

        case 1:{
            CTX->active_fan = FAN_2;
            EVENT_Q_push(ev, MEAS_FAN_START);
            step = 2;
            break;
        }

        case 2:{
            EVENT_Q_push(ev, MEAS_ENS160_START);
            step = 0;
            break;
        }

        default:{
            break;
        }
    }
}



// int main(void) {
    
//     /*set clocks and pins*/
//     SYSTEM_init(); 
//     APP_init();
   
//     INTCONbits.GIE = 1;
    
    
//     // if(sm.CTX.init_flags == (INIT_PWM1 | INIT_PWM2 | INIT_ENS160))
//     // {
//     //     FSM_dispatch(&sm, INIT_COMP);
//     // }

//     while(1)
//     {
//         APP_service();
//         APP_dispatch_events();
//     }

//     return 1;

// }
// static UART_tx_msg_t tx_msg; //used in test 3 and 4
// static UART_msg_t rx_msg;
// static bool fan_gate_active = false;
// static uint32_t fan_gate_deadline = 0;
// static uint16_t fan_gate_ms = 500u;
    
/*protos for testing fan measurement*/
// static inline bool time_reached(uint32_t now, uint32_t deadline)
// {
//     return (int32_t)(now - deadline) >= 0;
// }
// void FAN_measure_start(fan_t *fan)
// {
//     fan_gate_active = true;
//     fan_gate_deadline = g_sys_ms + fan_gate_ms;

//     FAN_CNT_start(fan);
// }

/*TX message assembler for F1*/
// void COMM_build_resp_f1(UART_tx_msg_t *tx, uint8_t duty, uint16_t rpm)
// {
//     tx->cmd = CMD_GET_F1;
//     tx->len = 3;
//     tx->payload[0] = duty; 
//     tx->payload[1] = (uint8_t)(rpm & 0xFF);
//     tx->payload[2] = (uint8_t)((rpm >> 8) & 0xFF);
// }

// void COMM_build_resp_sensor(UART_tx_msg_t *tx, ENS160_t *dev)
// {
//     tx->cmd = CMD_GET_SENSOR;
//     tx->len = 6;
//     tx->payload[0] = dev->aqi; 
//     tx->payload[1] = (uint8_t)(dev->tvoc_ppb & 0xFF);
//     tx->payload[2] = (uint8_t)((dev->tvoc_ppb >> 8) & 0xFF);
//     tx->payload[3] = (uint8_t)(dev->eco2_ppm & 0xFF);
//     tx->payload[4] = (uint8_t)((dev->eco2_ppm >> 8) & 0xFF);
//     tx->payload[5] = dev->dev_status;
// }

int main(void)
{   
    CLOCK_init(); //works
    PIN_MANAGER_init(); //works
    ISR_init(); // works
    EUSART1_init(); //works
    TMR0_init(); //works
    TMR1_CNT_init(); // works
    PWM_init(); 
    I2C2_init(); // works

    //WORKS
    // while(1)
    // {
    //     while(!PIR4bits.TX1IF)
    //     {
    //         ;
    //     }
    //     TX1REG = 0x55;
    //     __delay_ms(100);
    // }
    
    //WORKS
    // g_comm_tx_done_f = 1;

    // while(1)
    // {
    //     if(g_comm_tx_done_f)
    //     {
    //         g_comm_tx_done_f = 0;

    //         tx_msg.cmd = CMD_PING;
    //         tx_msg.len = 0;

    //         COMM_assemble_frame(&tx_msg);
    //         COMM_TX_start(&tx_msg);

    //         __delay_ms(1000);
    //     }
    // }

    // WORKS
    // while(1)
    // {
    //     if(UART_parser_GetMsg(&rx_msg))
    //     {
            
    //         tx_msg.cmd = CMD_PING;
    //         tx_msg.len = 0;

    //         COMM_assemble_frame(&tx_msg);
    //         COMM_TX_start(&tx_msg);
    //     }
    // }

    // uint32_t last_ms = 0;
    // uint8_t dc = 1;
    //WORKS  -- 3
    // fan_t fan1;
    // fan_t fan2;

    // FAN_init(&fan1, &PWM_FAN1_CH, 20);
    // FAN_init(&fan2, &PWM_FAN2_CH, 30);

    // __delay_ms(1000);


    //test I2C ENS16t0  --4 WORKS
    // ENS160_t sensor;
    // ENS160_init(&sensor,ENS_160_ADDR0);
    // uint16_t part_id = 0;
    // __delay_ms(1000);
    // ENS160_probe(ENS_160_ADDR0, &part_id);
    // __delay_ms(1000);
    // ENS160_set_opmode(&sensor, ENS160_OPMODE_STANDARD);
    // __delay_ms(1000);


    // test -- 5 FSM with UART
    // FSM_init(&sm);
    // __delay_ms(2000);
    
    // FSM_dispatch(&sm, INIT_COMP);
    // __delay_ms(2000);

    // test -- 6 app_service without TMR
    FSM_init(&sm);    
    EVENT_Q_init(&ev_q);
    FSM_dispatch(&sm, INIT_COMP);
    // __delay_ms(1000);
    



    while(1)
    {
        //works
        // if(UART_parser_GetMsg(&rx_msg))
        // {
        //     tx_msg.cmd = rx_msg.cmd;
        //     tx_msg.len = rx_msg.len;

        //     for(uint8_t i = 0; i < rx_msg.len; i++)
        //     {
        //         tx_msg.payload[i] = rx_msg.payload[i];
                
        //     }
        //     COMM_assemble_frame(&tx_msg);
        //     COMM_TX_start(&tx_msg);
        // }

        // if(UART_parser_GetMsg(&rx_msg))
        // {
        //     switch(rx_msg.cmd)
        //     {
        //         case CMD_PING:
        //             tx_msg.cmd = CMD_PING;
        //             tx_msg.len = 0;
        //             break;
                
        //         case CMD_GET_F1:
        //             if(rx_msg.len == 1)
        //             {
        //                 tx_msg.cmd = UART_CMD_ACK;
        //             }else{
        //                 tx_msg.cmd = UART_CMD_NACK;
        //             }
        //             tx_msg.cmd = UART_CMD_NACK;
        //             break;

        //         case CMD_GET_F2:
        //             tx_msg.cmd = CMD_GET_F2;
        //             tx_msg.len =  0;
        //             break;

        //         default:
        //             tx_msg.cmd = UART_CMD_NACK;
        //             tx_msg.len =  0;
        //             break;
        //     }
        //     COMM_assemble_frame(&tx_msg);
        //     COMM_TX_start(&tx_msg);
        // }


        //TMR0 test
        //WORKS
        // if((g_sys_ms - last_ms) >= 1000)  //check every second
        // {
        //     last_ms = g_sys_ms;
        //     LATCbits.LATC5 ^= 1; //toggle RC5
        // }


        
        // PWM / FAN test WORKS
        // FAN_set_duty(&fan1, 10);
        // FAN_set_duty(&fan2, 10);
        // __delay_ms(2000);

        // FAN_set_duty(&fan1, 25);
        // FAN_set_duty(&fan2, 25);
        // __delay_ms(2000);


        // FAN_set_duty(&fan1, 50);
        // FAN_set_duty(&fan2, 50);
        // __delay_ms(2000);


        // FAN_set_duty(&fan1, 75);
        // FAN_set_duty(&fan2, 75);
        // __delay_ms(2000);

        // FAN_set_duty(&fan1, 100);
        // FAN_set_duty(&fan2, 100);
        // __delay_ms(2000);

        // FAN_set_duty(&fan1, 0);
        // FAN_set_duty(&fan2, 0);
        // __delay_ms(2000);

        // for(uint8_t i = 0; i <= 100; i++)
        // {
        //     FAN_set_duty(&fan1, i);
        //     __delay_ms(200);
        // }


        // fan counter test -- 3
        // WORKS
        // FAN_CNT_start(&fan1);
        
        // wait_ms_sys(fan_gate_ms);

        // FAN_CNT_stop(&fan1);
        // tx_msg.cmd = CMD_GET_F1;
        // COMM_build_resp_f1(&tx_msg, fan1.duty_percent, fan1.RPM);
        // COMM_assemble_frame(&tx_msg);
        // COMM_TX_start(&tx_msg);

        // while(!g_comm_tx_done_f)
        // {

        // }
        // g_comm_tx_done_f = 0;

        // __delay_ms(1000);





        //i2c ENS160 sensor test -- 4
        //works part 1
        // ENS160_read_status(&sensor);
        // __delay_ms(1000);  
        // ENS160_read_data(&sensor);
        // __delay_ms(1000);
        

        // part - send with UART -- WORKS -- 4

        // ENS160_read_status(&sensor);
        // __delay_ms(1000);
        // ENS160_read_data(&sensor);
        // __delay_ms(1000);

        // COMM_build_resp_sensor(&tx_msg, &sensor);
        // COMM_assemble_frame(&tx_msg);
        // COMM_TX_start(&tx_msg);

        // __delay_ms(10000);




        //part 5 - FSM with UART -- works
        /* init function runs in main once, outside of while
        in while, run through every state, at every event transition 
        print a uart message about current state and next state
        PUT UART IN THE DISPATCHER
        */
    
        // FSM_dispatch(&sm, MEAS_FAN1_START);
        // __delay_ms(2000);

        // FSM_dispatch(&sm, MEAS_FAN1_DONE);
        // __delay_ms(2000);

        // FSM_dispatch(&sm, MEAS_FAN2_START);
        // __delay_ms(2000);

        // FSM_dispatch(&sm, MEAS_FAN2_DONE);
        // __delay_ms(2000);
        
        //UART SANITY CHECK
        // if(COMM_tx_done())
        // {
        //     COMM_clear_tx_done();

        //     sm.CTX.tx_msg.cmd = CMD_DBG_ST;
        //     sm.CTX.tx_msg.len = 3;
        //     sm.CTX.tx_msg.payload[0] = 0xAA;
        //     sm.CTX.tx_msg.payload[1] = 0xBB;
        //     sm.CTX.tx_msg.payload[2] = 0xCC;
        //     COMM_assemble_frame(&sm.CTX.tx_msg);
        //     COMM_TX_start(&sm.CTX.tx_msg);

        // }


        //PART 6 - fsm with app_service -- no TMR
        
        //sanity check, transitions are too fast for UART
        // EVENT_Q_push(&ev_q, MEAS_FAN1_START);
        // event_t ev;
        // while(EVENT_Q_pop(&ev_q, &ev))
        // {
        //     FSM_dispatch(&sm, ev);
        // }
        // __delay_ms(500);

        // EVENT_Q_push(&ev_q, MEAS_FAN1_DONE);
        // while(EVENT_Q_pop(&ev_q, &ev))
        // {
        //     FSM_dispatch(&sm, ev);
        // }
        
        // __delay_ms(1000);
       
        if(g_tmr0_1ms_flag)
        {
            g_tmr0_1ms_flag = 0;
            sm.CTX.sys_ms = g_sys_ms;
            APP_service(&sm.CTX, &ev_q);
        }
        
        event_t ev;
        while(EVENT_Q_pop(&ev_q, &ev))
        {
            FSM_dispatch(&sm, ev);
        }


    



    }   



    

    //WORKS
    // ANSELCbits.ANSELC5 = 0;
    // ODCONCbits.ODCC5 = 0;
    // TRISCbits.TRISC5 = 0;
    // LATCbits.LATC5 = 1;

    // while(1)
    // {
    //     LATCbits.LATC5 ^= 1;
    //     __delay_ms(1000);
    // }
    
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
    - RX (check)
    - TX (check)


TESTING:
    FAN counter works (TMR1)
    TMR0 works
    UART works (TX & RX)
    I2C works 

context_t scheduling step - next_step, decide what comes after a given measurement
right now, the measurement sequence is buried inside the FSM, more specifically
inside the fn_handle when a measurement is complete, the only way to set
a new gate_owner is in a state_entry, which is not ideal.



IMPORTANT!
check app_service g_uart_rx_f condition




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