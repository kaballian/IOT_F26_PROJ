#ifndef PARSE_H
#define PARSE_H

#include "xc.h"
#include "eusart1.h"



typedef enum{
    CMD_PING        = 0x01,   // on payload
    CMD_STAT        = 0x02,       // send status flags
    CMD_SET_F1      = 0x03,     // payload [fan_id] [duty]
    CMD_SET_F2      = 0x04,     // payload [fan_id] [duty]
    CMD_GET_F1      = 0x05,     // payload [fan_id]
    CMD_GET_F2      = 0x06,     // payload [fan_id]
    CMD_GET_SENSOR  = 0x07  // payload [sensor id] 
}UART_comm_t;


#define SOF     0x5A    /*start of frame*/

/*
PROTOCOL:
[SOF] [TYPE] [LEN] [PAYLOAD] [CRC8] [END1] [END2]
BYTE# | descp 


CHECKSUM strategy:
CHK  = CMD ^ LEN ^ payload[0] ^ payload [1] ^...

*/



void UART_RX_ParserFeed(UART_rx_parser_t *p, uint8_t byte);
void UART_RX_ParserReset(UART_rx_parser_t *p);
#endif