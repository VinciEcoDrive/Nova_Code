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

double latitude = 0.0;
double longitude = 0.0;
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
String getField(String trame, int fieldIndex) {
    int virguleCount = 0;
    int startPos = 0;
    int endPos;
    
    // Trouver le début du champ
    for (int i = 0; i < trame.length(); i++) {
        if (trame.charAt(i) == ',') {
            virguleCount++;
            if (virguleCount == fieldIndex) {
                startPos = i + 1;
                break;
            }
        }
    }
    
    // Si on a trouvé le début, trouver la fin (prochaine virgule ou fin de chaîne)
    if (virguleCount >= fieldIndex) {
        endPos = trame.indexOf(',', startPos);
        if (endPos == -1) {
            endPos = trame.length();
        }
        return trame.substring(startPos, endPos);
    }
    
    return "";
}

// Converts degrees/minutes to decimal coords
double dmm_to_decimal(String coord, String direction) {
    if (coord.length() == 0) return 0.0;
    
    double dmm = coord.toDouble();
    int degrees = (int)(dmm / 100);
    double minutes = dmm - (degrees * 100);
    double decimal = degrees + (minutes / 60.0);
    
    if (direction == "S" || direction == "W") {
        decimal *= -1;
    }   
    return decimal;
}

// Parses GPS NMEA RMC & GGA sentences
void get_gps(){
    while (gpsSerial.available() > 0) {
        String trame = gpsSerial.readStringUntil('\n');
        trame.trim(); // Enlève les caractères de retour chariot
        
        if (trame.startsWith("$GPGGA")) {
            String raw_lat = getField(trame, 2);  // Champ 2 = latitude
            String lat_dir = getField(trame, 3);  // Champ 3 = direction latitude
            String raw_lng = getField(trame, 4);  // Champ 4 = longitude
            String lng_dir = getField(trame, 5);  // Champ 5 = direction longitude
            
            // Vérifier que les données sont valides
            if (raw_lat.length() > 0 && raw_lng.length() > 0) {
                latitude = dmm_to_decimal(raw_lat, lat_dir);
                longitude = dmm_to_decimal(raw_lng, lng_dir);
                
                // Debug - afficher les données brutes
                Serial.println("Trame GPGGA reçue");
                Serial.print("Lat brute: "); Serial.print(raw_lat);
                Serial.print(" Dir: "); Serial.print(lat_dir);
                Serial.print(" Lng brute: "); Serial.print(raw_lng);
                Serial.print(" Dir: "); Serial.println(lng_dir);
            }
        }
    }
}

