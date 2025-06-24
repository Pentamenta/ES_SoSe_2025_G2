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


unsigned long t_debug, t_fast, t_slow;

// Einbinden der Bibliotheken für I2C-Kommunikation und Display-Ansteuerung
#include <Wire.h>                   // Für I2C-Kommunikation
#include <Adafruit_GFX.h>           // Grundlegende Grafikfunktionen (Text, Linien etc.)
#include <Adafruit_SSD1306.h>       // Treiber für SSD1306 OLED-Display

// Konstante Maße des Displays
const int SCREEN_WIDTH = 128;       // Breite in Pixel
const int SCREEN_HEIGHT = 64;       // Höhe in Pixel

// Erstellen des Displayobjekts über I2C-Verbindung
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Software I2C Pininitialisierung (Eva) für linkes Display, R3 für die Displaydrehung
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C ldisplay(U8G2_R3, /* clock=*/ 8, /* data=*/ 9, /* reset=*/ U8X8_PIN_NONE);

#include "Tacho.h" //Einbindung der Funktionen/Variablen für mittleres Display: Tache & Tempomat (Shari)
#include "Abstandsmessung.h" //Einbindung der Funktionen/Variablen zur Abstandsmessung (Shari)
//#include "leftDisplay.h" //Einbindung der Funktionen/Variablen für linkes Display: Tempomat (Eva)

unsigned long t_exchange;

// Variablen zur Abstandsmessung
void setup() {
Serial.begin(9600);
while(!Serial); 
t_debug, t_fast, t_slow = millis();

pinMode(BLE_LED, OUTPUT);
Led_Setup();
joystick_setup(); // Initialisiert Joysticks (Adrian)
Button_Setup();
ldisplay.begin();

// Ausgabe des Dysplays 3D über I2C (Shari)
 if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed")); // Fehlerausgabe, wenn das Dysplay nicht erkannt wird
    while (true); // Endlosschleife bei Fehler
  }
  abstand(data_to_dash.distance_f_val, data_to_dash.distance_l_f_val, data_to_dash.distance_r_f_val, data_to_dash.distance_b_val, data_to_dash.distance_l_b_val, data_to_dash.distance_r_b_val, boolean_to_dash_arr[0][9], boolean_to_dash_arr[0][8], boolean_to_dash_arr[0][3], boolean_to_dash_arr[0][4], boolean_to_dash_arr[0][7], boolean_to_dash_arr[0][6]);

//Ausgabe der Dysplays 3C über I2C (Shari)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed")); // Fehlerausgabe, wenn das Dysplay nicht erkannt wird
    while (true); // Endlosschleife bei Fehler;
  }
 tachorech(); // Skalenpunkte vorberechnen

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
    writeTacho(data_to_dash.speed_actual_val, tempo_speed, tempo_on, data_to_dash.temperature_val, angleY); 
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

  writedispl(speed_actual_val);

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



