#pragma once
#include "sensor.hpp"
#include "variables.hpp"
#include "pinout.hpp"

// PWM
extern int PWM_CHANNEL;
extern uint16_t dutyCycle;

// PID parameters
extern float kp;
extern float ki;
extern float kd;
extern float Ts;
extern float alpha;
extern bool Pressed_button;
extern float integral;
extern float old_ef;

extern float MotorSpeedreference;
extern float MotorSpeedreceive;

void IRAM_ATTR encoderISR();
void PWM_controle();
void PWM_controle_slowdown();
float calcAlphaEMA(float fn);
