#ifndef MCP23008_H
#define MCP23008_H

#define IODIR   00h
#define IPOL    01h
#define GPINTEN 02h
#define DELVAL  03h
#define INTCON  04h
#define IOCON   05h
#define GPPU    06h
#define INTF    07h
#define INTCAP  08h // readonly
#define GPIO    09h
#define OLAT    0AH



/*
1.6.1 IODIR 
register controls input output direction 
1 = inputs, 0 = outputs
*/
#define IODIR_ALL_OUT 0b00000000

/*
1.6.2.IPOL
input polarity 
1 = GPIO register bit will reflect the opposite logic state of the input pin
0 = GPIO register bit will reflect the same logic state of the input pin

unused in the scenario
*/

/*
1.6.3 GPINTEN
interrupt on change
1 = enable GPIO pin for IOC
0 = disable GPIO pin for IOC
*/
#define GPINTEN_DISABLE 0b00000000


#endif