#ifndef SD_HPP
#define SD_HPP

#include <FS.h>
#include <SD.h>
#include "pinout.hpp"

extern String path;
extern File fileSD;
extern uint8_t indexSD;

void init_SD_card();
void write_SD_card();

#endif // SD_HPP
