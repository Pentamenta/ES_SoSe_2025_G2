#define CAR //Flag für Precompiler (Adrian)

#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

#define CONNECT_NOTIFY 4
#define UART_LED 5

#include <Servo.h>  //Servo Motor Library (jan)
#define MAX_SPEED 10

// TAMO {
enum BlinkerStatus {BLINKER_NONE, BLINKER_LEFT, BLINKER_RIGHT};
BlinkerStatus lastBlinkerState = BLINKER_NONE;
BlinkerStatus currentBlinkerState = BLINKER_NONE;

#include <Servo.h>

Servo wiper;

// === Pin-Zuweisungen ===
const int joystickX = A0;    // X-Achse des Joysticks
const int joystickY = A3;    
const int leftLED = 24;       // Blinker links
const int rightLED = 22;      // Blinker rechts
const int piezoPin = 8;       // Piezo-Buzzer
const int lightSensor = A1;             // Fotowiderstand
const int Frontlicht = 30;
const int brakeLight = 32;
const int backLight = 34;    // Rückfahrlicht
const int Fernlicht = 13;
const int OnOff = 25;            // Ablendlicht
const int Warn = 23;                     // Warnblinker
const int braking = 33;          // Bremse

const int wiperPin = 44;                // Wiper
const int Wish = 31;

const int comfortBlinkerBtn = A3; // Taster für Komfortblinker

const int PIN_NTC = A2;        // Temperatur-Sensor
const int PIN_HOT = 29;         // Ausgang für Lüfter (z. B. LED oder
const int PIN_COLD = 27;          // Frostwarnung (LED oder Piezo)


// === Schwellwerte ===
const int thresholdLeft = 200;
const int thresholdRight = 800;
const int blinkDelay = 400;  // Blinkfrequenz in ms
const int toneFrequency = 440; // Tonhöhe des "Tick"
const int toneDuration = 100;  // Dauer des Tons in ms
const float R_1 = 1000.0;  // Widerstand vor Fotosensor

static unsigned long lightOffTime = 0;
static bool nachleuchten = false;

bool comfortLeftActive = false;
bool comfortRightActive = false;
int LcomfortCount = 0;
int RcomfortCount = 0;
int comfortCount = 0;
unsigned long comfortPreviousMillis = 0;

bool leftBlinkState = false;
bool rightBlinkState = false;

bool lightOn = false;
bool warningActive = false;
bool Blink = false;

//float speed_actual_val = 0;

unsigned long previousMillis = 0; //test1
unsigned long cpreviousMillis = 0; //test2

unsigned long wiperMillis = 0; //test2
bool Wip = false;
bool wishi = false;

const int linksAngle = 5;
const int rechtsAngle = 175;

unsigned long bremsPreviousMillis = 0;

int yValue;
int bValue;

const float R_REF = 10000.0;
const float T0_K = 273.15 + 25;
const float BETA = 3435;
const float R0 = 10000.0;
// } TAMO 

//Motorsteuerung (Jan)
const int ServoAnschluss = 9;
const int ENA = 10;
const int IN1 = 11;
const int IN2 = 12;
const int MIN_PWM = 50;
float speed_target_val;
int stear_target_val;
int MotorGrad = 90;
int pwmVal;
float speed_buffer_val;
float avgSpeed = 0.0;
int speed_buffer_pwm = 0;
float speed_reg;

//Geschwindigkeitsmessung (Jan)
const byte sensorDigitalPin = 2;
volatile unsigned int pulseCount = 0;
unsigned long lastMeasurementTime = 0;
const unsigned long interval = 1000;
const float wheelCircumference = 0.305;  // Meter
float rpmValues[3] = {0, 0, 0};
float speedValues[3] = {0, 0, 0};
int index = 0;

Servo srv;

unsigned long t_debug;
unsigned long t_exchange;
//Test Jan
void setup() {
Serial.begin(9600);
Serial1.begin(9600, SERIAL_8N1);

pinMode(CONNECT_NOTIFY, INPUT); // High wenn BLE Verbindung besteht (Adrian)
pinMode(UART_LED, OUTPUT); // Leuchtet wenn BLE Verbindung besteht (Adrian)

t_exchange = millis();
t_debug = millis();

//DC Motor und Servo Setup (jan)
srv.attach(ServoAnschluss, 500, 2500);
srv.write(MotorGrad);
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);

//Geschwindigkeitsmesser Setup
pinMode(sensorDigitalPin, INPUT);
attachInterrupt(digitalPinToInterrupt(sensorDigitalPin), countPulse, FALLING);

// TAMO { 
  // — Analoge Sensoren (Fotowiderstand, Joystick, TMP36) —
   pinMode(joystickX,     INPUT);        // A0
   pinMode(joystickY,     INPUT);        // A3
   pinMode(lightSensor,   INPUT);        // A1
   pinMode(PIN_NTC,    INPUT);        // A2

   // — Digitale Ausgänge (LEDs, Piezo, Motoren) —
   pinMode(leftLED,       OUTPUT);       // D4
   pinMode(rightLED,      OUTPUT);       // D2
   pinMode(piezoPin,      OUTPUT);       // D8
   pinMode(Frontlicht,    OUTPUT);       // D10 (PWM)
   pinMode(brakeLight,    OUTPUT);       // D12
   pinMode(Fernlicht,     OUTPUT);       // D13
   pinMode(wiperPin,    OUTPUT);       // D11 (PWM)
   pinMode(PIN_HOT,    OUTPUT);       
   pinMode(PIN_COLD,      OUTPUT);      

   // — Digitale Eingänge mit internem Pull-Up (Buttons/Taster) —
   pinMode(OnOff,               INPUT_PULLUP); // D7  Licht-Taster
   pinMode(Warn,                INPUT_PULLUP); // D6  Warnblinker-Taster
   pinMode(braking,             INPUT_PULLUP); // D5  Bremspedal-Taster
   pinMode(Wish,         INPUT_PULLUP); // D31  Wischer-Taster
   pinMode(comfortBlinkerBtn,   INPUT_PULLUP); // A3 Komfortblinker-Taster

   wiper.attach(wiperPin);
  // } TAMO 

}

void loop() {
  unpack_bool();
  package_bool();
  BLE_fast_exchange();

//jan 
handleSteering();   // Servo Motor
handleDrive();      // DC Motor
handleRPMandSpeed(); //RPM Messung
buffer();
if (digitalRead(CONNECT_NOTIFY)) {
  digitalWrite(UART_LED, HIGH);
}
else {
  digitalWrite(UART_LED, LOW);
}


if (millis() >= t_exchange + 20) { // BLE und Serial Kommunikation (Adrian)
  package_bool();
  Serial_val_exchange();
  unpack_bool();
  t_exchange = millis();
  }

if (millis() >= t_debug + 500){ // Debug Loop
  Serial.println("Bin im Main");

  Debug_data();
  Serial.print("To Car: ");
  Serial.println(sizeof(data_to_car));
  Serial.print("To Dash: ");
  Serial.println(sizeof(data_to_dash));

  t_debug = millis();
  }

// TAMO {
  unpack_bool();
  package_bool();
  BLE_fast_exchange();
  unsigned long currentMillis = millis(); //test1
  unsigned long comfortMillis = millis(); //test1

  int xValue = analogRead(joystickX);
  int yValue = analogRead(joystickY);
  
  

  int y = analogRead(lightSensor); // ADC = y
  float U_light = y * (5.0 / 1023.0);
  float R_light = U_light / (5.0 - U_light) * R_1;

  int ledBrightness = map(y, 0, 1023, 0, 255); // Umrechnen!

  // === Temperaturmessung über TMP36 ===
  //int sensorValue = analogRead(tempSensor);
  //float voltage = sensorValue * (5.0 / 1023.0); // Spannung in Volt
  //float temperatureC = (voltage - 0.5) * 100.0; // Temperatur in °C

    
  // === Temperaturmessung über NTC ===
  int raw = analogRead(PIN_NTC);  // 0–1023
  float Vout = raw * (5.0 / 1023.0);
  // Berechne NTC-Widerstand: Rntc = R_ref * Vout / (Vcc - Vout)
  float Rntc = R_REF * Vout / (5.0 - Vout);
  
  // 1/T = 1/T0 + (1/BETA) * ln(Rntc/R0)
  float invT = (1.0 / T0_K) + (1.0 / BETA) * log(Rntc / R0);
  float T_K = 1.0 / invT;         // Temperatur in Kelvin
  float T_Craw = T_K - 273.15;      // Temperatur in °C
  float T_C = T_Craw + 16;      // Fehelrkorektur

  Serial.println(T_C);
  Serial.println(digitalRead(PIN_HOT));

  data_to_dash.temperature_val = T_C;


  // === Klimaentlüftung einschalten bei > 25 °C ===
  if (T_C > 27.0) {
    digitalWrite(PIN_HOT, HIGH);
  } else {
    digitalWrite(PIN_HOT, LOW);
  }

  // === Frostwarner aktivieren bei < 4 °C ===
  if (T_C < 4.0) {
    digitalWrite(PIN_COLD, HIGH);
    //tone(8, 1000, 200); // Piezo auf Pin 8 (wenn angeschlossen)
  } else {
    digitalWrite(PIN_COLD, LOW);
  }


  

  // Bremslicht (yValue < 500) && (yValue > 150)
  if (boolean_to_car_arr[0][5] || digitalRead(braking)) {
    analogWrite(brakeLight, 255); // sehr helles Bremslicht
    boolean_to_dash_arr[0][5] = true;
  } else {
    analogWrite(brakeLight, 0);
    boolean_to_dash_arr[0][5] = true;
  }

  // bValue = yValue;

  // Rückfahrtlicht
  if (data_to_car.speed_target_val < 0 || yValue < 250) {
    analogWrite(backLight, 255);
  } else {
    analogWrite(backLight, 0);
  }

  // Warnblinker taster
  if (digitalRead(Warn)  == true) {
    boolean_to_car_arr[0][2] = !boolean_to_car_arr[0][2];
    digitalWrite(leftLED, false);
    digitalWrite(rightLED, false);
    delay(100);
  }
  // Warnblinker
  if (boolean_to_car_arr[0][2]) {
    boolean_to_dash_arr[0][2] = true;
    if (currentMillis - previousMillis >= blinkDelay) {
      previousMillis = currentMillis;
        Blink = !Blink;
      digitalWrite(leftLED, Blink);
      digitalWrite(rightLED, Blink);
      if (Blink) {
        tone(piezoPin, toneFrequency, toneDuration);
      }
    }
  } else {
  boolean_to_dash_arr[0][2] = false;
  }

  // Ablendlicht Taster
  if (digitalRead(OnOff) == true) {
    boolean_to_car_arr[0][3] = !boolean_to_car_arr[0][3];
    delay(100);
    if (!boolean_to_car_arr[0][3]) {
      nachleuchten = true;
        lightOffTime = millis();
      }
  }
  // Ablendlicht
  if (boolean_to_car_arr[0][3] == true) {
    analogWrite(Frontlicht, ledBrightness); // Helligkeit steuern
    boolean_to_dash_arr[0][3] = true;

      // Fernlicht
      if (y > 900) {
      analogWrite(Fernlicht, 255);
      boolean_to_dash_arr[0][4] = true;
      } else {
      analogWrite(Fernlicht, 0);
      boolean_to_dash_arr[0][4] = false;
      }

    lightOffTime = millis();
  } else {
    if (millis() - lightOffTime < 5000) {  //Wegbeleuchtung
    analogWrite(Frontlicht, ledBrightness);
    } else {
    analogWrite(Frontlicht, 0); // Helligkeit aus
    analogWrite(Fernlicht, 0);
    boolean_to_dash_arr[0][3] = false;
    }
  }

  // Komfortblinker
  if (boolean_to_car_arr[0][1] || xValue < thresholdLeft) {
    currentBlinkerState = BLINKER_LEFT;
  } else if (boolean_to_car_arr[0][0] || xValue > thresholdRight) {
    currentBlinkerState = BLINKER_RIGHT;
  } else {
    currentBlinkerState = BLINKER_NONE;
  }

  if (currentBlinkerState != lastBlinkerState) {
    if (lastBlinkerState == BLINKER_LEFT) LcomfortCount = 6;
    else if (lastBlinkerState == BLINKER_RIGHT) RcomfortCount = 6;
    lastBlinkerState = currentBlinkerState;
  }

  if (comfortMillis - cpreviousMillis >= blinkDelay) {
    cpreviousMillis = currentMillis;
    RcomfortCount -= 1;
    LcomfortCount -= 1;

  }

  // Blinker links
  if (boolean_to_car_arr[0][1] || (LcomfortCount > 0) || xValue < thresholdLeft) {
    digitalWrite(rightLED, false);
    boolean_to_dash_arr[0][1] = true;
    if (currentMillis - previousMillis >= blinkDelay) {
      previousMillis = currentMillis;
        Blink = !Blink;
      digitalWrite(leftLED, Blink);
      if (Blink == true) {
        tone(piezoPin, toneFrequency, toneDuration);
      }
    }
    boolean_to_dash_arr[0][0] = false;
  }

  // Blinker rechts
  else if (boolean_to_car_arr[0][0] || (RcomfortCount > 0) || xValue > thresholdRight) {
    digitalWrite(leftLED, false);
    boolean_to_dash_arr[0][0] = true;
    if (currentMillis - previousMillis >= blinkDelay) {
      previousMillis = currentMillis;
        Blink = !Blink;
      digitalWrite(rightLED, Blink);
      if (Blink == true) {
        tone(piezoPin, toneFrequency, toneDuration);
      }
    }
    boolean_to_dash_arr[0][1] = false;
  }
  // Zentriert → alles aus
  else if (boolean_to_car_arr[0][2] == false) {
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, LOW);
    noTone(piezoPin); // Sicherheitshalber abschalten
    //Serial.println(comfortCount);
    boolean_to_dash_arr[0][0] = false;
    boolean_to_dash_arr[0][1] = false;
  }
  
  if (digitalRead(Wish) == true) {
    boolean_to_car_arr[1][1] = !boolean_to_car_arr[1][1];
    delay(100);
  }

  if(boolean_to_car_arr[1][1]){
    boolean_to_dash_arr[1][1] = true;
    if (currentMillis - wiperMillis >= 1000) {
      wiperMillis = currentMillis;
          Wip = !Wip;
      if (Wip == true) {
        wiper.write(rechtsAngle);
      } else {
        wiper.write(linksAngle);
      }
    }
  } else {
    boolean_to_dash_arr[1][1] = false;
  }
  // delay(100);
  // } TAMO
  
}


void countPulse() {
  pulseCount++;
}

//buffer
void buffer() {
  speed_buffer_val = data_to_car.speed_target_val;
  float error = speed_buffer_val - avgSpeed;
  const float KP = 100.0;
  speed_buffer_pwm = constrain(error * KP, -255, 255);
}

//jan
void handleSteering() {
  int xVal = data_to_car.stear_target_val;

  if (xVal > -5 && xVal < 5) {
    MotorGrad = 90; // Zentrum
  } else if (xVal <= -5) {
    MotorGrad = map(xVal, -5, -100, 90, 40); // Links
  } else if (xVal >= 5) {
    MotorGrad = map(xVal, 5, 100, 90, 140); // Rechts
  }
  srv.write(MotorGrad);
  
  data_to_dash.stear_actual_val = xVal;
}

//jan
void handleDrive() {

  int pwmVal = speed_buffer_pwm; 

  if (speed_buffer_pwm > 0.0) {
    // Vorwärts
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, max(pwmVal, MIN_PWM));
  }
  else if (speed_buffer_pwm < 0.0) {
    // Rückwärts
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, max(-pwmVal, MIN_PWM));
  }
  else {
    // Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}

//jan
void handleRPMandSpeed() {
  unsigned long currentTime = millis();

  if (currentTime - lastMeasurementTime >= interval) {
    detachInterrupt(digitalPinToInterrupt(sensorDigitalPin));

    float currentRPM = pulseCount * 60.0;

    // Direkt in m/s berechnen
    float currentSpeed = (pulseCount * wheelCircumference) / (interval / 1000.0);  // m/s

    // Alternativ mit RPM (zur besseren Lesbarkeit):
    // float currentSpeed = (currentRPM / 60.0) * wheelCircumference;

    rpmValues[index] = currentRPM;
    speedValues[index] = currentSpeed;
    index = (index + 1) % 3;

    float avgRPM = (rpmValues[0] + rpmValues[1] + rpmValues[2]) / 3.0;
    float avgSpeed = (speedValues[0] + speedValues[1] + speedValues[2]) / 3.0;

    pulseCount = 0;
    lastMeasurementTime = currentTime;

    attachInterrupt(digitalPinToInterrupt(sensorDigitalPin), countPulse, FALLING);

    data_to_dash.speed_actual_val = avgSpeed;  // Jetzt direkt in m/s
  }
}

