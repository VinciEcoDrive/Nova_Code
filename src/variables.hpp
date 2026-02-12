#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#define wifi_name       "OnePlus 8T"
#define wifi_password   "uhcd2281"
// #define wifi_name       "Shell Eco-marathon" //"R-Toukour" // //"OnePlus 8T" //   //"Miti-phone"  //PoleDeVinci_IFT"Antoine's iPhone"
// #define wifi_password   "" //"uhcd2281"////     "00000000"//"Tbucsi123"   //*c.r4UV@VfPn_0 "Antoine2"
#define MQTT_PORT       1883
#define MQTT_PUB        "pe/telemetrie"

//do NOT touch
extern bool DATA_FLAG;          //Flag use to send data to the telemetrie interrupt
extern bool GPS_TIMER_FLAG;
extern bool PWM_FLAG;
extern bool BUTTON;             //Flag for the steeringwheel button

extern bool PWM_SLOWDOWN;
extern bool DEBUG;               //Flag for Debuging or driver mode
extern bool SERVER;              //Flag for the telemetrie sending
extern bool GPS_FLAG;           //Flag for the GPS connection
extern bool MPU_FLAG;           //Flag for the gyro sensor
extern bool SCREEN_FLAG;        //Flag for the screen diplay
extern bool SD_FLAG;            //Flag dor the SD card
extern const char* serverName;

extern float DATA[11]; // TMOT- TBAT - TMOS - VMOT - VBAT - CUR - SPD - LNG - LAT - DTY - GYRO

#endif // VARIABLES_HPP
