#define CAR_CONNECT //Flag für Precompiler (Adrian)

// Definitionen für Accelerometer (Adrian)
#define ACC_WAIT_TIME 500 // Zyklus zum lesen der Daten
#define MINIMUM_TILT 5    // Winkelgrenze für allarm (DEBUG zwecke)
// Definitionen für BLE Verbindung (Adrian)
#define CONNECT_NOTIFY 2 // High wenn BLE Verbindung besteht
#define BLE_LED 4 // LED für BLE Verbindung

#include <Arduino_BMI270_BMM150.h>
#include <ArduinoBLE.h>
#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)
#include "acc_data.h" // Custom Header mit Funktionen für Accelerometer

unsigned long t_debug, t_exchange, t_acc;

void setup() {
Serial.begin(9600);
Serial1.begin(115200, SERIAL_8N1);

t_debug, t_exchange, t_acc = millis();

pinMode(BLE_LED, OUTPUT);
pinMode(CONNECT_NOTIFY, OUTPUT);

Acc_Setup();

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

  if (millis() >= t_acc + ACC_WAIT_TIME) { // Auslesen der Neigungswinkel (Adrian)
    Acc_Read();
    t_acc = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    
    Serial.println("Bin im Main");
    Serial.println(data_to_car.angleX);
    Serial.println(data_to_car.angleY);
    //Serial.println(data_to_car.stear_target_val);
    //Serial.println(sizeof(data_to_car));
    //Debug_data();
    
    t_debug = millis();
    }

}