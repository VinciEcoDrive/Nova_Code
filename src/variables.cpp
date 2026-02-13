//Variables file
// YOU CAN TOUCH THIS FILE, BUT BE CAREFUL
#include "variables.hpp"

// #region System Flags
// Interrupt flags for timing & control events
bool DATA_FLAG = false;          //Flag to send data to telemetry
bool GPS_TIMER_FLAG = false;     //GPS update timer flag
bool PWM_FLAG = false;           //PWM control timer flag
bool BUTTON = false;             //Steering wheel button press
// #endregion

// #region Feature Flags
// Enable/disable optional system features
bool PWM_SLOWDOWN = true;        //Enable safer deceleration
bool DEBUG = true;               //Debug or driver mode
bool SERVER = true;              //Enable telemetry transmission
bool GPS_FLAG = true;            //Enable GPS module
bool MPU_FLAG = false;           //Enable gyro sensor
bool SCREEN_FLAG = false;        //Enable display output
bool SD_FLAG = false;            //Enable SD card logging
// #endregion

// #region Server Configuration
const char* serverName = "http://telemetrie.vinciecodrive.fr/telemetry";
// #endregion
