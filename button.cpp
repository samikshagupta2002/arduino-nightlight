#include <avr/io.h> 
#include <util/delay.h>
#include "usart.h"
#include "lcd.h"
#include "button.h"

#define BUTTON PB2  // Defining the button pin as PB2

uint8_t last_button_state = 1;             // Storing the previous state of the button (1 = not pressed)
volatile uint8_t system_running = 0;       // Indicating whether the system is running or paused

/*
 * This function is checking the state of the push button.
 * It is detecting a falling edge and toggling the system state accordingly.
 * It is also displaying messages on the LCD and sending updates via USART.
 */
void check_button(void) {
    // Reading the current state of the button (1 = not pressed, 0 = pressed)
    uint8_t new_state = (PINB & (1 << BUTTON)) ? 1 : 0;

    // Detecting falling edge: button just pressed
    if (new_state == 0 && last_button_state == 1) {
        _delay_ms(50);  // Adding delay to debounce the button

        // Confirming button is still pressed after debounce
        if (((PINB & (1 << BUTTON)) ? 1 : 0) == 0) {
            // Toggling the system state
            system_running = !system_running;

            // Updating status on LCD and via serial
            if (system_running)
                USART_send_string("System RESUMED\n");
            else {
                USART_send_string("System PAUSED\n");
                lcd_secondLineMessage("System PAUSED");
            }
        }
    }

    // Storing current state for next comparison
    last_button_state = new_state;
}
