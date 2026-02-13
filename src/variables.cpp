//Variables file
// YOU CAN TOUCH THIS FILE, BUT BE CAREFUL
#include "variables.hpp"

// Define global variables
bool DATA_FLAG = false;          //Flag use to send data to the telemetrie interrupt
bool GPS_TIMER_FLAG = false;
bool PWM_FLAG = false;
bool BUTTON = false;             //Flag for the steeringwheel button

bool PWM_SLOWDOWN = true;
bool DEBUG = true;               //Flag for Debuging or driver mode
bool SERVER = true;              //Flag for the telemetrie sending
bool GPS_FLAG = true;            //Flag for the GPS connection
bool MPU_FLAG = false;           //Flag for the gyro sensor
bool SCREEN_FLAG = false;        //Flag for the screen diplay
bool SD_FLAG = false;            //Flag dor the SD card
const char* serverName = "http://telemetrie.vinciecodrive.fr/telemetry";

