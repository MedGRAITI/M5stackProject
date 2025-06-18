/* #include "mh-z16_sensor.h"
#include <HardwareSerial.h>
#include <M5Core2.h>

HardwareSerial mhzSerial(2); // UART2

void initMHZ16() {
    mhzSerial.begin(9600, SERIAL_8N1, 16, 17);  // GPIO 16 (RX), 17 (TX)
    Serial.println("[MH-Z16] UART initialized");
}

void readMHZ16() {
  uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0, 0, 0, 0, 0, 0x79};
  mhzSerial.write(cmd, 9);
  delay(10);

  if (mhzSerial.available() >= 9) {
    uint8_t response[9];
    mhzSerial.readBytes(response, 9);

    if (response[0] == 0xFF && response[1] == 0x86) {
      int co2 = response[2] * 256 + response[3];
      Serial.printf("[MH-Z16] CO2: %d ppm\n", co2);
      M5.Lcd.printf("[MH-Z16]\nCO2: %d ppm\n\n", co2);
    } else {
      Serial.println("[MH-Z16] Invalid response");
      M5.Lcd.println("[MH-Z16] Invalid");
    }
  } else {
    Serial.println("[MH-Z16] No data received");
    M5.Lcd.println("[MH-Z16] No data");
  }
}
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
    return -1;  // Erreur
  }
}

void Mhz16::calibrateZeroPoint() {
  serial.write(zeroPoint, 9);
  serial.flush();
}