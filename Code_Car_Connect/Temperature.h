#ifndef TEMPERATURE
#define TEMPERATURE

#include <Arduino_HTS221.h>

#define TEMP_CYCLE 2000

float old_temp = 0;

void HTS_Setup() {

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }

}


float temp_read() {

    float temperature = HTS.readTemperature();

    if (abs(old_temp - temperature) >= 0.5) {
        old_temp = temperature;

        return temperature;
    }
    else {
        return old_temp;
    }

}

#endif