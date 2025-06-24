#define DASH //Flag für Precompiler (Adrian)

#define BLE_LED 4
#define MAX_SPEED 10  // m/s
#define MAX_STEAR 100 // Ausschlag zum Steuern

// Tempomat
float tempo_speed = 0;      // Tempomat Geschwindigkeit
bool tempo_on = false;  // Ist der Tempomat an?

#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  // Custom Header mit BLE definitionen (Adrian)
#include "Joystick.h" // Custom Header mit Joystick Funktionen (Adrian)
#include "Buttons.h"  // Custom Header für Button Anfrage (Adrian)
#include "Leds.h"
#include <U8g2lib.h> //Software I2C Display Ansteuerung (Eva)
#include <math.h>    //für Grafiken auf Displays (Eva)
#include "Tacho_Tempomat.h" //Einbindung der Funktionen/Variablen für mittleres Display: Tache & Tempomat (Eva)

unsigned long t_debug, t_fast, t_slow;

void setup() {
Serial.begin(9600);
delay(50);
t_debug, t_fast, t_slow = millis();

pinMode(BLE_LED, OUTPUT);
Led_Setup();
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
    BLE_fast_exchange();
    t_fast = millis();
  }

// Funktionen Tasterabfrage + LEDs
Led_Update();
speed_control();
button_eval();


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
return -speed;

}

float read_stear() { // Ließt rechten Joystick für Lenkung aus (Adrian)

int buffer = read_joystick_pos(2);
float max = MAX_STEAR;
float dead = DEADZONE_LOWER;

float stear = (max/dead)*buffer;
return stear;

}

void speed_control() {

  if (tempo_on && (read_speed() <= tempo_speed) && (read_speed() >= 0)) {
    data_to_car.speed_target_val = tempo_speed;
  }
  else if (tempo_on && (read_speed() > tempo_speed) && (read_speed() >= 0)) { // Tempomat ist an und es wird beschleunigt
    data_to_car.speed_target_val = read_speed();
  }
  else if (tempo_on && (read_speed() < 0)) { // Beim bremsen geht der Tempomat aus
    tempo_on = false;
    data_to_car.speed_target_val = read_speed();
  }
  else { // Tempomat ist aus
      data_to_car.speed_target_val = read_speed();
  }

}

void button_eval() {

  // Button 1 - Tempomat on
  if (t1 && !tempo_on) {  // Einschalten
    tempo_speed = data_to_dash.speed_actual_val; // Tempomat Geschwindigkeit auf aktuellen Wert setzen
    tempo_on = true;      // Tempomat einschalten
    t1 = false;           // Button resetten
  }
  else if (t1 && tempo_on) {
    tempo_on = false;     // Tempomat auschalten
    t1 = false;           // Button resetten
  }

  // Button 2 - Tempomat set
  if (t2 && !tempo_on) {  // Einschalten
    if (tempo_speed == 0) {
      // Tempomat Geschwindigkeit auf aktuellen Wert setzen, falls noch nicht gesetzt
      tempo_speed = data_to_dash.speed_actual_val; 
    }
    tempo_on = true;      // Tempomat einschalten
    t2 = false;           // Button resetten
  }
  else if (t2 && tempo_on) { // Tempomat auf aktuellen speed setzen
    tempo_speed = tempo_speed = data_to_dash.speed_actual_val;
    t2 = false;           // Button resetten
  }

  // Button 3 - Licht
  if (t3) {
    // Scheinwerfer toggeln
    boolean_to_car_arr[0][3] = !boolean_to_car_arr[0][3];
  }

  // Button 4 - Fernlicht
    if (t3 && boolean_to_dash_arr[0][3]) {
    // Fernlicht toggeln
    boolean_to_car_arr[0][4] = !boolean_to_car_arr[0][4];
  }

  // Joystick Buttons - Blinker
  if (jl_sw && jr_sw) { // Beide Taster Gleichzeitig --> Warnblinker
    boolean_to_car_arr[0][2] = !boolean_to_car_arr[0][2];
    jl_sw, jr_sw = false;
  }
  else if (jl_sw && !jr_sw) { // Blinker links
    boolean_to_car_arr[0][1] = !boolean_to_car_arr[0][1];
    jl_sw = false;
  }
  else if (!jl_sw && jr_sw) { // Blinker rechts
    boolean_to_car_arr[0][0] = !boolean_to_car_arr[0][0];
    jr_sw = false;
  }
}


