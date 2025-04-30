#pragma once
#include <Arduino.h>

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Declaring a function that is setting up Timer0 for overflow-based timing
void timer_setup(void);

// Declaring a blocking wait function that is using ms_counter for millisecond delays
void wait_ms(uint16_t duration);

// Declaring the counter used to keep track of time in milliseconds (shared globally)
extern volatile uint16_t ms_counter;

#endif
