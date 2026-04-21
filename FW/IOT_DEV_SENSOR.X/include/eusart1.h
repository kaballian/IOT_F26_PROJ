#ifndef EUSART1_H
#define EUSART1_H

/*
configuration for EUSART1
*/

#include <xc.h>
#include "utils.h"

#define END1                0xAA /*RX mesg end sequence*/
#define END2                0x55 
#define UART_MAX_PAYLOAD    8
volatile uint8_t g_uart_rx_f;
volatile uint8_t g_uart_rx_msg_r; /*RX message is ready for parsing*/


volatile uint8_t END_REG; /*register for END flags*/


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
    uint8_t msg_ready;
    uint8_t msg_error;
}UART_rx_parser_t;





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

#endif  