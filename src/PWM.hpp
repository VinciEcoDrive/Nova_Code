#pragma once
#include "sensor.hpp"
#include "variables.hpp"
#include "pinout.hpp"

// #region PWM Configuration
extern int PWM_CHANNEL;
extern uint16_t dutyCycle;

// #endregion

// #region PWM Control Functions
// Manages motor PWM with speed & current limits
void PWM_controle();
// Applies reduced delta for safer deceleration
void PWM_controle_slowdown();
