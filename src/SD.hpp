#ifndef SD_HPP
#define SD_HPP
#pragma once
#include <FS.h>
#include <SD.h>
#include "pinout.hpp"

// #region SD Card Variables
extern String path;
extern File fileSD;
extern uint8_t indexSD;
// TMOT-TBAT-TMOS-VMOT-VBAT-CUR-SPD-LNG-LAT-DTY-GYRO
extern float DATA[11];
// #endregion

// #region SD Card Functions
// Initializes SD card & creates CSV header
void init_SD_card();
// Writes sensor data to CSV file
void write_SD_card();

#endif // SD_HPP
