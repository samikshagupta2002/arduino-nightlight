#pragma once
#include <Arduino.h>

#ifndef SEGMENT_H
#define SEGMENT_H

#include <avr/io.h>

// Declaring a function to shift out data to a 7-segment display via shift register
void shiftOut(uint8_t value);

// Declaring an external array that holds the binary segment patterns for digits 0–9
extern const uint8_t segment_digits[10];

#endif
