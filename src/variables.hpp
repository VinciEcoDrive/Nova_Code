#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#pragma region WiFi & MQTT Configuration
#define wifi_name       "OnePlus 8T"
#define wifi_password   "uhcd2281"
// #define wifi_name       "Shell Eco-marathon" //"R-Toukour" // //"OnePlus 8T" //   //"Miti-phone"  //PoleDeVinci_IFT"Antoine's iPhone"
// #define wifi_password   "" //"uhcd2281"////     "00000000"//"Tbucsi123"   //*c.r4UV@VfPn_0 "Antoine2"
#define MQTT_PORT       1883
#define MQTT_PUB        "pe/telemetrie"
#pragma endregion

#pragma region System Flags
// Interrupt flags for timing & control events
extern bool DATA_FLAG;          //Flag to send data to telemetry
extern bool GPS_TIMER_FLAG;     //GPS update timer flag
extern bool PWM_FLAG;           //PWM control timer flag
extern bool BUTTON;             //Steering wheel button press
#pragma endregion

#pragma region Feature Flags
// Enable/disable optional system features
extern bool PWM_SLOWDOWN;       //Enable safer deceleration
extern bool DEBUG;               //Debug or driver mode
extern bool SERVER;              //Enable telemetry transmission
extern bool GPS_FLAG;           //Enable GPS module
extern bool MPU_FLAG;           //Enable gyro sensor
extern bool SCREEN_FLAG;        //Enable display output
extern bool SD_FLAG;            //Enable SD card logging
#pragma endregion

#pragma region Server Configuration
extern const char* serverName;
#pragma endregion

#pragma GPS variable
extern HardwareSerial gpsSerial;
extern double latitude;
extern double longitude;
#pragma endregion

#endif // VARIABLES_HPP
