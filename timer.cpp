#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "button.h"

volatile uint8_t tick_50ms = 0;      // This flag is being set every overflow (~16ms)
volatile uint16_t ms_counter = 0;    // This counter is tracking time in milliseconds

// This is the Timer0 overflow ISR that runs every ~16ms
ISR(TIMER0_OVF_vect) {
    TCNT0 = 0;            // Resetting the counter
    tick_50ms = 1;        // Raising flag for timing use (not essential in current version)
    ms_counter += 16;     // Adding ~16ms to counter
}

// This function is setting up Timer0 in Normal mode with 1024 prescaler
void timer_setup() {
    TCCR0A = 0x00;                             // Setting Timer0 to Normal mode
    TCCR0B = (1 << CS00) | (1 << CS02);        // Using prescaler 1024
    TIMSK0 |= (1 << TOIE0);                    // Enabling Timer0 overflow interrupt
    TCNT0 = 0;                                 // Initializing timer counter
    sei();                                     // Enabling global interrupts
}

// This function is waiting for the specified delay using ms_counter
void wait_ms(uint16_t duration) {
    ms_counter = 0;             // Resetting delay timer
    while (ms_counter < duration) {
        check_button();         // Continuously checking the button during delay
    }
}
