#define DASH //Flag für Precompiler (Adrian)

#define BLE_LED 4

#include <ArduinoBLE.h> 
#include "EBS_BLE.h"  //Custom Header mit BLE definitionen (Adrian)

unsigned long t_debug;
unsigned long t_exchange;

void setup() {
Serial.begin(9600);
delay(50);
t_debug = millis();
t_exchange = millis();

pinMode(BLE_LED, OUTPUT);

BLE_Setup(); //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
connect_car(); //Stellt Verbindung mit dem Auto her (Adrian)
}

void loop() {

  if (!car.connected()) { // Stellt Verbindung wieder her (Adrian)

    // Anzeige auf dem Bildschirm
    // Fahrzeug Steuerung resetten
    connect_car();
  }

  if (millis() >= t_exchange + 20) {
    data_to_car.speed_target_val = random(1,100);
    data_to_car.stear_target_val = random(1,100);
    BLE_val_exchange();
    t_exchange = millis();
  }

  if (millis() >= t_debug + 500){ // Debug Loop
    Serial.println("Bin im Main");
    t_debug = millis();
    }


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

    if (millis() >= t_wait + 500){
      Serial.println("- Still Searching...");
      t_wait = millis();
      digitalWrite(BLE_LED, !digitalRead(BLE_LED));
    }
  } while (!car);

  car.connect();
  car.discoverAttributes();

  speed_target = car.characteristic(speed_target_Uuid); // Characteristika discovern
  speed_actual = car.characteristic(speed_actual_Uuid);

  stear_target = car.characteristic(stear_target_Uuid);
  stear_actual = car.characteristic(stear_actual_Uuid);

  boolean_to_car_0 = car.characteristic(boolean_to_car_0_Uuid);
  boolean_to_car_1 = car.characteristic(boolean_to_car_1_Uuid);

  boolean_to_dash_0 = car.characteristic(boolean_to_dash_0_Uuid);
  boolean_to_dash_1 = car.characteristic(boolean_to_dash_1_Uuid);


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

    digitalWrite(BLE_LED, HIGH);

  }
}

void BLE_val_exchange() { // BLE Variablen Senden und Empfangen (Adrian)

  package_bool();
  // Variablen Senden
  speed_target.writeValue(data_to_car.speed_target_val);
  stear_target.writeValue(data_to_car.stear_target_val);
  boolean_to_car_0.writeValue(data_to_car.boolean_0_val);
  boolean_to_car_1.writeValue(data_to_car.boolean_1_val);

  // Variablen Lesen
  speed_actual.readValue(data_to_dash.speed_actual_val);
  stear_actual.readValue(data_to_dash.stear_actual_val);
  boolean_to_dash_0.readValue(data_to_dash.boolean_0_val);
  boolean_to_dash_1.readValue(data_to_dash.boolean_1_val);

  unpack_bool();
}
