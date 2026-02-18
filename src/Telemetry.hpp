#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include "sensor.hpp"
#include "variables.hpp"
#include "PWM.hpp"
#include "SD.hpp"

#ifndef TELEMETRIE_HPP
#define TELEMETRIE_HPP

#pragma region Telemetry Variables
extern uint8_t index_list;         //Current index in buffers
extern uint8_t httpResponseCode ;   //Server response code
#pragma endregion

#pragma region MQTT Configuration
extern const char* mqtt_server ;
extern const char* mqtt_client_id;
#pragma endregion

#pragma region WiFi & MQTT Clients
extern WiFiClient espClient;
extern PubSubClient client;
extern TaskHandle_t ECU;
extern TaskHandle_t telemetrie_task;
#pragma endregion

#pragma region Telemetry Functions
// Fills sensor buffers with latest values
void write_buffers();
// Averages buffers & stores in DATA array
void write_DATA();
// Publishes DATA via MQTT to server
void telemetrie();
// Establishes WiFi & MQTT connections
void wifi_mqtt_connection();
// Task loop for telemetry operations
void telemetrie_task_loop(void *pvParameters);

#endif
