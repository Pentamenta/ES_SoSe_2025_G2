#define CAR //Flag für Precompiler (Adrian)

#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);
Serial1.begin(115200, SERIAL_8N1);

t_exchange = millis();
t_debug = millis();
}

void loop() {

  if (millis() >= t_exchange + 80) { // BLE und Serial Kommunikation (Adrian)
    Serial_val_exchange();

    t_exchange = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    Serial.println("Bin im Main");
    Debug_data();

    t_debug = millis();
    }

}

void Serial_val_exchange() { // Variablen an MEGA Senden und Empfangen (Adrian)
  data_p = &data;
  byte_p = (uint8_t*)data_p;
  byte_p = byte_p + sizeof(data);
  for (int i = 0; i < sizeof(data); i++) {
    *byte_p = (byte)Serial1.read();
    byte_p--;
  }
  data = *data_p;

}