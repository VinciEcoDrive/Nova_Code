#include "PWM.hpp"
#include <Arduino.h>
#include <cmath>

#pragma region Configuration et Variables

int PWM_CHANNEL = 10; 
uint16_t dutyCycle = 5; 
float kp = 0.1; 
float ki = 0.05; //18.5 donne un Control_signal était supérieur à 1023*
float kd = 0.0;
//float fc;
float Ts = 0.2; //200ms        
float alpha = 0.3; //calcAlphaEMA(fc*Ts); //alpha to filter derivative because the encoder can be noisy  
float integral = 0;
float old_ef = 0;

#define PPR 500      //valeur à vérifier tester à 1024 et 2048 pour voir le rapport précision/saturation.
volatile long pulse_count = 0;
unsigned long lastTimeRPM = 0;
float MotorSpeedreceive = 0; 
float MotorSpeedreference = 4620; // valeur à vérifier
bool Pressed_button = false;


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
    if(digitalRead(Pressed_Button_PIN) == HIGH){ 
      Pressed_button = true;
    }
    else { 
      Pressed_button = false;
    }

    if(current > limited_current) {  //security to avoid electrical overload
        int16_t new_dutyCycle = (int16_t)dutyCycle - (delta * 3);
        if(new_dutyCycle < 0){
            dutyCycle = 0;
        }
        else { 
            dutyCycle = (uint16_t)new_dutyCycle;
        }
        integral = 0; // Réinitialise le PID
    }
    else if(current <= limited_current) {
        if(Pressed_button) {
            static unsigned long lastTimePID = 0;
            unsigned long now = millis();
            
            if (now - lastTimePID >= (Ts * 1000.0)) { // Ts est en secondes, on convertit en ms
                lastTimePID = now;
                
                float error = MotorSpeedreference - MotorSpeedreceive; 
                float ef = alpha * error + (1 - alpha) * old_ef;
                float derivative = (ef - old_ef) / Ts; 
                
                float next_integral = integral + (error * Ts);  
                
                // Formule PID
                float Control_signal = (kp * error) + (ki * next_integral) + (kd * derivative); 

                old_ef = ef; 

                // Anti-saturation
                if (Control_signal > 4095) { 
                    dutyCycle = 4095; 
                    // Optionnel: on peut geler l'intégrale ici pour éviter le windup
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
        }
        else {
            integral = 0;
            old_ef = 0;
            dutyCycle = 0;
        }
    }

    ledcWrite(PWM_CHANNEL, dutyCycle); 
}

void PWM_controle_slowdown() {
    
    uint16_t target_dutyCycle = Pressed_button ? 4095 : 0; 

    if(current <= limited_current) {
        if(dutyCycle < target_dutyCycle) {
            dutyCycle += delta;
        } else if (dutyCycle > target_dutyCycle) {
            // Freinage plus lent
            dutyCycle = (dutyCycle > delta) ? dutyCycle - delta : 0;
        }
    } else {
        int16_t new_dutyCycle = (int16_t)dutyCycle - (delta * 5);
        dutyCycle = (new_dutyCycle < 0) ? 0 : (uint16_t)new_dutyCycle;
    }
    
    ledcWrite(PWM_CHANNEL, dutyCycle);
}

#pragma endregion