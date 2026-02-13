#ifndef SD_HPP
#define SD_HPP
#pragma once
#include <FS.h>
#include <SD.h>
#include "pinout.hpp"

extern String path;
extern File fileSD;
extern uint8_t indexSD;
extern float DATA[11]; // TMOT- TBAT - TMOS - VMOT - VBAT - CUR - SPD - LNG - LAT - DTY - GYRO
void init_SD_card();
void write_SD_card();

#endif // SD_HPP
