#define DASH //Flag für Precompiler (Adrian)

#define BLE_LED 4

#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  //Custom Header mit BLE definitionen (Adrian)

#include <U8g2lib.h> //Software I2C Display Ansteuerung (Eva)
#include <math.h>    //für Grafiken auf Displays (Eva)

#include "Tacho_Tempomat.h" //Einbindung der Funktionen/Variablen für mittleres Display: Tacho & Tempomat (Eva)

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);
delay(50);
t_debug = millis();
t_exchange = millis();

pinMode(BLE_LED, OUTPUT);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
connect_car(); //Stellt Verbindung mit dem Auto her (Adrian)
mdisplay.begin();
}

void loop() {

  if (!car.connected()) { // Stellt Verbindung wieder her (Adrian)

    // Anzeige auf dem Bildschirm
    // Fahrzeug Steuerung resetten
    connect_car();
  }

  if (millis() >= t_exchange + 20) {
    data_to_car.speed_target_val = random(1,100);
    data_to_car.stear_target_val = random(1,100);
    BLE_val_exchange();
    t_exchange = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    Serial.println("Bin im Main");
    t_debug = millis();
  }

  writeTacho(100);
}

