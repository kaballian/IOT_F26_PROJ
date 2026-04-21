#include "include/parse.h"
#include "include/eusart1.h"


void UART_RX_ParserReset(UART_rx_parser_t *p)
{
    /*resets entire PARSER mini statemachine*/
    p->state    = RX_WAIT_SOF; /*initial state of parser*/
    p->cmd      = 0;
    p->len      = 0;
    p->idx      = 0;
    p->CHKsum   = 0;
    p->msg_error = 0;   
}




void UART_RX_ParserFeed(UART_rx_parser_t *p, uint8_t byte)
{
    switch(p->state)
    {
        case RX_WAIT_SOF:{
            if(byte == SOF){ //check if the SOF condition is met
                p->state = RX_WAIT_CMD; /*advance STM to next state*/
                p->idx = 0; //reset index counter
                p->CHKsum = 0; //reset checksum 
                p->msg_ready = 0; //reset complete flag
            }   
            break;
        }
        case RX_WAIT_CMD:{
            /*in this state the next byte must be a cmd*/
            p->cmd = byte;   
            p->CHKsum ^= byte;  /*XOR for checksum*/
            p->state = RX_WAIT_LEN; /*advance state*/
            break;
        }
        
        case RX_WAIT_LEN:{
            p->len = byte;
            p->CHKsum ^= byte; /*add to checksum*/
            /*check if received length is bigger than allowed*/
            if(p->len > UART_MAX_PAYLOAD)
            {
                p->msg_error = 1;
            }else if(p->len == 0)
            {
                /*if payload len is 0, then advance to checksum*/
                p->state = RX_WAIT_CHK;
            }else{
                /*ensure that the index is reset and advance to next state*/
                p->idx = 0;
                p->state = RX_WAIT_PAYLOAD;
            }
            break;
        }

        case RX_WAIT_PAYLOAD:{
            /*next data must be payload*/
            p->payload[p->idx++] = byte;
            p->CHKsum ^= byte; /*add to checksum*/

            /*as data is received the index is increasing, when limit is met
            advance the state*/
            if(p->idx >= p->len)
            {
                p->state = RX_WAIT_CHK;
            }
            break;
        }

        case RX_WAIT_CHK:{
            /*check if calculated checksum is equal to received checksum*/
            if(byte == p->CHKsum)
            {
                /*if it is: advance to end sequence 1 of 2*/
                p->state = RX_WAIT_END1;
            }else{
                UART_RX_ParserReset(p);
                p->msg_error = 1;
            }
            break;
        }

        /*next two cases are end sequences, which must match*/
        case RX_WAIT_END1:{
            if(byte == END1)
            {
                p->state = RX_WAIT_END2;
            }else{
                UART_RX_ParserReset(p);
                p->msg_error = 1;
            }
            break;
        }
    
        case RX_WAIT_END2:{
            if(byte == END2)
            {
                p->msg_ready = 1;
            }else{
                p->msg_error = 1;
            }
            p->state = RX_WAIT_SOF;
            break;
        }

        default:
        UART_RX_ParserReset(p);
        break;
    }
}
