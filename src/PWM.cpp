//PWM FILE
// DO NOT TOUCH
//

#include "PWM.hpp"

#pragma region PWM Configuration
int PWM_CHANNEL = 10;
uint16_t dutyCycle = 5;         //Duty Cycle for motor control
#pragma endregion

#pragma region PWM Control Functions
// Controls motor speed via potentiometer value
void PWM_controle(){
  //Delta controle the speed the dutyCycle will reach the potentiometer value
  uint16_t potentiometer_value = analogRead(speed_potentiometer_PIN);   //Get the potentiometer value
  if(potentiometer_value < 15) {
    potentiometer_value = 0;                 //Use it only if it's greater than 15
    dutyCycle = 0;
  }

<<<<<<< Updated upstream
  if(current <= limited_current){                                       //Check if the actual current is not greater than the limit current
    if(dutyCycle < potentiometer_value){                                //Check if the Duty Cycle is lower than the potentiometer value
      dutyCycle += delta;                                               //Add the Delta to the Duty Cycle if all condition are past
=======
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

    if (dt >= 50) { 
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
>>>>>>> Stashed changes
    }
  }
  else dutyCycle -= delta;                                              //Remove the Delta to the Duty Cycle if the last condition is not pass

  if(dutyCycle > potentiometer_value) {
    uint16_t new_dutyCycle = dutyCycle - (delta * 3);
    if (new_dutyCycle > dutyCycle || dutyCycle > 10000){
      dutyCycle = 0;
    }
    else dutyCycle = new_dutyCycle;
  }  //If the Duty Cycle is greater than the potentiometer value, give the potentiometer value to the
                                                                        //duty cycle to efficently stop the motor asservissement
  ledcWrite(PWM_CHANNEL, dutyCycle);                                        //Write the PWM signal
}

// Slows motor with 5x delta multiplier
void PWM_controle_slowdown(){
  uint16_t potentiometer_value = analogRead(speed_potentiometer_PIN);   //Get the potentiometer value
  if(potentiometer_value < 15) {
    potentiometer_value = 0;                 //Use it only if it's greater than 15
  }

  if(current <= limited_current){                                       //Check if the actual current is not greater than the limit current
    if(dutyCycle < potentiometer_value){                                //Check if the Duty Cycle is lower than the potentiometer value
      dutyCycle += delta;                                               //Add the Delta to the Duty Cycle if all condition are past
    }
  }
  else dutyCycle -= delta;                                              //Remove the Delta to the Duty Cycle if the last condition is not pass

  if(dutyCycle > potentiometer_value) {
    uint16_t new_dutyCycle = dutyCycle - (delta * 5);
    if (new_dutyCycle > dutyCycle || dutyCycle > 10000){
      dutyCycle = 0;
    }
    else dutyCycle = new_dutyCycle;
  }
      //If the Duty Cycle is greater than the potentiometer value, give the potentiometer value to the                                                           //duty cycle to efficently stop the motor asservissement
  ledcWrite(PWM_CHANNEL, dutyCycle);                                        //Write the PWM signal
}
#pragma endregion
