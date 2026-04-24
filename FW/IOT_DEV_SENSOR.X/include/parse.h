#ifndef PARSE_H
#define PARSE_H

#include "xc.h"
#include "eusart1.h"






typedef struct {
    uint8_t cmd;
    uint8_t len;
    uint8_t payload[UART_MAX_PAYLOAD];
}UART_msg_t;



// typedef enum{
//     CMD_PING        = 0x01,   // on payload
//     CMD_STAT        = 0x02,       // send status flags
//     CMD_SET_F1      = 0x03,     // payload [fan_id] [duty]
//     CMD_SET_F2      = 0x04,     // payload [fan_id] [duty]
//     CMD_GET_F1      = 0x05,     // payload [fan_id]
//     CMD_GET_F2      = 0x06,     // payload [fan_id]
//     CMD_GET_SENSOR  = 0x07  // payload [sensor id] 
// }UART_comm_t;


#define SOF     0x5A    /*start of frame*/

/*
PROTOCOL:
[SOF] [TYPE] [LEN] [PAYLOAD] [CRC8] [END1] [END2]
BYTE# | descp 


CHECKSUM strategy:
CHK  = CMD ^ LEN ^ payload[0] ^ payload [1] ^...

*/



void UART_RX_ParserFeed(uint8_t byte);
void UART_RX_ParserReset();
bool UART_parser_MsgAvailable(void);
bool UART_parser_GetMsg(UART_msg_t *msg);
uint8_t UART_CHKSUM(uint8_t cmd, uint8_t len, const uint8_t *payload);
#endif