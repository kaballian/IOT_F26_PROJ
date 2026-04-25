#ifndef EUSART1_H
#define EUSART1_H

/*
configuration for EUSART1
*/
/*writeup regarding timing:
baud rate at 115200
1 byte = ~87us

if the ISR + PARSER ~200 cycles at 16MHz, -> 3-12 us
this should be plenty of time to complete a parsing gauntlet

*/

#include <xc.h>
#include "utils.h"

#define END1                0xAA /*RX/TX mesg end sequence*/
#define END2                0x55 
#define UART_MAX_PAYLOAD    8
volatile uint8_t g_uart_rx_f;
volatile uint8_t g_uart_rx_msg_r; /*RX message is ready for parsing*/


volatile uint8_t END_REG; /*register for END flags*/



typedef enum{
    CMD_PING        = 0x01,     // no payload
    CMD_STAT        = 0x02,     // send status flags
    CMD_SET_F1      = 0x03,     // payload [fan_id] [duty]
    CMD_SET_F2      = 0x04,     // payload [fan_id] [duty]
    CMD_GET_F1      = 0x05,     // payload [fan_id]
    CMD_GET_F2      = 0x06,     // payload [fan_id]
    CMD_GET_SENSOR  = 0x07,     // payload [sensor id] 
    UART_CMD_ACK    = 0xF0,
    UART_CMD_NACK   = 0xF1
}UART_comm_t;


typedef struct{
    uint8_t cmd;
    uint8_t len;
    uint8_t payload[8];
    uint8_t frame[16];
    uint8_t frame_len;
}UART_tx_msg_t;

/*
7   6   5   4   3   2   1       0
                        END2    END1
*/


void EUSART1_init(void);
void EUSART1_ISR(void);
/*byte transmission API*/
bool EUSART1_rx_available(void);
bool EUSART1_read_byte(uint8_t *byte);
bool EUSART1_tx_has_room(void);
bool EUSART1_write_byte(uint8_t byte);
uint8_t EUSART1_write_buf(const uint8_t *data, uint8_t len);


void COMM_assemble_frame(UART_tx_msg_t *tx);
void COMM_TX_start(UART_tx_msg_t *tx);

#endif  