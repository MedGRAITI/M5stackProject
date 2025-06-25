/*      
Nom du fichier : scd40_sensor.h
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur scd40
Description : Fichier Header de capteur scd40 Communication I2C, 
*/ 

#pragma once
#include <Arduino.h>
#include <stdint.h>

void initSCD40();
bool readSCD40(uint16_t &co2, float& temp, float& humidity);
