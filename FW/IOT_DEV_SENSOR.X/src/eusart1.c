#include "include/eusart1.h"
#include "include/parse.h"


#define UART_RX_BUF_SIZE    64u
#define UART_TX_BUF_SIZE    64u


/*parser states*/




extern volatile uint8_t g_uart_rx_f; //set in the ISR and consumed in the APP layer
extern volatile uint8_t g_uart_rx_msg_r;
extern volatile uint8_t END_REG = 0;

static volatile uint8_t rx_buf[UART_RX_BUF_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint8_t tx_buf[UART_TX_BUF_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;



static uint8_t next_index(uint8_t index, uint8_t size)
{
    /*helper for ring buffer*/
    index++; //advances the index (rx or tx)
    if(index >= size)
    {
        index = 0;
    }
    return index;
}

void EUSART1_init(void)
{
    TX1REG      = 0x0;
    BAUD1CON    = 0x40;
    RC1STA      = 0x0;
    TX1STA      = 0x22;
    SP1BRGL     = 0x0;
    SP1BRGH     = 0x0;

    //baud generator 16 bit high speed
    BAUD1CONbits.BRG16 = 1;
    TX1STAbits.BRGH = 1;

    //async mode
    TX1STAbits.SYNC = 0;

    //enable serial port
    RC1STAbits.SPED = 1;

    //enable TX and RX
    TX1STAbits.TXEN = 1;
    RC1STAbits.CREN = 1;

    //8 bits mode
    TX1STAbits.TX9 = 0;
    RC1STAbits.RX9 = 0;
    //9600 baud @ 16 MHZ
    SP1BRGH = 0x01;
    SP1BRGL = 0xA0; //~416

    rx_head = 0;
    rx_tail = 0;
    tx_head = 0;
    tx_tail = 0;


    UART_RX_ParserReset();
}



void EUSART1_ISR(void)
{
    

    /*RX error recovery*/
    if(RC1STAbits.OERR) //overrun error, cleared by clearing SPEN or CREN
    {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }

    /*RX*/
    if(PIR4bits.RC1IF)
    {   
        /*read data from receive buffer
        this also clears RC1IF flag automatically sec 12.10.13*/
        uint8_t byte = RC1REG; 
       

        UART_RX_ParserFeed(byte);
    }

    /*TX*/
    /*sec 12.10.13*/ 
    if(PIR4bits.TX1IF)
    {

    }

}

bool EUSART1_rx_available(void)
{   /*if the head is not at the same place at the fail, there is free space*/
    return (rx_head != rx_tail);
}

bool EUSART1_read_byte(uint8_t *byte)
{
    if(byte == 0)
    {
        return false;
    }

    if(rx_head == rx_tail)
    {   //check for room
        return false;
    }

    byte = rx_buf[rx_tail];
    rx_tail = next_index(rx_tail, UART_RX_BUF_SIZE);
    return true;

}

bool EUSART1_tx_has_room(void)
{
    uint8_t next = next_index(tx_head, UART_TX_BUF_SIZE);
    return (next != tx_tail);
}

bool EUSART1_write_byte(uint8_t byte)
{
    uint8_t next = next_index(tx_head, UART_TX_BUF_SIZE);

    if(next == tx_tail)
    {
        return false; //buffer is full
    }
    tx_buf[tx_head] = byte;
    tx_head = next;

    /*activate TX interrupt*/
    PIE4bits.TX1IE = 1;

    return true;
}
uint8_t EUSART1_write_buf(const uint8_t *data, uint8_t len)
{
    uint8_t i;
    uint8_t written = 0;

    if(data == 0)
    {
        return 0;
    }

    for(i = 0; i < len; i++)
    {
        if(!EUSART1_write_byte(data[i]))
        {
            break;
        }
        written++;
    }
    return written;

}

void COMM_assemble_frame(UART_tx_msg_t *tx)
{
    uint8_t i = 0;
    tx->frame[i++] = SOF; /*start of frame*/
    tx->frame[i++] = tx->cmd;
    tx->frame[i++] = tx->len;

    for(uint8_t j = 0; j < tx->len; j++)
    {
        tx->frame[i++] = tx->payload[j];
    }

    tx->frame[i++] = UART_CHKSUM(tx->cmd, tx->len, tx->payload);
    tx->frame[i++] = END1;
    tx->frame[i++] = END2;
    tx->frame_len = i;
}
void COMM_TX_start(UART_tx_msg_t *tx);