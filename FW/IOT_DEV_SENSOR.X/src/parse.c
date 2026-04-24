#include "include/parse.h"
#include "include/eusart1.h"


typedef enum{
    RX_WAIT_SOF = 0,
    RX_WAIT_CMD,
    RX_WAIT_LEN,
    RX_WAIT_PAYLOAD,
    RX_WAIT_CHK,
    RX_WAIT_END1,
    RX_WAIT_END2
}rx_state_t;

typedef struct{
    rx_state_t state;
    uint8_t cmd;
    uint8_t len;
    uint8_t idx;
    uint8_t CHKsum;
    uint8_t payload[UART_MAX_PAYLOAD];

   
}UART_rx_parser_t;





static UART_rx_parser_t rx_parser;
static volatile uint8_t msg_ready = 0;
static UART_msg_t rx_msg;

void UART_RX_ParserReset()
{
    /*resets entire PARSER mini statemachine*/
    rx_parser.state    = RX_WAIT_SOF; /*initial state of parser*/
    rx_parser.cmd      = 0;
    rx_parser.len      = 0;
    rx_parser.idx      = 0;
    rx_parser.CHKsum   = 0;
     
    
}




void UART_RX_ParserFeed(uint8_t byte)
{
    switch(rx_parser.state)
    {
        case RX_WAIT_SOF:{
            if(byte == SOF){ //check if the SOF condition is met
                rx_parser.state = RX_WAIT_CMD; /*advance STM to next state*/
                rx_parser.idx = 0; //reset index counter
                rx_parser.CHKsum = 0; //reset checksum 
            }   
            break;
        }
        case RX_WAIT_CMD:{
            /*in this state the next byte must be a cmd*/
            rx_parser.cmd = byte;   
            rx_parser.CHKsum ^= byte;  /*XOR for checksum*/
            rx_parser.state = RX_WAIT_LEN; /*advance state*/
            break;
        }
        
        case RX_WAIT_LEN:{
            rx_parser.len = byte;
            rx_parser.CHKsum ^= byte; /*add to checksum*/
            /*check if received length is bigger than allowed*/
            if(rx_parser.len > UART_MAX_PAYLOAD)
            {
                UART_RX_ParserReset();
            }else if(rx_parser.len == 0)
            {
                /*if payload len is 0, then advance to checksum*/
                rx_parser.state = RX_WAIT_CHK;
            }else{
                /*ensure that the index is reset and advance to next state*/
                rx_parser.idx = 0;
                rx_parser.state = RX_WAIT_PAYLOAD;
            }
            break;
        }

        case RX_WAIT_PAYLOAD:{
            /*next data must be payload*/
            rx_parser.payload[rx_parser.idx++] = byte;
            rx_parser.CHKsum ^= byte; /*add to checksum*/
            
            /*as data is received the index is increasing, when limit is met
            advance the state*/
            if(rx_parser.idx >= rx_parser.len)
            {
                rx_parser.state = RX_WAIT_CHK;
            }
            break;
        }

        case RX_WAIT_CHK:{
            /*check if calculated checksum is equal to received checksum*/
            if(byte == rx_parser.CHKsum)
            {
                /*if it is: advance to end sequence 1 of 2*/
                rx_parser.state = RX_WAIT_END1;
            }else{
                UART_RX_ParserReset();
            }
            break;
        }

        /*next two cases are end sequences, which must match*/
        case RX_WAIT_END1:{
            if(byte == END1)
            {
                rx_parser.state = RX_WAIT_END2;
            }else{
                UART_RX_ParserReset();
            }
            break;
        }
    
        case RX_WAIT_END2:{
            if(byte == END2)
            {
                rx_msg.cmd = rx_parser.cmd;
                rx_msg.len = rx_parser.len;

                for(uint8_t i = 0; i < rx_parser.len; i++)
                {
                    rx_msg.payload[i] = rx_parser.payload[i];
                }

                msg_ready = 1; /*global, signal data is ready*/
            }
            
            UART_RX_ParserReset(); /*sets state to SOF*/
            break;
        }

        default:
            UART_RX_ParserReset();
            break;
    }
}

/*this is API - check this to see if stuff is ready*/
bool UART_parser_MsgAvailable(void)
{
    return msg_ready != 0;
}
/*this is API - get the data through this call*/
bool UART_parser_GetMsg(UART_msg_t *msg)
{
    if(!msg_ready)
    {
        return false;
    }
    *msg = rx_msg;
    msg_ready = 0;
    return true;
}
uint8_t UART_CHKSUM(uint8_t cmd, uint8_t len, const uint8_t *payload)
{
    uint8_t chk = 0;
    chk ^= cmd;
    chk ^= len;

    for(uint8_t i = 0; i < len; i++)
    {
        chk ^= payload[i];
    }
    return chk;
}
