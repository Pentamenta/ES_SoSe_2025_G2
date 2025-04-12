#define DASH //Flag für Precompiler (Adrian)

#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  //Custom Header mit BLE definitionen (Adrian)

void setup() {
Serial.begin(9600);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
connect_car(); //Stellt Verbindung mit dem Auto her (Adrian)
send_speed();  //Testfunktion zum Senden einer Variable ans Auto (Adrian)
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

speed_target = car.characteristic(speed_target_Uuid); //Weise Charakteristik des Autos zu

 if (!speed_target) { //Überprufe erreichbarkeit und Schreibbarkeit der Charakteristik --> Im Fehlerfall return 0
    Serial.println("* Car does not have gesture_type characteristic!");
    car.disconnect();
    return 0;
  } else if (!speed_target.canWrite()) {
    Serial.println("* Peripheral does not have a writable gesture_type characteristic!");
    car.disconnect();
    return 0;
  }

//######################################################### Test Block
    while (car.connected()) {
    speed_target_val = random(1, 100);
    
      Serial.print("* Writing value to gesture_type characteristic: ");
      Serial.println(speed_target_val);
      speed_target.writeValue((byte)speed_target_val);
      Serial.println("* Writing value to gesture_type characteristic done!");
      Serial.println(" ");
  
  }
  Serial.println("- Car disconnected!");
//######################################################### Test Block
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
