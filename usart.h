#pragma once
#include <Arduino.h>

#ifndef USART_H
#define USART_H

// Defining the maximum buffer size for string messages
#define MAX_TEXT 200  // This constant is helping to allocate enough space for USART strings

// Declaring a function to initialize the USART with standard settings (9600 baud)
void USART_init(void);

// Declaring a function to send a single character over serial
void USART_send_char(char c);

// Declaring a function to send a full null-terminated string over serial
void USART_send_string(char* str);

#endif


