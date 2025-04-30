#include "adc.h" 
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

/*
 * This function is initializing the ADC system.
 * It is configuring PC0 as input, enabling ADC with auto-trigger, 
 * setting the clock prescaler to 128, and selecting AVcc as the reference voltage.
 */
void ADC_Init() {										
	DDRC &= ~(1 << PC0); // Making ADC pin (PC0) an input

	// Enabling ADC, enabling auto-trigger mode, and setting prescaler to 128
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Setting AVcc as reference voltage and selecting ADC0 as input channel
	ADMUX = 0x40;

	// Disabling digital input buffer on ADC0 (optional, to save power)
	DIDR0 = 0x01;
}

/*
 * This function is reading the analog value from the specified ADC channel.
 * It is starting the conversion and waiting until the conversion is complete.
 * It is returning the final 10-bit digital value from the ADC.
 */
int ADC_Read(byte channel) {
	// Selecting the desired ADC channel (0 to 7)
	ADMUX = 0x40 | (channel & 0x07);

	// Starting the conversion
	ADCSRA |= (1 << ADSC);

	// Waiting until the conversion completes (ADSC bit becomes 0)
	while (!(ADCSRA & (1 << ADSC)));

	// Adding a small delay to allow ADC result to settle
	_delay_ms(1);

	// Returning the ADC result (0–1023)
	return ADCW;
}
