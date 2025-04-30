#include <avr/io.h>
#include "segment.h"

// Defining pin mappings for 7-segment display shift register
#define LATCH PB0
#define DATA  PB1
#define CLOCK PD7

// This array is storing the 7-segment binary patterns for digits 0 to 9
const uint8_t segment_digits[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// This function is shifting out a byte to the 74HC595 shift register
void shiftOut(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        // Writing each bit to the DATA line
        if (value & (1 << i)) PORTB |= (1 << DATA);
        else                  PORTB &= ~(1 << DATA);

        // Clocking the bit into the shift register
        PORTD |=  (1 << CLOCK);
        PORTD &= ~(1 << CLOCK);
    }

    // Latching the shifted data to output
    PORTB |=  (1 << LATCH);
    PORTB &= ~(1 << LATCH);
}
