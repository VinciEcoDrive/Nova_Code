#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <MPU9250.h>

#ifndef SENSOR_HPP
#define SENSOR_HPP

#define VCC_ACS770      5.041   //VCC of the current sensor
#define SENSTA          0.08    //Sensisivity for the current sensor
#define IERROM          0.120   //Magnetic offset error of the current sensor
#define LIST_SIZE       100     //Size of the buffer use to mean the values of the sensors
#define limited_current 12      //Current max allowed in the system
#define sw_inclination  1.3     //Inclination init
#define delta           10
#define resistor_1      6.2
#define resistor_2      100.0



extern TinyGPSPlus gps; //Init GPS object
extern HardwareSerial gpsSerial; //Init GPS Serial connection
extern MPU9250 mpu; //Init MPU object
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
extern float latitude;
extern float longitude;
const uint8_t speed_buffer_size = 5;
extern float speed_buffer[speed_buffer_size];
extern float speed;


extern float temperature_MOSFET_buffer[LIST_SIZE];
extern float temperature_MOSFET;
extern float temperature_batterie_buffer[LIST_SIZE] ;
extern float temperature_batterie;
extern float temperature_motor_buffer[LIST_SIZE] ;
extern float temperature_motor;
extern float tension_batterie_buffer[LIST_SIZE] ;
extern float tension_batterie;
extern float tension_motor_buffer[LIST_SIZE] ;
extern float tension_motor;
extern float current_buffer[LIST_SIZE] ;
extern float current;
extern float rotation_buffer[LIST_SIZE];
extern float rotation;

extern float latitude;
extern float longitude;

#endif

void get_temperatures();
void get_tensions();
void get_current();
void get_mpu();
void get_gps();

