#ifndef EUSART1_H
#define EUSART1_H

/*
configuration for EUSART1
*/

#include <xc.h>
#include "utils.h"
volatile uint8_t g_uart_rx_f;

void EUSART1_init(void);
void EUSART1_ISR(void);
/*byte transmission API*/
bool EUSART1_rx_available(void);
bool EUSART1_read_byte(uint8_t *byte);
bool EUSART1_tx_has_room(void);
bool EUSART1_write_byte(uint8_t byte);
uint8_t EUSART1_write_buf(const uint8_t *data, uint8_t len);

#endif  