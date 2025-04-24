#define DASH //Flag für Precompiler (Adrian)

#define BLE_LED 4

#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  //Custom Header mit BLE definitionen (Adrian)

bool ble_led_val = LOW;

void setup() {
Serial.begin(9600);
delay(50);

pinMode(BLE_LED, OUTPUT);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
connect_car(); //Stellt Verbindung mit dem Auto her (Adrian)
}

void loop() {




}

bool send_speed(){ //Testfunktion zum Senden einer Variable ans Auto (Adrian)
    Serial.println("- Connecting to car...");

  if (car.connect()) { //Verbindung überprüfen --> Im Fehlerfall return 0
    Serial.println("* Connected to car!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to car failed!");
    Serial.println(" ");
    return 0;
  }

  Serial.println("- Discovering car attributes...");
  if (car.discoverAttributes()) { //Attribute überprüfen --> Im Fehlerfall return 0
    Serial.println("* Car attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Car attributes discovery failed!");
    Serial.println(" ");
    car.disconnect();
    return 0;
  }

/*
 if (!speed_target) { //Überprufe erreichbarkeit und Schreibbarkeit der Charakteristik --> Im Fehlerfall return 0
    Serial.println("* Car does not have characteristic!");
    car.disconnect();
    return 0;
  } else if (!speed_target.canWrite()) {
    Serial.println("* Peripheral does not have a writable characteristic!");
    car.disconnect();
    return 0;
  }
*/

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

  Serial.println("- Searching for car...");

  unsigned long t_wait = millis();

  do  //Suche nach dem peripheral Device, bis es gefunden wurde
  {
    BLE.scanForUuid(remote_service_Uuid);
    car = BLE.available();

    if (millis() >= t_wait +1000){
      Serial.println("- Still Searching...");
      t_wait = millis();
      ble_led_val = !ble_led_val;
      digitalWrite(BLE_LED, ble_led_val);
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

    ble_led_val = true;
    digitalWrite(BLE_LED, ble_led_val);

  }
}
