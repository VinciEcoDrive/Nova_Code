#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <vfs_api.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SD.h>
#include <MPU9250.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C screen_1(U8G2_R0, U8X8_PIN_NONE); //I2C address : 0x3C

TinyGPSPlus gps;                //Init GPS object
HardwareSerial gpsSerial(2);    //Init GPS Serial connection
MPU9250 mpu;                    //Init MPU object
hw_timer_t *timer_data = NULL;       //Init the timer
hw_timer_t *timer_gps = NULL;       //Init the timer
hw_timer_t *timer_PWM = NULL; 
File fileSD;                    
WiFiClient espClient;
PubSubClient client(espClient);
TaskHandle_t ECU;
TaskHandle_t telemetrie_task;

const char* serverName = "http://telemetrie.vinciecodrive.fr/telemetry";   

#define speed_potentiometer_PIN   39  
#define temperature_MOSFET_PIN    34
#define temperature_batterie_PIN  35
#define temperature_motor_PIN     32
#define tension_batterie_PIN      33
#define tension_motor_PIN         32
#define current_sensor_PIN        36
#define button_PIN                13
#define PWM_PIN                   27
#define SDA_PIN                   21
#define SCL_PIN                   22
#define CS_PIN                    5
#define MISO_PIN                  19
#define MOSI_PIN                  23
#define SCK_PIN                   18 
#define RX0_PIN                   3
#define TX0_PIN                   1
#define RX1_PIN                   14
#define TX1_PIN                   12
#define RX2_PIN                   16
#define TX2_PIN                   17

#define VCC_ACS770      5.041   //VCC of the current sensor
#define SENSTA          0.08    //Sensisivity for the current sensor
#define IERROM          0.120   //Magnetic offset error of the current sensor
#define LIST_SIZE       100     //Size of the buffer use to mean the values of the sensors
#define limited_current 12      //Current max allowed in the system
#define sw_inclination  1.3     //Inclination init
#define delta           10
#define resistor_1      6.2
#define resistor_2      100.0
#define wifi_name       "OnePlus 8T"
#define wifi_password   "uhcd2281"
// #define wifi_name       "Shell Eco-marathon" //"R-Toukour" // //"OnePlus 8T" //   //"Miti-phone"  //PoleDeVinci_IFT"Antoine's iPhone"
// #define wifi_password   "" //"uhcd2281"////     "00000000"//"Tbucsi123"   //*c.r4UV@VfPn_0 "Antoine2"
#define MQTT_PORT       1883
#define MQTT_PUB        "pe/telemetrie"

//do NOT touch 
bool DATA_FLAG = false;          //Flag use to send data to the telemetrie interrupt
bool GPS_TIMER_FLAG = false;
bool PWM_FLAG = false;
bool BUTTON = false;             //Flag for the steeringwheel button

bool PWM_SLOWDOWN = true;
bool DEBUG = true;               //Flag for Debuging or driver mode   
bool SERVER = true;              //Flag for the telemetrie sending
bool GPS_FLAG = true;           //Flag for the GPS connection
bool MPU_FLAG = false;           //Flag for the gyro sensor
bool SCREEN_FLAG = false;        //Flag for the screen diplay
bool SD_FLAG = false;            //Flag dor the SD card

String path;
uint8_t index_list = 0;         //Current index in the buffers 
uint8_t indexSD = 0;
uint8_t httpResponseCode = 0;   //Response from the server
uint16_t dutyCycle = 5;         //Duty Cycle use for the motor control
const char* mqtt_server = "5.250.176.118";
const char* mqtt_client_id = "ESP32Client";
int PWM_CHANNEL = 10;

const uint8_t speed_buffer_size = 5;
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

float latitude;
float longitude;

float DATA[11] = {0}; // TMOT- TBAT - TMOS - VMOT - VBAT - CUR - SPD - LNG - LAT - DTY - GYRO

void IRAM_ATTR timer_interrupt(){
  DATA_FLAG = true;
}

void IRAM_ATTR gps_interrupt(){
  GPS_TIMER_FLAG = true;
}

void IRAM_ATTR PWM_interrupt(){
  PWM_FLAG = true;
}

void IRAM_ATTR button_press(){
  BUTTON = true;
} 

void init_SD_card(){
  if(!SD.begin(CS_PIN)){return;}                                        //Begin the communication
  indexSD = 0;                                                  //Set index to 0
  while (SD.exists("data" + String(indexSD) + ".csv")) indexSD++;       //Look for the last dataX.csv file
  path = "data" + String(indexSD) + ".csv";
  fileSD = SD.open(path, FILE_WRITE);      //Create a new data(X+1).csv
  fileSD.println("TMOT;TBAT;TMOS;VMOT;VBAT;CUR;SPD;LNG;LAT;DTY;GYR");   //Print the header of the csv
}

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

void get_tensions(){
  float vin_batterie = (analogRead(tension_batterie_PIN) * 3.3) / 4095.0;
  tension_batterie = (vin_batterie * (resistor_1 + resistor_2)) / resistor_1; 

  float vin_motor = (analogRead(tension_motor_PIN) * 3.3) / 4095.0;
  tension_motor = tension_batterie - ((vin_motor * (resistor_1 + resistor_2)) / resistor_1);  
  if (tension_motor < 0) tension_motor = 0;
}

void get_current(){
  float value_current_sensor_PIN = analogRead(current_sensor_PIN);                    
  float voltage = ((5.0 / 4095.0) * value_current_sensor_PIN) - (VCC_ACS770 * 0.1);
  current = abs(voltage / SENSTA) - IERROM;
}

void get_mpu(){
  if (mpu.update()) {                             //Check if the MPU update new values
    rotation = (sw_inclination)*mpu.getPitch();   //Calculate the rotation of the steering wheel based on it's inclination and the pitch of the sensor
    }
  mpu.update();
}

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

void write_buffers(){                                             
  index_list = (index_list - 1 + LIST_SIZE) % LIST_SIZE;          // Decrement index_list with wrap-around
  temperature_MOSFET_buffer[index_list] = temperature_MOSFET;
  temperature_batterie_buffer[index_list] = temperature_batterie;
  temperature_motor_buffer[index_list] = temperature_motor;
  tension_batterie_buffer[index_list] = tension_batterie;
  tension_motor_buffer[index_list] = tension_motor;
  current_buffer[index_list] = current;
  rotation_buffer[index_list] = rotation;
}

float mean(const float* buffer) {
    double total = 0;
    for (int i = 0; i < LIST_SIZE; i++) {
        total += buffer[i];
    }
    return total / LIST_SIZE;
}

void write_DATA(){
  DATA[0] = mean(temperature_motor_buffer);
  DATA[1] = mean(temperature_batterie_buffer);
  DATA[2] = mean(temperature_MOSFET_buffer);
  DATA[3] = mean(tension_motor_buffer);
  DATA[4] = mean(tension_batterie_buffer);
  DATA[5] = mean(current_buffer);
  DATA[6] = speed;                            //All the value from the GPS don't need to be
  DATA[7] = longitude;                        //stored in buffer, they are already filter 
  DATA[8] = latitude;                         //with the library
  DATA[9] = dutyCycle;
  DATA[10] = mean(rotation_buffer);
}

void write_SD_card(){
  String data_SD = "";                            //Init the sd card message as an empty string
  for(int i = 0; i < 11; i++){  
    data_SD += String(DATA[i], 6) + ";";          //Write all the sensor value with a ';' as separator
  }
  data_SD.remove(data_SD.length() - 1);           //Remove the last ';'

  fileSD = SD.open(path, FILE_WRITE);    //Oper the file we create during the initiation
  if(fileSD){
    fileSD.println(data_SD);                                          //Write the message
    fileSD.close();                                                   //Close the file
  }
}

void display_screen_DEBUG(){

  screen_1.clearBuffer();               //Start by cleaning the buffer

  if(BUTTON){                           //The button flag allow the screen to switch to other sensors value because not everything fit in once
    screen_1.setCursor(0,10);           //Print the Motor temperature
    screen_1.print("TMOT: ");
    screen_1.print(DATA[0], 2);
    screen_1.print("°C");
    
    // screen_1.setCursor(0,20);
    // screen_1.print("TBAT: ");           //Print the batterie temperature
    // screen_1.print(DATA[1], 2);
    // screen_1.print("°C");

    screen_1.setCursor(0,20);           //Print the MOSFET temperature
    screen_1.print("TMOS: ");
    screen_1.print(DATA[2], 2);
    screen_1.print("°C");
    
    screen_1.setCursor(0,30);            //Print the motor tension
    screen_1.print("VMOT: ");
    screen_1.print(DATA[3], 2);
    screen_1.print("V");

    screen_1.setCursor(0,40);           //Print the batterie tension DATA[4]
    screen_1.print("VBAT: ");
    screen_1.print(DATA[4], 2);
    screen_1.print("V");

    screen_1.setCursor(0,50);           //Print the current
    screen_1.print("CUR: ");
    screen_1.print(DATA[5], 2);
    screen_1.print("A");

    screen_1.setCursor(0,60);           //Print the http response
    screen_1.print("http: ");
    screen_1.print(httpResponseCode);
  }

  else{
    screen_1.setCursor(0,10);           //Print the speed
    screen_1.print("SPD: ");
    screen_1.print(DATA[6], 2);
    screen_1.print("KmH");
    
    screen_1.setCursor(0,20);           //Print the longitude
    screen_1.print("LNG: ");
    screen_1.print(DATA[7], 2);

    screen_1.setCursor(0,30);           //Print the latitude
    screen_1.print("LAT: ");
    screen_1.print(DATA[8], 2); 

    screen_1.setCursor(0,40);           //Print the Streering wheel inclination
    screen_1.print("GYR: ");
    screen_1.print(DATA[10], 2); 
    screen_1.print("°");
    
    screen_1.setCursor(0,50);           //Print the duty Cycle
    screen_1.print("DTY: ");
    screen_1.print(DATA[9], 2); 

    screen_1.setCursor(0,60);           //Print the steering wheel potentiometer value
    screen_1.print("POTV: ");
    screen_1.print(analogRead(speed_potentiometer_PIN));

  }

  screen_1.sendBuffer();                //Send to the buffer 
}

void display_screen_DRIVER() {
  // display.clear();

  // // Barre de progression
  // int barWidth = map(analogRead(speed_potentiometer_PIN), 0, 4095, 0, 100);
  // display.drawRect(10, 10, 100, 10);
  // display.fillRect(10, 10, barWidth, 10);

  // // Ampérage
  // display.drawString(40, 30, String(String(DATA[5]) + " A"));
  // display.display();
}

void telemetrie(){ 
  //Check if the WIFI is well connected 
  if(WiFi.status()== WL_CONNECTED && client.connected()){ 
    client.loop();
    String httpRequestData = "{";                                 //Start a new string to store the json message to send
    httpRequestData += "\"TMOT\":" + String(DATA[0], 2) + ",";
    httpRequestData += "\"TBAT\":" + String(DATA[1], 2) + ",";
    httpRequestData += "\"TMOS\":" + String(DATA[2], 2) + ",";
    httpRequestData += "\"VMOT\":" + String(DATA[3], 2) + ",";
    httpRequestData += "\"VBAT\":" + String(DATA[4], 2) + ",";
    httpRequestData += "\"CUR\":" + String(DATA[5], 2) + ",";
    httpRequestData += "\"S\":" + String(DATA[6], 2) + ",";
    httpRequestData += "\"LONG\":" + String(DATA[7], 6) + ",";
    httpRequestData += "\"LAT\":" + String(DATA[8], 6) + ",";
    httpRequestData += "\"DUTY\":" + String(DATA[9]) + ",";
    httpRequestData += "\"GYRO\":" + String(DATA[10], 2);
    httpRequestData += "}";

  client.publish(MQTT_PUB, httpRequestData.c_str());
  delay(10);
  }
}

void wifi_mqtt_connection(){
  Serial.println("Start connections");
  unsigned long connection_timer = millis();

  WiFi.begin(wifi_name, wifi_password);      //Start the WIFI connection of the ESP32
  while(WiFi.status() != WL_CONNECTED) {              
    if (millis() - connection_timer > 10000){
      Serial.println("Fail to connect WIFI"); 
      return;
    } 
  }
  Serial.println("WIFI connected");

  connection_timer = millis();
  client.setServer(mqtt_server, MQTT_PORT);
  while (!client.connect(mqtt_client_id)) {
    if (millis() - connection_timer > 10000){
      Serial.println("Fail to link MQTT"); 
      return;
    } 
  }
  Serial.println("MQTT linked");
}


void telemetrie_task_loop(void *pvParameters) {
  for(;;){
    if(DATA_FLAG){
      Serial.println("DATA FLAG");
      write_DATA();
      if(SCREEN_FLAG){
        if(DEBUG) display_screen_DEBUG();
        else display_screen_DRIVER();
      }

      if(SERVER) telemetrie();
      if(SD_FLAG) write_SD_card();

      DATA_FLAG = !DATA_FLAG;
    }
    vTaskDelay(10);
  }
}



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

  if(SCREEN_FLAG) {
    screen_1.begin();                                     //Start the I2C oled screen 
    if(DEBUG){screen_1.setFont(u8g2_font_5x7_tr);}        //Choose the font based on the mode (DEBUG or DRIVER)
    else {screen_1.setFont(u8g2_font_5x7_tr);}
  }
  if(SERVER) wifi_mqtt_connection();
  if(GPS_FLAG) gpsSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);  //Start the NEO-6M GPS with 9600 bauds with the UART2 connection
  if(MPU_FLAG) mpu.setup(0x68);                                    // Default I2C adress of the MPU9250
  if(SD_FLAG) init_SD_card();

  timerAlarmEnable(timer_PWM);
  timerAlarmEnable(timer_gps);  
  timerAlarmEnable(timer_data);

  xTaskCreatePinnedToCore(telemetrie_task_loop,"Telemetrie",10000,NULL,1,&telemetrie_task,0);          /* pin task to core 0 */
  delay(100);
  xTaskCreatePinnedToCore(ECU_task_loop,"ECU",10000,NULL,1,&ECU,1);          /* pin task to core 1 */
  delay(100);                   
  Serial.println("End of SetUP");
}

void loop() {}
