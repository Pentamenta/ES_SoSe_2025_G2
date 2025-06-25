#define CAR //Flag für Precompiler (Adrian)

#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

#define CONNECT_NOTIFY 4
#define UART_LED 5

#include <Servo.h>  //Servo Motor Library (jan)
#define MAX_SPEED 10

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

//JJ
//aktive und passive Fahrsicherheit 

float speed = data_to_dash.speed_actual_val;       
float dps_Z = data_to_car.dps_Z_val;     
float acc_Y = data_to_car.acc_Y_val;     
float acc_Z = data_to_car.acc_Z_val;   
float angleX = data_to_car.angleX;
float angleY = data_to_car.angleY;
int16_t distance_f = data_to_dash.distance_f_val;
//Schwellwerte zur Entscheidung der Auslösechrekteristik 
const float schwelle_fahrzeug_ABS   = 0.5f;    
const float schwelle_fahrzeug_ASR   = 0.2f;    
const float schwelle_drehzahl_ASR   = 50.0f;   
const float schwelle_esp            = 30.0f;  
const float schwelle_diebstahl      = 1.5f;
const float schwelle_crash          = -0.6f;   
const float schwelle_angle_x	      = 45.0f;
const float schwelle_angle_y	      = 45.0f; 
//JJ
//Ergebnis-Flags
bool abs_active        = false;
bool asr_active        = false;
bool esp_active        = false;
bool diebstahl_active   = false;
bool airbag_active     = false;
bool ueberschlag_active = false;
bool abstand_active     = false;  


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
}

void loop() {
//jan 
handleSteering();   // Servo Motor
handleDrive();      // DC Motor
handleRPMandSpeed(); //RPM Messung
buffer();

//JJ
abs_active        = checkABS(pulseCount ,  speed, schwelle_fahrzeug_ABS);
asr_active        = checkASR(speed, pulseCount, schwelle_drehzahl_ASR, schwelle_fahrzeug_ASR);
esp_active        = checkESP( dps_Z, schwelle_esp);
diebstahl_active   = checkDiebstahl( acc_Z,schwelle_diebstahl);
airbag_active      = checkAirbag(acc_Y, schwelle_crash );
ueberschlag_active = checkAbstand(distance_f,speed);
abstand_active     = checkAbstand(distance_f,speed); 

boolean_to_dash_arr[0][10]= abs_active;
boolean_to_dash_arr[0][11]= ueberschlag_active;
boolean_to_dash_arr[0][12]= diebstahl_active;
boolean_to_dash_arr[0][13]= airbag_active;
boolean_to_dash_arr[0][14]= esp_active;
boolean_to_dash_arr[0][15]= asr_active;
boolean_to_dash_arr[1][0]= abstand_active;


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
  int xVal;
  if(digitalRead(CONNECT_NOTIFY)){
  xVal = data_to_car.stear_target_val;
  }
  else{ xVal = 0;
  }

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
  
  if(digitalRead(CONNECT_NOTIFY)){
  int pwmVal = speed_buffer_pwm;
  }
  else{int pwmVal = 0;}

   

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


//JJ aktive passive sicher heit -> bool Fuktionen

// ABS: Rad blockiert, wenn Fahrzeug schnell genug und Raddrehzahl = 0
bool checkABS(volatile unsigned int pulseCount , float speed, float schwelle_fahrzeug_ABS) 

  {
    float currentRPM = pulseCount * 60.0f;
  return (speed> schwelle_fahrzeug_ABS) && (currentRPM == 0.0f);
  }
  
  

// ASR: Antriebsschlupf, wenn Fahrzeug langsam und Räder zu schnell
bool checkASR(float speed,volatile unsigned int pulseCount, float schwelle_drehzahl_ASR, float schwelle_fahrzeug_ASR) 
{
  float currentRPM = pulseCount * 60.0;
  return ( speed < schwelle_fahrzeug_ASR  && currentRPM > schwelle_drehzahl_ASR);

}

// ESP: Schleudern erkannt, wenn Z-Gyro > Schwelle
bool checkESP(float dps_Z, float schwelle_esp) 
{
  return (abs( dps_Z) > schwelle_esp);
}

// Diebstahl: Anheben erkannt, wenn Z-Beschleunigung < -Schwelle
bool checkDiebstahl(float acc_Z, float schwelle_diebstahl)
{
  return (acc_Z <= schwelle_diebstahl );
}


// Airbag: Crash erkannt, wenn Y-Beschleunigung < Schwelle
bool checkAirbag(float acc_Y, float schwelle_crash ) 
{
  return (acc_Y <   schwelle_crash  );
}

// Überschlag: erkannt, wenn angleX oder angleY > Schwelle
bool checkUeberschlag(float angleX,float angleY , float schwelle_angle_y,float schwelle_angle_x) 
{
  return (abs(angleX) >= schwelle_angle_x || abs(angleY) >= schwelle_angle_y);
}

// kritischer Abstand: erkannt, wenn Abstand vorne < dynamische Schwelle
bool checkAbstand(int16_t distance_f, float speed) 
{ 
//Schwelle für Abstand – wird dynamisch berechnet
 float schwelle_abstand = ( speed * 100.)/2;

  return (distance_f < schwelle_abstand);
}

