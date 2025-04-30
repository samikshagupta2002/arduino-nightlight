#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

// Declaring a function to initialize the LCD in 4-bit mode
void lcd_init(void);

// Declaring a function to send a command to the LCD
void lcd_command(uint8_t cmd);

// Declaring a function to send a single character (data) to the LCD
void lcd_char(uint8_t data);

// Declaring a function to display a null-terminated string on the LCD
void lcd_string(const char* str);

// Declaring a function to clear the LCD screen
void lcd_clear_line(void);

// Declaring a function to display LED and light sensor status on the first line
void lcd_firstLineStatus(uint16_t adc_value, bool led_on);

// Declaring a function to display a message on the second line
void lcd_secondLineMessage(const char* msg);

#endif
