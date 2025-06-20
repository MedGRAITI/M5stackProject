/*      
Nom du fichier : scd30_sensor.h
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur scd30
Description : Fichier Header de capteur scd30 Communication I2C, 
*/ 

#ifndef SCD30_SENSOR_H
#define SCD30_SENSOR_H

#include <Arduino.h>

void initSCD30();
bool readSCD30(float& co2, float& temperature, float& humidity);

#endif  // SCD30_SENSOR_H
