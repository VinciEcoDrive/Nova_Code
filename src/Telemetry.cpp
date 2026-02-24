//Telemetrie FILE
// DO NOT TOUCH
//
#include "Telemetry.hpp"

#pragma region WiFi & MQTT Clients
// Définitions des variables globales externes
WiFiClient espClient;
PubSubClient client(espClient);
TaskHandle_t ECU;
TaskHandle_t telemetrie_task;
#pragma endregion

#pragma region Telemetry Configuration
uint8_t index_list = 0;         //Current index in buffers
uint8_t httpResponseCode = 0;   //Server response code
#pragma endregion

#pragma region MQTT Configuration
const char* mqtt_server = "37.59.113.108";
const char* mqtt_client_id = "ESP32Client";
#pragma endregion

#pragma region Telemetry Functions
// Shifts buffer values & stores latest sensor
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

// Calculates average from buffer values
float mean(const float* buffer) {
    double total = 0;
    for (int i = 0; i < LIST_SIZE; i++) {
        total += buffer[i];
    }
    return total / LIST_SIZE;
}

// Builds DATA array with averaged sensor values
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

// Publishes JSON data via MQTT if connected
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

// Connects WiFi & MQTT to server
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
#pragma endregion

#pragma region Task Loop
// Main telemetry task loop for data transmission
void telemetrie_task_loop(void *pvParameters) {
  for(;;){
    if(DATA_FLAG){
      Serial.println("DATA FLAG");
      write_DATA();

      if(SERVER) telemetrie();
      if(SD_FLAG) write_SD_card();

      DATA_FLAG = !DATA_FLAG;
    }
    vTaskDelay(10);
  }
}
#pragma endregion
