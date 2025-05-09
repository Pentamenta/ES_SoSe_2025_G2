#define CAR //Flag für Precompiler (Adrian)


unsigned long t_exchange;

void setup() {
Serial.begin(9600);
Serial1.begin(115200);

}

void loop() {

  if (millis() >= t_exchange + 20) { // BLE und Serial Kommunikation (Adrian)


    t_exchange = millis();
  }


}

