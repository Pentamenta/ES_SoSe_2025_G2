#define DASH //Flag für Precompiler (Adrian)

#define BLE_LED 4
#define MAX_SPEED 10 //m/s
#define MAX_STEAR 10 // Ausschlag zum Steuern

#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  // Custom Header mit BLE definitionen (Adrian)
#include "joystick.h" // Custom Header mit Joystick Funktionen (Adrian)
#include "Buttons.h"  // Custom Header für Button Anfrage (Adrian)

#include <U8g2lib.h> //Software I2C Display Ansteuerung (Eva)
#include <math.h>    //für Grafiken auf Displays (Eva)

#include "Tacho_Tempomat.h" //Einbindung der Funktionen/Variablen für mittleres Display: Tache & Tempomat (Eva)

unsigned long t_debug, t_fast, t_slow;

void setup() {
Serial.begin(9600);
delay(50);
t_debug, t_fast, t_slow = millis();

pinMode(BLE_LED, OUTPUT);

joystick_setup(); // Initialisiert Joysticks (Adrian)
Button_Setup();
display.begin();
BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
connect_car(); //Stellt Verbindung mit dem Auto her (Adrian)

}

void loop() {

  if (!car.connected()) { // Stellt Verbindung wieder her (Adrian)
    // Anzeige auf dem Bildschirm
    // Fahrzeug Steuerung resetten
    connect_car();
  }

  if (millis() >= t_fast + FAST_CYCLE) { // schnelle BLE und Serial Kommunikation (Adrian)
    data_to_car.stear_target_val = read_stear();
    data_to_car.speed_target_val = read_speed();
    BLE_fast_exchange();
    t_fast = millis();
  }

  if (millis() >= t_slow + SLOW_CYCLE) { // langsame BLE Kommunikation (Adrian)
    BLE_slow_exchange();
    t_slow = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    Serial.println("Bin im Main");
    Debug_data();
    //joystick_debug();
    t_debug = millis();
    }

}

float read_speed() { // Ließt linken Joystick für Beschleunigung aus (Adrian)

int buffer = read_joystick_pos(1);
float max = MAX_SPEED;
float dead = DEADZONE_LOWER;

float speed = (max/dead)*buffer;
return speed;

}

float read_stear() { // Ließt rechten Joystick für Lenkung aus (Adrian)

int buffer = read_joystick_pos(2);
float max = MAX_STEAR;
float dead = DEADZONE_LOWER;

float stear = (max/dead)*buffer;
return stear;

}