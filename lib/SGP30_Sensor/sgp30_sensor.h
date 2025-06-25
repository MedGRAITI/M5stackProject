/*      
Nom du fichier : sgp30_sensor.h
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur sgp30
Description : Fichier Header de capteur sgp30 Communication I2C, 
*/ 
#pragma once
#include <Arduino.h>
#include <stdint.h>

void initSGP30();
bool readSGP30(uint16_t& eco2, uint16_t& tvoc);
