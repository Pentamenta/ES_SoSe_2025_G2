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
int pwm = 0;

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
Serial1.begin(115200, SERIAL_8N1);

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

if (digitalRead(CONNECT_NOTIFY)) {
  digitalWrite(UART_LED, HIGH);
}
else {
  digitalWrite(UART_LED, LOW);
}


if (millis() >= t_exchange + 20) { // BLE und Serial Kommunikation (Adrian)
  Serial_val_exchange();
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

//jan
void handleSteering() {
  int xVal = stear_target_val;

  if (xVal > -5 && xVal < 5) {
    MotorGrad = 90; // Zentrum
  } else if (xVal <= -5) {
    MotorGrad = map(xVal, -5, -100, 90, 40); // Links
  } else if (xVal >= 5) {
    MotorGrad = map(xVal, 5, 100, 90, 140); // Rechts
  }
  srv.write(MotorGrad);
  
  int stear_actual_val = xVal;
  
  
}

//jan
void handleDrive() {
  float yVal = speed_target_val;  // Wert im Bereich -10.0 bis +10.0
  int pwm;

  if (yVal > 0.0) {
    // Vorwärts
    float norm = yVal / 10.0;  // Normierung auf 0.0 – 1.0
    pwm = MIN_PWM + norm * (255 - MIN_PWM);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, (int)pwm);
  }
  else if (yVal < 0.0) {
    // Rückwärts
    float norm = -yVal / 10.0;  // Normierung auf 0.0 – 1.0
    pwm = MIN_PWM + norm * (255 - MIN_PWM);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, (int)pwm);
  }
  else {
    // Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
    pwm = 0;
  }
}

//jan
void handleRPMandSpeed() {
  unsigned long currentTime = millis();

  if (currentTime - lastMeasurementTime >= interval) {
    detachInterrupt(digitalPinToInterrupt(sensorDigitalPin));

    // Direkt hier RPM berechnen
    float currentRPM = pulseCount * 60.0;
    float currentSpeed = (currentRPM * wheelCircumference * 60.0) / 1000.0;

    rpmValues[index] = currentRPM;
    speedValues[index] = currentSpeed;
    index = (index + 1) % 3;

    float avgRPM = (rpmValues[0] + rpmValues[1] + rpmValues[2]) / 3.0;
    float avgSpeed = (speedValues[0] + speedValues[1] + speedValues[2]) / 3.0;


    pulseCount = 0;
    lastMeasurementTime = currentTime;

    attachInterrupt(digitalPinToInterrupt(sensorDigitalPin), countPulse, FALLING);

    float speed_actual_val = avgSpeed;
  }
}
