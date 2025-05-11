#define CAR //Flag für Precompiler (Adrian)

#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);
Serial1.begin(115200);

t_exchange = millis();
t_debug = millis();
}

void loop() {

  if (millis() >= t_exchange + 20) { // BLE und Serial Kommunikation (Adrian)
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
Serial.println(Serial1.available());
if (Serial1.available()){
  for (int i = 0; i < sizeof(data); i++) {
    byte_p = Serial1.read();
    Serial.print(*byte_p, HEX);
    byte_p++;
  }
  Serial.println();
  byte_p = byte_p - sizeof(data);

  data_p = (struct exchange_data*)byte_p;
  data = *data_p;
}
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