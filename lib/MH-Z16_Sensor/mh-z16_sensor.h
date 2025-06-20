/*      
Nom du fichier : mh-z16_sensor.h
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur MH Z16
Description : Fichier Header de capteur MH-Z16 Communication série (UART), deux trame de donnée envoyé pour la lecture de Co2,
*/ 

#ifndef MHZ16_SENSOR_H
#define MHZ16_SENSOR_H

#include <Arduino.h>
#include <HardwareSerial.h>

class Mhz16 {
  private:
    HardwareSerial& serial;
    uint8_t gasConcentration[9] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 }; // Trame pour lire la concentration CO2 
    uint8_t zeroPoint[9] = { 0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78 }; // Calibration de zero point

  public:
    Mhz16(HardwareSerial& s) : serial(s) {}
    void begin(unsigned long baud);
    int readCO2();
    void calibrateZeroPoint();
};

#endif