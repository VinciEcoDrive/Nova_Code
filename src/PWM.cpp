#include "PWM.hpp"

int PWM_CHANNEL = 10;
uint16_t dutyCycle = 5;         //Duty Cycle use for the motor control


void PWM_controle(){
  //Delta controle the speed the dutyCycle will reach the potentiometer value
  uint16_t potentiometer_value = analogRead(speed_potentiometer_PIN);   //Get the potentiometer value
  if(potentiometer_value < 15) {
    potentiometer_value = 0;                 //Use it only if it's greater than 15
    dutyCycle = 0;
  }

  if(current <= limited_current){                                       //Check if the actual current is not greater than the limit current
    if(dutyCycle < potentiometer_value){                                //Check if the Duty Cycle is lower than the potentiometer value
      dutyCycle += delta;                                               //Add the Delta to the Duty Cycle if all condition are past
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
