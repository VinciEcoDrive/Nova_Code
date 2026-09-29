//SD FILE
// DO NOT TOUCH
//

#include <FS.h>
#include <SD.h>
#include "pinout.hpp"
#include "variables.hpp"
#include "SD.hpp"

#pragma region SD Card Variables
String path;
File fileSD;
uint8_t indexSD = 0;
float DATA[13] = {0}; // TMOT-TBAT-TMOS-VMOT-VBAT-CUR-SPD-LNG-LAT-DTY-GYRO
#pragma endregion

#pragma region SD Card Functions
// Sets up SD card & creates new CSV file
void init_SD_card(){
  if(!SD.begin(CS_PIN)){
    Serial.println("Erreur de carte SD !");
    return;
  }
  
  indexSD = 0;
  // Ajout du "/" pour la racine
  while (SD.exists("/data" + String(indexSD) + ".csv")) indexSD++;
  path = "/data" + String(indexSD) + ".csv";

  fileSD = SD.open(path, FILE_WRITE);
  if(fileSD){
    fileSD.println("TMOT;TBAT;TMOS;VMOT;VBAT;CUR;SPD;LNG;LAT;DTY;GYR");
    fileSD.close(); // On ferme immédiatement
    Serial.println("Fichier cree : " + path);
  } else {
    Serial.println("Impossible de creer le fichier !");
  }
}

void write_SD_card(){
  String data_SD = "";
  for(int i = 0; i < 11; i++){
    data_SD += String(DATA[i], 2) + ";"; // 2 decimales suffisent souvent
  }
  data_SD.remove(data_SD.length() - 1);

  // Utilisation de FILE_APPEND pour ne pas effacer le contenu existant
  fileSD = SD.open(path, FILE_APPEND); 
  if(fileSD){
    if(fileSD.println(data_SD)){
       // Optionnel : fileSD.flush(); // Force l'ecriture physique
    }
    fileSD.close();
  } else {
    Serial.println("Erreur d'ouverture lors de l'ecriture");
  }
}
#pragma endregion
