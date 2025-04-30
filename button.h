#pragma once
#include <Arduino.h>

#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

// Declaring the function that is checking for a button press and toggling system state
void check_button(void);

// Declaring the system state variable as external so it can be shared across multiple files
extern volatile uint8_t system_running;

#endif
