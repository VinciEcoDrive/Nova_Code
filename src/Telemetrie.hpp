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
extern uint8_t index_list;         //Current index in the buffers

extern uint8_t httpResponseCode ;   //Response from the server

extern const char* mqtt_server ;
extern const char* mqtt_client_id;

extern WiFiClient espClient;
extern PubSubClient client;
extern TaskHandle_t ECU;
extern TaskHandle_t telemetrie_task;

void write_buffers();
void write_DATA();
void telemetrie();
void wifi_mqtt_connection();
void telemetrie_task_loop(void *pvParameters);

#endif
