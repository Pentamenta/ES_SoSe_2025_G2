#define CAR_CONNECT //Flag für Precompiler (Adrian)

#include <ArduinoBLE.h>
#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

#define CONNECT_NOTIFY 2 // High wenn BLE Verbindung besteht (Adrian)
#define BLE_LED 4 // LED für BLE Verbindung (Adrian)


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

void BLE_Setup(){ //Öffnet die BLE-Schnittstelle und initiallisiert das Peripherial Device (Adrian)
// Ist unbedingt erst nach Serial.begin() auszuführen.

  if (!BLE.begin()) { //Funktion des BLE Moduls überprüfen
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  
  BLE.setLocalName("Car (Peripherial)"); //Namen des Central Devices Festlegen

  BLE.setAdvertisedService(remote_service); //Setzt die UUID des Services, wenn er advertised wird
  remote_service.addCharacteristic(speed_target); //Charakteristiken zum Service hinzufügen
  remote_service.addCharacteristic(speed_actual); 

  remote_service.addCharacteristic(stear_target); 
  remote_service.addCharacteristic(stear_actual); 

  remote_service.addCharacteristic(boolean_to_car); 
  remote_service.addCharacteristic(boolean_to_dash); 


  BLE.addService(remote_service); //Fügt den Service zu der Liste an verfügbaren Services hinzu.
  
  BLE.advertise();

  Serial.println("Car (Slave Device)");
  Serial.println(" ");
  
}

void Dash_connect() { //Verbindung mit dem Dashboard herstellen (Adrian)

  unsigned long t_wait = millis();

do{
  dashboard = BLE.central();

    if (millis() >= t_wait + 500){
      Serial.println("- Discovering Dashboard...");
      t_wait = millis();

      digitalWrite(BLE_LED, !digitalRead(BLE_LED)); // blinks the LED while waiting for connection

    }
    
  } while (!dashboard);

  if (dashboard) {
    Serial.println("* Connected to Dashboard!");
    Serial.print("* Device MAC address: ");
    Serial.println(dashboard.address());
    Serial.println(" ");
    digitalWrite(BLE_LED, HIGH);
    digitalWrite(CONNECT_NOTIFY, HIGH);
  }

}

void BLE_val_exchange() { // BLE Variablen Senden und Empfangen (Adrian)

  package_bool();
  // Variablen Senden
  speed_actual.writeValue(data_to_dash.speed_actual_val);
  stear_actual.writeValue(data_to_dash.stear_actual_val);
  boolean_to_dash.writeValue(data_to_dash.boolean_val);

  // Variablen Lesen
  data_to_car.speed_target_val = speed_target.value();
  data_to_car.stear_target_val = stear_target.value();
  data_to_car.boolean_val = boolean_to_car.value();

  unpack_bool();
}

void Serial_val_exchange() { // Variablen an MEGA Senden und Empfangen (Adrian)

  // Daten zum Dashboard empfangen
  data_p_d = &data_buffer;      // Pointer vorbereiten
  byte_p = (uint8_t*)data_p_d; 

  if(Serial1.read() == '<') { // Auf start-byte checken

  for (int i = 0; i < sizeof(data_to_dash); i++) { // Daten einlesen
    *byte_p = (byte)Serial1.read();
    byte_p++;
  }

  if (Serial1.read() == '>') {
    data_to_dash = *data_p_d;
  }
  }

  data_p_c = &data_to_car;
  byte_p = (uint8_t*)data_p_c;
  Serial1.write('<');
  for (int i = 0; i < sizeof(data_to_car); i++) {
    //Serial.print(*byte_p, BIN);
    Serial1.write(*byte_p++);
  }
  //Serial.println();
  Serial1.write('>');
  Serial1.flush(); // Warten bis alle Daten im Puffer gesendet wurden

}

