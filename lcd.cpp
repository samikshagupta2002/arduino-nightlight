#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"

// Defining control and data pins
#define RS PB4
#define EN PB3
#define LCD_RS_PORT PORTB
#define LCD_EN_PORT PORTB
#define LCD_DATA_PORT PORTD
#define LCD_RS_DDR DDRB
#define LCD_EN_DDR DDRB
#define LCD_DATA_DDR DDRD

// This function is writing 4 bits (a nibble) to the LCD using pins PD2–PD5
static void lcd_nibble_write(uint8_t nibble, uint8_t is_data) {
    // Clearing PD2–PD5 before writing new bits
    LCD_DATA_PORT &= ~(0b00111100);

    // Mapping bits of nibble to PD2–PD5 in reversed order
    if (nibble & (1 << 0)) LCD_DATA_PORT |= (1 << PD5);
    if (nibble & (1 << 1)) LCD_DATA_PORT |= (1 << PD4);
    if (nibble & (1 << 2)) LCD_DATA_PORT |= (1 << PD3);
    if (nibble & (1 << 3)) LCD_DATA_PORT |= (1 << PD2);

    // Setting RS high for data, low for command
    if (is_data)
        LCD_RS_PORT |= (1 << RS);
    else
        LCD_RS_PORT &= ~(1 << RS);

    // Pulsing EN to latch the data into LCD
    LCD_EN_PORT |= (1 << EN);
    _delay_us(1);
    LCD_EN_PORT &= ~(1 << EN);
    _delay_ms(1);
}

// This function is sending a command to the LCD in 4-bit mode
void lcd_command(uint8_t command) {
    lcd_nibble_write(command >> 4, 0); // Sending high nibble
    lcd_nibble_write(command & 0x0F, 0); // Sending low nibble
    if (command < 4) _delay_ms(2); // Waiting longer for clear/home commands
    else _delay_ms(1);
}

// This function is sending a character (data) to the LCD
void lcd_char(uint8_t data) {
    lcd_nibble_write(data >> 4, 1); // Sending high nibble
    lcd_nibble_write(data & 0x0F, 1); // Sending low nibble
    _delay_ms(1); // Short delay after character write
}

// This function is sending a null-terminated string to the LCD
void lcd_string(const char* str) {
    while (*str) lcd_char(*str++); // Sending each character one by one
}

// This function is clearing the entire LCD screen
void lcd_clear_line() {
    lcd_command(0x01); // Clear command
    _delay_ms(2);
}

// This function is initializing the LCD in 4-bit mode with 2 lines
void lcd_init() {
    // Configuring RS, EN, and D4–D7 as output
    LCD_RS_DDR |= (1 << RS);
    LCD_EN_DDR |= (1 << EN);
    LCD_DATA_DDR |= 0b00111100; // PD2–PD5

    _delay_ms(100); // Waiting for LCD to power up

    // Sending reset/init sequence for 4-bit mode
    lcd_nibble_write(0x03, 0); _delay_ms(5);
    lcd_nibble_write(0x03, 0); _delay_ms(5);
    lcd_nibble_write(0x03, 0); _delay_ms(5);
    lcd_nibble_write(0x02, 0); _delay_ms(5);

    // Sending configuration commands
    lcd_command(0x28); // 4-bit, 2 lines, 5x7 dots
    lcd_command(0x0C); // Display ON, cursor OFF
    lcd_command(0x06); // Increment cursor
    lcd_command(0x01); // Clear screen
    _delay_ms(2);
}

// This function is displaying LED state and light level on the first LCD line
void lcd_firstLineStatus(uint16_t adc_value, bool led_on) {
    char buffer[17];
    lcd_command(0x80); // Move cursor to line 1 (address 0x00)
    if (led_on)
        snprintf(buffer, sizeof(buffer), "LED ON  L:%4u", adc_value);
    else
        snprintf(buffer, sizeof(buffer), "LED OFF L:%4u", adc_value);
    lcd_string(buffer);
}

// This function is displaying a custom message on the second LCD line
void lcd_secondLineMessage(const char* msg) {
    lcd_command(0xC0); // Move cursor to line 2 (address 0x40)
    for (int i = 0; i < 16; i++) {
        if (msg[i] == '\0') break; // Stop if end of string
        lcd_char(msg[i]);
    }
    // Padding the rest of line with spaces
    for (int i = strlen(msg); i < 16; i++) {
        lcd_char(' ');
    }
}
