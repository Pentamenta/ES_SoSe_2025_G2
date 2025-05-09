#define CAR_CONNECT //Flag für Precompiler (Adrian)

#include <ArduinoBLE.h>
#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

#define BLE_LED 4 // LED for showing BLE connection

String data_stream;

void setup() {
Serial.begin(9600);
delay(50);

pinMode(BLE_LED, OUTPUT);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Peripherial Device (Adrian)
Dash_connect(); //Verbindung mit dem Dashboard herstellen (Adrian)

}

void loop() {

  if (!dashboard) { // try to reconnect if connection lost
    // Alert car that connection was lost

    Dash_connect();
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
  
  BLE.addService(remote_service); //Fügt den Service zu der Liste an verfügbaren Services hinzu.
  
  BLE.advertise();

  Serial.println("Car (Slave Device)");
  Serial.println(" ");
  
}

void Dash_connect(){ //Verbindung mit dem Dashboard herstellen (Adrian)

  unsigned long t_wait = millis();

do{
  dashboard = BLE.central();

    if (millis() >= t_wait + 500){
      Serial.println("- Discovering Dashboard...");
      t_wait = millis();

      digitalWrite(BLE_LED, !digitalRead(BLE_LED)); // blinks the LED while waiting for connection

    }
    

  if (dashboard.connected()) {
    Serial.println("* Connected to Dashboard!");
    Serial.print("* Device MAC address: ");
    Serial.println(dashboard.address());
    Serial.println(" ");
  }
}while (!dashboard);

digitalWrite(BLE_LED, HIGH);
}
