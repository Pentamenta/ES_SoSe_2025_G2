#define CAR_CONNECT //Flag für Precompiler (Adrian)

// Definitionen für Accelerometer (Adrian)
#define ACC_WAIT_TIME 500 // Zyklus zum lesen der Daten
#define MINIMUM_TILT 5    // Winkelgrenze für allarm (DEBUG zwecke)
// Definitionen für BLE Verbindung (Adrian)
#define CONNECT_NOTIFY 2 // High wenn BLE Verbindung besteht
#define BLE_LED 4 // LED für BLE Verbindung

#include <Arduino_BMI270_BMM150.h>
#include <ArduinoBLE.h>
//#include <Arduino_HTS221.h>
#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)
#include "acc_data.h" // Custom Header mit Funktionen für Accelerometer

unsigned long t_debug, t_fast, t_slow, t_acc;

void setup() {
Serial.begin(9600);
Serial1.begin(9600, SERIAL_8N1);

t_debug, t_fast, t_slow, t_acc = millis();

pinMode(BLE_LED, OUTPUT);
pinMode(CONNECT_NOTIFY, OUTPUT);

Acc_Setup();

digitalWrite(CONNECT_NOTIFY, LOW);
BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Peripherial Device (Adrian)
data_to_car.temperature_val = 24; // Debug da Temperatursensor nicht vorhanden
}

void loop() {

  BLE.poll();

  if (millis() >= t_fast + FAST_CYCLE) { // schnelle BLE und Serial Kommunikation (Adrian)
    BLE_fast_exchange();
    Serial_val_exchange();
    t_fast = millis();
  }

  if (millis() >= t_slow + SLOW_CYCLE) { // langsame BLE Kommunikation (Adrian)
    BLE_slow_exchange();
    t_slow = millis();
  }

  if (millis() >= t_acc + ACC_WAIT_TIME) { // Auslesen der Neigungswinkel (Adrian)
    Acc_Read();
    t_acc = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    
    Serial.println("Bin im Main");

    if (dashboard.connected()) {
      //Debug_data();
      //Serial.print("To Car: ");
      //Serial.println(sizeof(data_to_car));
      //Serial.print("To Dash: ");
      //Serial.println(sizeof(data_to_dash));
    }
    else {
      // Alert car that connection was lost
      digitalWrite(CONNECT_NOTIFY, LOW);

      digitalWrite(BLE_LED, !digitalRead(BLE_LED));
    }

    t_debug = millis();
    }

}