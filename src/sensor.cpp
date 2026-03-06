//SENSOR FILE
// DO NOT TOUCH
//

#include "sensor.hpp"
#include "pinout.hpp"
#include "variables.hpp"

#pragma region Sensor Objects
// Global variable definitions
TinyGPSPlus gps;                //Init GPS object
HardwareSerial gpsSerial(2);    //Init GPS Serial connection
MPU9250 mpu;                    //Init MPU object
#pragma endregion

#pragma region Sensor Buffers & Variables
float speed_buffer[speed_buffer_size] = {0};
float speed;

float temperature_MOSFET_buffer[LIST_SIZE] = {0};
float temperature_MOSFET;
float temperature_batterie_buffer[LIST_SIZE] = {0};
float temperature_batterie;
float temperature_motor_buffer[LIST_SIZE] = {0};
float temperature_motor;
float tension_batterie_buffer[LIST_SIZE] = {0};
float tension_batterie;
float tension_motor_buffer[LIST_SIZE] = {0};
float tension_motor;
float current_buffer[LIST_SIZE] = {0};
float current;
float rotation_buffer[LIST_SIZE] = {0};
float rotation;

double latitude;
double longitude;
#pragma endregion

#pragma region Temperature & Voltage Functions
// Reads & converts temperature sensor values
void get_temperatures(){
  int value_temperature_MOSFET_PIN = analogRead(temperature_MOSFET_PIN);
  float temperature_MOSFET_mv = ((value_temperature_MOSFET_PIN * 3.3) / 4095 ) * 1000;
  temperature_MOSFET = (temperature_MOSFET_mv - 500) /10;

  int value_temperature_batterie_PIN = analogRead(temperature_batterie_PIN);
  float temperature_batterie_mv = ((value_temperature_batterie_PIN * 3.3) / 4095 ) * 1000;
  temperature_batterie = (temperature_batterie_mv - 500) /10;

  // int value_temperature_motor_PIN = analogRead(temperature_motor_PIN);
  // float temperature_motor_mv = ((value_temperature_motor_PIN * 3.3) / 4095 ) * 1000;
  // temperature_motor = (temperature_motor_mv - 500) /10;
}

// Reads battery & motor voltages via dividers
void get_tensions(){
  float vin_batterie = (analogRead(tension_batterie_PIN) * 3.3) / 4095.0;
  tension_batterie = (vin_batterie * (resistor_1 + resistor_2)) / resistor_1;

  float vin_motor = (analogRead(tension_motor_PIN) * 3.3) / 4095.0;
  tension_motor = tension_batterie - ((vin_motor * (resistor_1 + resistor_2)) / resistor_1);
  if (tension_motor < 0) tension_motor = 0;
}

// Calculates current from ACS770 sensor
void get_current(){
  float value_current_sensor_PIN = analogRead(current_sensor_PIN);
  float voltage = ((5.0 / 4095.0) * value_current_sensor_PIN) - (VCC_ACS770 * 0.1);
  current = abs(voltage / SENSTA) - IERROM;
}
#pragma endregion

#pragma region MPU & GPS Functions
// Reads MPU9250 pitch for steering angle
void get_mpu(){
  if (mpu.update()) {                             //Check if the MPU update new values
    rotation = (sw_inclination)*mpu.getPitch();   //Calculate the rotation of the steering wheel based on it's inclination and the pitch of the sensor
    }
  mpu.update();
}

// Extracts substring from NMEA sentence
String scrap_gps(String trame, int index, int len){
  int virguleCount = 0;
  String result = "";
  for (int i = 0; i < trame.length(); i++) {
    if (trame.charAt(i) == ',') {
      virguleCount++;
      if(virguleCount == index){
        result = trame.substring(i + 1, i + len);
        break;
      }
    }
  }
  return result;
}

// Converts degrees/minutes to decimal coords
float dmm_to_decimal(String coord, String direction) {
  float dmm = coord.toFloat();
  int degrees = (int)(dmm / 100);
  float minutes = dmm - (degrees * 100);
  float decimal = degrees + (minutes / 60.0);
  if (direction == "S" || direction == "W") {
    decimal *= -1;
  }
  return decimal;
}

// Parses GPS NMEA RMC & GGA sentences
void get_gps(){
  String trame = "";
  String raw_speed;
  String raw_latitude;
  String latitude_letter;
  String raw_longitude;
  String longitude_letter;

  while(gpsSerial.available() > 0){
    char c = gpsSerial.read();
    if (c == '\n') {
      if (trame.startsWith("$GPRMC")) {
        raw_speed = scrap_gps(trame, 7, 5);
        speed = raw_speed.toFloat() * 1.852;
      }
      else if (trame.startsWith("$GPGGA")) {
        raw_latitude = scrap_gps(trame, 2, 9);
        latitude_letter = scrap_gps(trame, 3, 1);
        latitude = dmm_to_decimal(raw_latitude, latitude_letter);

        raw_longitude = scrap_gps(trame, 4, 9);
        longitude_letter = scrap_gps(trame, 5, 1);
        longitude = dmm_to_decimal(raw_longitude, longitude_letter);
      }
      trame = "";
    }
    else if (c != '\r') {
      trame += c;
    }
  }
}
#pragma endregion
