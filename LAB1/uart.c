#include "header/uart.h"
#include "header/gpio.h"
#include "header/utils.h"


void delay(unsigned int clock)
{   //Wait {clock} cycles
    while (clock--)
    {
        asm volatile("nop");
    }
}

void uart_init()
{
    register unsigned int selector;

    // P.104 Since I need UART 1 Transmit/Receive Data -> TXD1/RXD1
    // p.102 I find These two in GPIO 14/15 Fun5

    selector = *GPFSEL1;
    selector &= ~(7u << 12); // clean gpio14
    selector |= 2u << 12;    // set alt5 for gpio14
    selector &= ~(7u << 15); // clean gpio15
    selector |= 2u << 15;    // set alt5 for gpio 15
    *GPFSEL1 = selector;


    // set the required control signal (i.e. Pull-up or Pull-Down )
    *GPPUD = 0;   //  P101 top. 00- = off - disable pull-up/down 
    delay(150u); // provides the required set-up time for the control signal
    *GPPUDCLK0 = (1u << 14) | (1u << 15); // enable pins 14 and 15
    delay(150u); // provides the required set-up time for the control signal
    *GPPUDCLK0 = 0u;        // flush GPIO setup


    /* initialize UART */
    *AUX_ENABLE |=1;      
    //P.9: If set the mini UART is enabled. The UART will
    //immediately start receiving data, especially if the
    //UART1_RX line is low.
    //If clear the mini UART is disabled. That also disables
    //any mini UART register access 

    *AUX_MU_CNTL = 0u;
   //P.17 If this bit is set the mini UART receiver is enabled.
   //If this bit is clear the mini UART receiver is disabled
   //Prevent data exchange in initialization process
    *AUX_MU_IER = 0u;
   //Set AUX_MU_IER_REG to 0. 
   //Disable interrupt because currently you don’t need interrupt.
    *AUX_MU_LCR = 3u;       
   //P.14: 00 : the UART works in 7-bit mode
   //11(3) : the UART works in 8-bit mode
   //Cause 8 bits can use in ASCII, Unicode, Char
    *AUX_MU_MCR = 0u;
   //Don’t need auto flow control.
   //AUX_MU_MCR is for basic serial communication. Don't be too smart
    *AUX_MU_BAUD = 270u;
   //set BAUD rate to 115200(transmit speed)
   //so we need set AUX_MU_BAUD to 270 to meet the goal
    *AUX_MU_IIR = 0xc6;
   // bit 6 bit 7 No FIFO. Sacrifice reliability(buffer) to get low latency    // 0xc6 = 11000110
   // Writing with bit 1 set will clear the receive FIFO
   // Writing with bit 2 set will clear the transmit FIFO
   // Both bits always read as 1 as the FIFOs are always enabled  
    /* map UART1 to GPIO pins */
    *AUX_MU_CNTL = 3; // enable Transmitter,Receiver
}   


/**
 * Send a character
 */
void uart_send_char(unsigned int c) {
    /* wait until we can send */
    // P.15 AUX_MU_LSR register shows the data(line) status
    // AUX_MU_LSR bit 5 => 0x20 = 00100000
    // bit 5 is set if the transmit FIFO can accept at least one byte.  
    // &0x20 can preserve 5th bit, if bit 5 set 1 can get !true = false leave loop
    // else FIFO can not accept at lease one byte then still wait 
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20));
    /* write the character to the buffer */
    //P.11 The AUX_MU_IO_REG register is primary used to write data to and read data from the
    //UART FIFOs.
    //communicate with(send to) the minicom and print to the screen 
    *AUX_MU_IO=c;
}

/**
 * Receive a character
 */
char uart_get_char() {
    char r;
    /* wait until something is in the buffer */
    //bit 0 is set if the receive FIFO holds at least 1 symbol.
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01));
    /* read it and return */
    r=(char)(*AUX_MU_IO);
    /* convert carriage return to newline */
    return r=='\r'?'\n':r;
}

/**
 * Display a string
 */
void uart_send_string(char* s) {
    while(*s) {
        /* convert newline to carriage return + newline */
        if(*s=='\n')
            uart_send_char('\r');
        uart_send_char(*s++);
    }
}

/**
 * Display a binary value in hexadecimal
 */
void uart_binary_to_hex(unsigned int d) {
    unsigned int n;
    int c;
    uart_send_string("0x");
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        uart_send_char(n);
    }
}