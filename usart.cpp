#include <avr/io.h>
#include "usart.h"

// This function is initializing USART with 9600 baud rate
void USART_init(void) {
    uint16_t ubrr = 103; // For 16 MHz clock and 9600 baud

    UBRR0H = (uint8_t)(ubrr >> 8);  // Setting high byte of baud rate
    UBRR0L = (uint8_t)ubrr;         // Setting low byte of baud rate

    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enabling transmitter and receiver
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Setting frame: 8 data bits, 1 stop bit
}

// This function is sending a single character over USART
void USART_send_char(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // Waiting for transmit buffer to be empty
    UDR0 = c;                         // Sending the character
}

// This function is sending a null-terminated string over USART
void USART_send_string(char* str) {
    while (*str) {
        USART_send_char(*str++);     // Sending each character in string
    }
}
