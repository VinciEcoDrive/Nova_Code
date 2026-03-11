#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <MPU9250.h>
#include <HardwareSerial.h>

#ifndef SENSOR_HPP
#define SENSOR_HPP

#pragma region Sensor Configuration Constants
#define VCC_ACS770      5.041   //VCC of the current sensor
#define SENSTA          0.08    //Sensisivity for the current sensor
#define IERROM          0.120   //Magnetic offset error
#define LIST_SIZE       100     //Buffer size for sensor averages
#define limited_current 12      //Max current limit in amps
#define sw_inclination  1.3     //Steering wheel inclination init
#define delta           10      //PWM acceleration step
#define resistor_1      6.2     //Voltage divider resistor 1
#define resistor_2      100.0   //Voltage divider resistor 2
#pragma endregion

#pragma region Sensor Objects & Serial
extern TinyGPSPlus gps; //Init GPS object
extern HardwareSerial gpsSerial; //Init GPS Serial connection
extern MPU9250 mpu; //Init MPU object
#pragma endregion

#pragma region Sensor Buffers & Variables
extern float temperature_MOSFET_buffer[LIST_SIZE];
extern float temperature_MOSFET;
extern float temperature_batterie_buffer[LIST_SIZE];
extern float temperature_batterie;
extern float temperature_motor_buffer[LIST_SIZE];
extern float temperature_motor;
extern float tension_batterie_buffer[LIST_SIZE];
extern float tension_batterie;
extern float tension_motor_buffer[LIST_SIZE];
extern float tension_motor;
extern float current_buffer[LIST_SIZE];
extern float current;
extern float rotation_buffer[LIST_SIZE];
extern float rotation;
extern double latitude;
extern double longitude;
const uint8_t speed_buffer_size = 5;
extern float speed_buffer[speed_buffer_size];
extern float speed;
#pragma endregion

#pragma region Sensor Reading Functions
// Reads MOSFET & battery temperatures
void get_temperatures();
// Reads motor & battery voltages
void get_tensions();
// Reads motor current from sensor
void get_current();
// Reads MPU9250 gyro/accel data
void get_mpu();
// Parses GPS NMEA sentences data
void get_gps();
#pragma endregion

#endif // SENSOR_HPP
