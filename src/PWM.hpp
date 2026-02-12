#pragma once
#include "sensor.hpp"
#include "variables.hpp"
#include "pinout.hpp"

extern int PWM_CHANNEL;
extern uint16_t dutyCycle;

void PWM_controle();
void PWM_controle_slowdown();
