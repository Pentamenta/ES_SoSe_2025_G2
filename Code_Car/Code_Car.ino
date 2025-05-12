#define CAR //Flag für Precompiler (Adrian)

#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);


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

Serial.print(Serial1.available());
Serial.print("  ");
Serial.println(sizeof(data));


if (Serial1.available() == sizeof(data)){
  data_p = &data;
  byte_p = (uint8_t*)data_p;
  for (int i = 0; i < sizeof(data); i++) {
    *byte_p = (byte)Serial1.read();
    byte_p++;
  }
}
Serial1.end();
Serial1.begin(115200, SERIAL_8N1);
}

void Debug_data() { // Debug Ausgabe des Data Structs (Adrian)

  Serial.println("Einmal der Data Struct:");
  Serial.print("Speed_target: ");
  Serial.println(data.speed_target_val);

  Serial.print("Speed_actual: ");
  Serial.println(data.speed_actual_val);

  Serial.print("Stear_target: ");
  Serial.println(data.stear_target_val);

  Serial.print("Stear_actual: ");
  Serial.println(data.stear_actual_val);

}