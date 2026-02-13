//SD FILE
// DO NOT TOUCH
//

#include <FS.h>
#include <SD.h>
#include "pinout.hpp"
#include "variables.hpp"
#include "SD.hpp"

// #region SD Card Variables
String path;
File fileSD;
uint8_t indexSD = 0;
float DATA[11] = {0}; // TMOT-TBAT-TMOS-VMOT-VBAT-CUR-SPD-LNG-LAT-DTY-GYRO
// #endregion

// #region SD Card Functions
// Sets up SD card & creates new CSV file
void init_SD_card(){
  if(!SD.begin(CS_PIN)){return;}                                        //Begin the communication
  indexSD = 0;                                                  //Set index to 0
  while (SD.exists("data" + String(indexSD) + ".csv")) indexSD++;       //Look for the last dataX.csv file
  path = "data" + String(indexSD) + ".csv";
  fileSD = SD.open(path, FILE_WRITE);      //Create a new data(X+1).csv
  fileSD.println("TMOT;TBAT;TMOS;VMOT;VBAT;CUR;SPD;LNG;LAT;DTY;GYR");   //Print the header of the csv
}

// Appends sensor data row to CSV
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
// #endregion
