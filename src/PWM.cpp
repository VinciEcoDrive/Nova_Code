#include "PWM.hpp"
#include <Arduino.h>
#include <cmath>

#pragma region Configuration et Variables

int PWM_CHANNEL = 11; //Attention channel 10 ?
uint16_t dutyCycle = 5; 
float kp = 0.07;
float ki = 18.5;
float kd = 0.0;
//float fc;
float Ts = 0.01; //10ms        
float alpha = 0.3; //calcAlphaEMA(fc*Ts);      
float integral = 0;
float old_ef = 0;

#define PPR 500         //valeur à vérifier
volatile long pulse_count = 0;
unsigned long lastTimeRPM = 0;
float MotorSpeedreceive = 0; 
float MotorSpeedreference = 4620; // valeur à vérifier


#pragma endregion

#pragma region Fonctions Utilitaires

void IRAM_ATTR encoderISR() {
  pulse_count++;
}

float calcAlphaEMA(float fn) {
    if (fn <= 0) return 1;
    const float c = std::cos(2 * float(M_PI) * fn);
    return c - 1 + std::sqrt(c * c - 4 * c + 3);
}

void updateMotorSpeed() { //Calculate motor current speed with encodor impulsion
    unsigned long now = millis();
    unsigned long dt = now - lastTimeRPM;

    if (dt >= 200) { 
        noInterrupts(); // Protection de la variable volatile
        long pulses = pulse_count;
        pulse_count = 0;
        interrupts();

        MotorSpeedreceive = (float)pulses * (60000.0 / (PPR * dt)); //Convert pulses in Rotation per minutes
        lastTimeRPM = now;

        // Debug console
        Serial.print("Vitesse : ");
        Serial.print(MotorSpeedreceive);
        Serial.println(" RPM");
    }
}

#pragma endregion

#pragma region Contrôle PWM

void PWM_controle() {
    updateMotorSpeed();
    bool Pressed_Button;
    int threshold = 1000; 
      int value_Pressed_BUTTON_PIN = analogRead(Pressed_Button_PIN);
  float voltage_Pressed_Button = (((value_Pressed_BUTTON_PIN * 5) / 4095) *1000);

    if (voltage_Pressed_Button < threshold){ //threshold / security to detect pressed button
      Pressed_Button = false;
    }
    else{ Pressed_Button = true;}
       
    if(current < 15) { //Avoid noise
        Pressed_Button = false;
        dutyCycle = 0;
    }

    if(current > limited_current) { //security to avoid electrical overload
        int16_t new_dutyCycle = (int16_t)dutyCycle - (delta * 3);
         if(new_dutyCycle < 0){
          dutyCycle = 0;
         }
         else{ (uint16_t)new_dutyCycle;}
        integral = 0; // Reinitialize Pid to 0 
    }
    else if(current <= limited_current) {
        if(Pressed_Button) {
            
            float error = MotorSpeedreference - MotorSpeedreceive; // difference between wanted and currant speed 
          
            float ef = alpha * error + (1 - alpha) * old_ef;
          
            float derivative = (ef - old_ef) / Ts; //filtered derivative
            
           
            float next_integral = integral + (error * Ts);  // Integration
            
            
            float Control_signal = (kp * error) + (ki * next_integral) + (kd * derivative); //PID formula

            old_ef = ef; //update for next iteration

            
            if (Control_signal > 1023) { //avoid saturation by limiting the value to 1023
                dutyCycle = 1023; //set to max value
            }
            else if (Control_signal < 0) {
                dutyCycle = 0;
                integral = 0;
            }
            else {
                dutyCycle = (uint16_t)Control_signal;
                integral = next_integral;
            }
        }
        else {
            integral = 0;
            old_ef = 0;
            dutyCycle =0;
        }
    }

    ledcWrite(PWM_CHANNEL, dutyCycle); //send dutyCycle 
}


void PWM_controle_slowdown() {
    uint16_t potentiometer_value = analogRead(Pressed_Button_PIN);
    if(potentiometer_value < 15) potentiometer_value = 0;

    if(current <= limited_current) {
        if(dutyCycle < potentiometer_value) {
            dutyCycle += delta;
        }
    } else {
        dutyCycle = (dutyCycle > delta) ? dutyCycle - delta : 0;
    }

    // Freinage plus agressif si on dépasse la consigne
    if(dutyCycle > potentiometer_value) {
        int16_t new_dutyCycle = (int16_t)dutyCycle - (delta * 5);
        dutyCycle = (new_dutyCycle < 0) ? 0 : (uint16_t)new_dutyCycle;
    }
    
    ledcWrite(PWM_CHANNEL, dutyCycle);
}

#pragma endregion