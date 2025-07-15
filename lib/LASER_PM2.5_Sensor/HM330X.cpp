#include "HM330X.h"

int HM330X::init() {
    Wire.begin();
    return 0;
}

int HM330X::read_sensor_value(uint8_t* data, int len) {
    if (!data || len < HM330X_FRAME_LENGTH) return -1;

    Wire.beginTransmission(HM330X_I2C_ADDR);
    Wire.write(0x88); // commande pour lecture continue
    if (Wire.endTransmission() != 0) return -2;

    delay(10); // attendre les données

    Wire.requestFrom(HM330X_I2C_ADDR, HM330X_FRAME_LENGTH);
    if (Wire.available() < HM330X_FRAME_LENGTH) return -3;

    for (int i = 0; i < HM330X_FRAME_LENGTH; i++) {
        data[i] = Wire.read();
    }

    return 0;
}
