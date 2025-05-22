#define CAR_CONNECT //Flag für Precompiler (Adrian)

#define CONNECT_NOTIFY 2 // High wenn BLE Verbindung besteht (Adrian)
#define BLE_LED 4 // LED für BLE Verbindung (Adrian)

#include <ArduinoBLE.h>
#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);
Serial1.begin(115200, SERIAL_8N1);

t_debug = millis();
t_exchange = millis();

pinMode(BLE_LED, OUTPUT);
pinMode(CONNECT_NOTIFY, OUTPUT);

digitalWrite(CONNECT_NOTIFY, LOW);
BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Peripherial Device (Adrian)
Dash_connect(); //Verbindung mit dem Dashboard herstellen (Adrian)

}

void loop() {

  if (!dashboard.connected()) { // try to reconnect if connection lost
    // Alert car that connection was lost
    digitalWrite(CONNECT_NOTIFY, LOW);
    Dash_connect();
  }

  if (millis() >= t_exchange + 20) { // BLE und Serial Kommunikation (Adrian)

    BLE_val_exchange();
    //data.stear_target_val = 11;
    Serial_val_exchange();

    t_exchange = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    
    Serial.println("Bin im Main");
    //Serial.println(data_to_car.stear_target_val);
    Serial.println(sizeof(data_to_car));
    Debug_data();
    
    t_debug = millis();
    }

}