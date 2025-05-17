#define CAR //Flag für Precompiler (Adrian)

#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

#define CONNECT_NOTIFY 2
#define UART_LED 3

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);
Serial1.begin(115200, SERIAL_8N1);

pinMode(CONNECT_NOTIFY, INPUT); // High wenn BLE Verbindung besteht (Adrian)
pinMode(UART_LED, OUTPUT); // Leuchtet wenn BLE Verbindung besteht (Adrian)

t_exchange = millis();
t_debug = millis();
}

void loop() {

if (digitalRead(CONNECT_NOTIFY)) {
  digitalWrite(UART_LED, HIGH);
}
else {
  digitalWrite(UART_LED, LOW);
}


if (millis() >= t_exchange + 20) { // BLE und Serial Kommunikation (Adrian)
  Serial_val_exchange();
  t_exchange = millis();
  }

if (millis() >= t_debug + 500){ // Debug Loop
  //Serial.println("Bin im Main");
  Debug_data();
  t_debug = millis();
  }

}

void Serial_val_exchange() { // Variablen an MEGA Senden und Empfangen (Adrian)
  data_p = &data_buffer;      // Pointer vorbereiten
  byte_p = (uint8_t*)data_p; 
  //byte_p = byte_p + (sizeof(data)-1);

  if(Serial1.read() == '<') { // Auf start-byte checken

  for (int i = 0; i < sizeof(data); i++) { // Daten einlesen
    *byte_p = (byte)Serial1.read();
    byte_p++;
  }

  if (Serial1.read() == '>') {
    data = *data_p;
  /*
  uint8_t *debug_p = (uint8_t*)&data;
  //Debug Loop
  for (int i = 0; i < sizeof(data); i++) {
    Serial.print(*debug_p++, BIN);
  } 
  Serial.println();
  */
  }
  }
}