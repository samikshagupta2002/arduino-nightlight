/*Group 25 - Room Night Light System

    * Members: Samiksha Gupta 
 * This is the main control file for the Room Night Light System project.
 * It is continuously checking light intensity and updating the LED, 7-segment, and LCD accordingly.
 * It is also handling the system state toggle using a button.
 */

 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdio.h>
 #include "adc.h"
 #include "usart.h"
 #include "lcd.h"
 #include "segment.h"
 #include "button.h"
 #include "timer.h"
 
 #define LED    PD6
 #define LATCH  PB0
 #define DATA   PB1
 #define CLOCK  PD7
 #define BUTTON PB2
 
 int main(void) {
     char text[MAX_TEXT];
 
     // Configuring the LED and CLOCK pins as outputs
     DDRD |= (1 << LED) | (1 << CLOCK);
 
     // Configuring the DATA and LATCH pins as outputs for 7-segment
     DDRB |= (1 << DATA) | (1 << LATCH);
 
     // Setting BUTTON pin as input with pull-up
     DDRB &= ~(1 << BUTTON);
     PORTB |= (1 << BUTTON);
 
     // Initializing modules
     USART_init();        // Setting up serial communication
     ADC_Init();          // Initializing ADC for photocell
     lcd_init();          // Initializing LCD in 4-bit mode
     lcd_string("Night Light"); // Displaying welcome message
     _delay_ms(1000);     // Holding message before clearing
     timer_setup();       // Setting up Timer0 for delay control
 
     while (1) {
         check_button(); // Continuously checking for button press
 
         if (system_running) {
             uint16_t light = ADC_Read(0); // Reading photocell value
             sprintf(text, "Light = %u\n", light);
             USART_send_string(text); // Sending light level to serial monitor
 
             if (light < 50) {
                 PORTD |= (1 << LED); // Turning ON LED when it's dark
                 lcd_firstLineStatus(light, 1); // Updating LCD: Night ON
                 lcd_secondLineMessage("Room is Dark"); // Showing status
 
                 for (int i = 9; i >= 0; i--) {
                     shiftOut(segment_digits[i]); // Displaying digit on 7-seg
                     ms_counter = 0;              // Resetting counter
 
                     while (ms_counter < 1000) {  // Waiting ~1s using timer
                         check_button();
                         if (!system_running) {   // Exiting early if paused
                             PORTD &= ~(1 << LED);
                             shiftOut(0x00);
                             lcd_firstLineStatus(light, 0);
                             break;
                         }
                     }
                     if (!system_running) break; // Exiting loop if paused
                 }
             } else {
                 PORTD &= ~(1 << LED); // Turning OFF LED when bright
                 shiftOut(0x00);       // Clearing 7-segment
                 lcd_firstLineStatus(light, 0);
                 lcd_secondLineMessage("Room is Bright ");
             }
         } else {
             PORTD &= ~(1 << LED);    // Turning off LED during pause
             shiftOut(0x00);          // Clearing 7-segment
             lcd_firstLineStatus(ADC_Read(0), 0); // Showing light level
         }
 
         wait_ms(100); // Using timer to delay next loop iteration
     }
 }
 
