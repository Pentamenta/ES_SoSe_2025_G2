#include <ArduinoBLE.h>
#include "EBS_BLE.h" //Custom Header mit BLE definitionen (Adrian)

void setup() {
Serial.begin(9600);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Peripherial Device (Adrian)
Dash_connect(); //Verbindung mit dem Dashboard herstellen (Adrian)

}

void loop() {

read_speed_target();

}


void read_speed_target(){ //Debug Funktion zum Testen von BLE (Adrian)
  if (speed_target.written()) {
         speed_target_val = speed_target.value();
         Serial.println("Geschwindigkeit empfangen:");
         Serial.println(speed_target_val);
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
  
  speed_target.writeValue(speed_target);
  BLE.advertise();

  Serial.println("Car (Dashboard Device)");
  Serial.println(" ");
  
}

void Dash_connect(){ //Verbindung mit dem Dashboard herstellen (Adrian)
  Serial.println("- Discovering Dashboard...");
  delay(500);

  if (dashboard) {
    Serial.println("* Connected to Dashboard!");
    Serial.print("* Device MAC address: ");
    Serial.println(dashboard.address());
    Serial.println(" ");
  }

}
