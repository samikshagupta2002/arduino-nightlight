#pragma once
#include <Arduino.h>

// Declaring a function that is initializing the ADC (Analog-to-Digital Converter)
void ADC_Init();

// Declaring a function that is reading analog data from the specified channel
int ADC_Read(byte channel);
