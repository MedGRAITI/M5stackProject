/*      
Nom du fichier : mh-z16_sensor.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur MH Z16
Description : Fichier cpp de capteur MH-Z16 Communication série (UART), détaille les fonctions utilisées,
*/ 

#include "mh-z16_sensor.h"

 void Mhz16::begin(unsigned long baud) {
  serial.begin(baud, SERIAL_8N1);
}

int Mhz16::readCO2() {
  uint8_t response[9] = {0};
  serial.write(gasConcentration, 9);
  serial.flush();
  delay(100);

  int i = 0;
  while (serial.available() && i < 9) {
    response[i++] = serial.read();
  }

  if (response[0] == 0xFF && response[1] == 0x86) {
    return response[2] * 256 + response[3];
  } else {
    return -1;  // Erreur de la lecture 
  }
}

void Mhz16::calibrateZeroPoint() {
  serial.write(zeroPoint, 9);
  serial.flush();
} 

/* int Mhz16::readCO2() {
  uint8_t response[9] = {0};
  serial.write(gasConcentration, 9);
  serial.flush();

  unsigned long start = millis();
  while (serial.available() < 9 && millis() - start < 300) {
    delay(10);
  }

  if (serial.available() >= 9) {
    for (int i = 0; i < 9; ++i) {
      response[i] = serial.read();
    }
    if (response[0] == 0xFF && response[1] == 0x86) {
      return response[2] * 256 + response[3];
    }
  }
  return -1;
}

void Mhz16::calibrateZeroPoint() {
  serial.write(zeroPoint, 9);
  serial.flush();
} */