#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  //Custom Header mit BLE definitionen (Adrian)

void setup() {
Serial.begin(9600);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
connect_car(); //Stellt Verbindung mit dem Auto her (Adrian)

}

void loop() {



}

void BLE_Setup(){ //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
// Ist unbedingt erst nach Serial.begin() auszuführen.

  if (!BLE.begin()) { //Funktion des BLE Moduls überprüfen
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  
  BLE.setLocalName("Dashboard (Central)"); //Namen des Central Devices Festlegen
  BLE.advertise();

  Serial.println("Dashboard (Central Device)");
  Serial.println(" ");
  
}

void connect_car(){ //Stellt Verbindung mit dem Auto her (Adrian)
  BLEDevice car; //Erstellt das Objekt car

  Serial.println("- Searching for car...");

  unsigned long t_wait = millis();

  do  //Suche nach dem peripheral Device, bis es gefunden wurde
  {
    BLE.scanForUuid(remote_service_Uuid);
    car = BLE.available();

    if (millis() >= t_wait +1000){
      Serial.println("- Still Searching...");
      t_wait = millis();
    }
  } while (!car);

 if (car) { //Einrichten des Autos
    Serial.println("* Found the Car!");
    Serial.print("* Device MAC address: ");
    Serial.println(car.address());
    Serial.print("* Device name: ");
    Serial.println(car.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(car.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
  }
}
