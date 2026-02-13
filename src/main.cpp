#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <vfs_api.h>
#include <HardwareSerial.h>
#include "variables.hpp"
#include "pinout.hpp"
#include "Telemetry.hpp"
#include "SD.hpp"
#include "sensor.hpp"
#include "PWM.hpp"

// #region Timer Objects
hw_timer_t *timer_data = NULL;       //Data interrupt timer
hw_timer_t *timer_gps = NULL;        //GPS update timer
hw_timer_t *timer_PWM = NULL;        //PWM control timer
// #endregion

// #region Interrupt Handlers
// Triggers data collection & telemetry
void IRAM_ATTR timer_interrupt(){
  DATA_FLAG = true;
}

// Triggers GPS data parsing
void IRAM_ATTR gps_interrupt(){
  GPS_TIMER_FLAG = true;
}

// Triggers PWM motor control update
void IRAM_ATTR PWM_interrupt(){
  PWM_FLAG = true;
}

// Steering wheel button press handler
void IRAM_ATTR button_press(){
  BUTTON = true;
}
// #endregion

// #region Task Functions
// Reads sensors & updates PWM every 10ms
void ECU_task_loop( void * pvParameters ){
  for(;;){
    if (PWM_FLAG) {
      Serial.println("PWM FLAG");
      if(PWM_SLOWDOWN) PWM_controle_slowdown();
      else PWM_controle();
      PWM_FLAG = false;
    }

    get_temperatures();
    get_tensions();
    get_current();
    if(MPU_FLAG) get_mpu();
    write_buffers();
    if(GPS_TIMER_FLAG){
        Serial.println("GPS FLAG");
        get_gps();
        for(int i = 0; i < speed_buffer_size - 1; i++){
        speed_buffer[i] = speed_buffer[i + 1]; // shift values to the left
        }
        speed_buffer[speed_buffer_size - 1] = speed;
        GPS_TIMER_FLAG = false;
    }
    vTaskDelay(10);
  }
}
// #endregion

// #region Initialization
// Configure pins, timers, & startup tasks
void setup() {
  Serial.begin(115200); //Start the serial with 115 200 Bauds
  Wire.begin();
  Serial.println("Start setup");

  pinMode(speed_potentiometer_PIN, INPUT);
  pinMode(temperature_MOSFET_PIN, INPUT);
  pinMode(temperature_batterie_PIN, INPUT);
  // pinMode(temperature_motor_PIN, INPUT);
  pinMode(tension_batterie_PIN, INPUT);
  pinMode(tension_motor_PIN, INPUT);
  pinMode(current_sensor_PIN, INPUT);

  pinMode(RX1_PIN, INPUT);
  pinMode(TX1_PIN, INPUT);
  pinMode(RX2_PIN, INPUT);
  pinMode(TX2_PIN, INPUT);

  pinMode(PWM_PIN, OUTPUT);
  ledcSetup(PWM_CHANNEL, 17200, 12);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);                     //Attach the PWM pin to generate a PWM signal, frequency : 17200, resolution: 12 (nb of bits)
  Serial.println("PWM pin attached");

  pinMode(button_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_PIN), button_press, RISING); //Attach the steering wheel button to interrupt
  Serial.println("Pin interrupt button");

  timer_data = timerBegin(0, 80, true);                          // Timer 0, clock divisor 80
  timerAttachInterrupt(timer_data, &timer_interrupt, true);        // Attach the interrupt handling function
  timerAlarmWrite(timer_data, 500000, true);
// Interrupt every 0.5 second
  Serial.println("Timer DATA setup");

  timer_gps = timerBegin(1, 80, true);                          // Timer 0, clock divisor 80
  timerAttachInterrupt(timer_gps, &gps_interrupt, true);        // Attach the interrupt handling function
  timerAlarmWrite(timer_gps, 100000, true);
                 // Interrupt every 0.1 second
  Serial.println("Timer GPS setup");

  timer_PWM = timerBegin(2, 80, true);                          // Timer 0, clock divisor 80
  timerAttachInterrupt(timer_PWM, &PWM_interrupt, true);        // Attach the interrupt handling function
  timerAlarmWrite(timer_PWM, 10000, true);
                   // Interrupt every 0.1 second
  Serial.println("Timer GPS setup");


  if(SERVER) wifi_mqtt_connection();
  if(GPS_FLAG) gpsSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);  //Start the NEO-6M GPS with 9600 bauds with the UART2 connection
  if(MPU_FLAG) mpu.setup(0x68);                                    // Default I2C adress of the MPU9250
  if(SD_FLAG) init_SD_card();

  timerAlarmEnable(timer_PWM);
  timerAlarmEnable(timer_gps);
  timerAlarmEnable(timer_data);

  xTaskCreatePinnedToCore(telemetrie_task_loop,"Telemetrie",10000,NULL,1,&telemetrie_task,0);
  delay(100);
  xTaskCreatePinnedToCore(ECU_task_loop,"ECU",10000,NULL,1,&ECU,1);
  delay(100);
  Serial.println("End of SetUP");
}
// #endregion

// #region Main Loop
void loop() {}
