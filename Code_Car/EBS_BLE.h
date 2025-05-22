#ifndef EBS_BLE_H
#define EBS_BLE_H

#define BOOLNUM 2 // Anzahl der verwendeten bool Ints

/// BLE Variablen (Adrian)

struct exchange_data_to_car { // Alle Variablen vom Dashboard zum Auto
// Geschwindigkeitskontrolle
// speed_val = 0 Stehen bleiben
// speed_val > 0 vorwärts
// speed_val < 0 rückwärts
uint16_t speed_target_val = 0; // Zu erreichende Geschwindigkeit

// Lenkung
// stear_val = 0 Stehen bleiben
// stear_val > 0 rechts
// stear_val < 0 links
uint16_t stear_target_val = 0; // Zu erreichende Lenkung

// Boolean Übertragung an das Auto vom Dashboard
// angefangen mit LSB:

uint16_t angleX;
uint16_t angleY;

uint16_t boolean_0_val;
uint16_t boolean_1_val;
};

struct exchange_data_to_dash { // Alle Variablen vom Auto zum Dashboard

uint16_t speed_actual_val = 0; // Aktuelle Geschwindigkeit
uint16_t stear_actual_val = 0; // Aktuelle Lenkung

// Boolean Übertragung an das Dashboard vom Auto
// angefangen mit LSB:

uint16_t boolean_0_val;
uint16_t boolean_1_val;
};

exchange_data_to_car data_to_car;   // Struct zum Senden der Daten zum Auto
exchange_data_to_dash data_to_dash;  // Struct zum Senden der Daten zum Dashboard


uint8_t *byte_p;        // Poitner für die UART Übertragung
exchange_data_to_car *data_p_c;
exchange_data_to_dash *data_p_d;

// Single Booleans to car

bool boolean_to_car_arr[BOOLNUM][16];

// Single Booleans to dash

bool boolean_to_dash_arr[BOOLNUM][16];


/// UUID und BLE Dekleration (Adrian)

// UUID des vom Dashboard bereitgestellten Services
const char* remote_service_Uuid     = "9aeb113b-c580-4bd1-8eb2-deefe0124d0d"; 

// UUID des vom Dashboard bereitgestellten Services
const char* speed_target_Uuid       = "014b35b7-5ae7-44e8-810b-95d17f2b9170";
const char* speed_actual_Uuid       = "a56a5afb-44ad-4a5b-9f31-36e0aa80f513";

const char* stear_target_Uuid       = "b6d4ffae-9e63-4a57-87b1-027d211b689b";
const char* stear_actual_Uuid       = "f285339c-25ff-47ab-b121-644cc70973ae";

const char* boolean_to_car_0_Uuid     = "6ab25277-1bf1-45c2-b375-0145246125a9";
const char* boolean_to_car_1_Uuid     = "6da77695-40f4-4f60-b7a3-050cf2dff13f";

const char* boolean_to_dash_0_Uuid    = "4620eee8-251b-41bf-8343-4c14ddf73621";
const char* boolean_to_dash_1_Uuid    = "4c7f1db6-173c-46ea-9729-f2eb5cd2e839";

//const char* stear_target_Uuid = "";


// Übergreifende Funktionen

void unpack_bool() {  // heir werden die Boolean Variablen aus einem Int extrahiert (Adrian)

  for (int i = 0; i < 16; i++) {	// Erstes Int entpacken
    if ((data_to_dash.boolean_0_val & (1 << i))) {
      boolean_to_dash_arr[0][i] = true;
    } else {
      boolean_to_dash_arr[0][i] = false;
    }
  }
  for (int i = 0; i < 16; i++) {	// Erstes Int entpacken
    if ((data_to_dash.boolean_1_val & (1 << i))) {
      boolean_to_dash_arr[1][i] = true;
    } else {
      boolean_to_dash_arr[1][i] = false;
    }
  }
}

void package_bool() {  // hier werden die Boolean Variablen in einen int Zusammengefasst (Adrian)

  for (int i = 0; i < 16; i++) {	// Erstes Int verpacken
    if (boolean_to_car_arr[0][15 - i]) {
      data_to_car.boolean_0_val++;
    }
    data_to_car.boolean_0_val << 1;
  }
  for (int i = 0; i < 16; i++) {	// Erstes Int verpacken
    if (boolean_to_car_arr[1][15 - i]) {
      data_to_car.boolean_1_val++;
    }
    data_to_car.boolean_1_val << 1;
  }
}

void Debug_data() { // Debug Ausgabe des Data Structs (Adrian)

  Serial.println("Einmal der Data Struct:");
  Serial.print("Speed_target: ");
  Serial.println(data_to_car.speed_target_val);

  Serial.print("Speed_actual: ");
  Serial.println(data_to_dash.speed_actual_val);

  Serial.print("Stear_target: ");
  Serial.println(data_to_car.stear_target_val);

  Serial.print("Stear_actual: ");
  Serial.println(data_to_dash.stear_actual_val);
  
  Serial.print("Bool 1 to car: ");
  Serial.println(data_to_car.boolean_0_val, BIN);
  
  Serial.print("Bool 1 to dash: ");
  Serial.println(data_to_dash.boolean_0_val, BIN);
  
  Serial.print("AngleX: ");
  Serial.println((int)data_to_car.angleX);
  
  Serial.print("AngleY: ");
  Serial.println((int)data_to_car.angleY);

}


#ifdef DASH  // Defines für Dashboard
    BLEDevice car;
    BLECharacteristic speed_target;
    //speed_target = car.characteristic(speed_target_Uuid); 
    BLECharacteristic speed_actual;
    //speed_actual = car.characteristic(speed_actual_Uuid);

    BLECharacteristic stear_target;

    BLECharacteristic stear_actual;

    BLECharacteristic boolean_to_car_0;
    BLECharacteristic boolean_to_car_1;

    BLECharacteristic boolean_to_dash_0;
    BLECharacteristic boolean_to_dash_1;

// Dashboard Funktionen

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



    #endif

#ifdef CAR_CONNECT // Defines für Car
    BLEDevice dashboard; 

    //Services und Characteristics
    BLEService remote_service(remote_service_Uuid);
    BLEUnsignedIntCharacteristic speed_target(speed_target_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic speed_actual(speed_actual_Uuid, BLERead | BLEWrite);

    BLEUnsignedIntCharacteristic stear_target(stear_target_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic stear_actual(stear_actual_Uuid, BLERead | BLEWrite);

    BLEUnsignedIntCharacteristic boolean_to_car_0(boolean_to_car_0_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic boolean_to_car_1(boolean_to_car_1_Uuid, BLERead | BLEWrite);

    BLEUnsignedIntCharacteristic boolean_to_dash_0(boolean_to_dash_0_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic boolean_to_dash_1(boolean_to_dash_1_Uuid, BLERead | BLEWrite);

    exchange_data_to_dash data_buffer; // Buffer für empfangene Daten über UART (Adrian)

// Funktionen

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

  remote_service.addCharacteristic(boolean_to_car_0);
  remote_service.addCharacteristic(boolean_to_car_1);

  remote_service.addCharacteristic(boolean_to_dash_0); 
  remote_service.addCharacteristic(boolean_to_dash_1);


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
  boolean_to_dash_0.writeValue(data_to_dash.boolean_0_val);
  boolean_to_dash_1.writeValue(data_to_dash.boolean_1_val);

  // Variablen Lesen
  data_to_car.speed_target_val = speed_target.value();
  data_to_car.stear_target_val = stear_target.value();
  data_to_car.boolean_0_val = boolean_to_car_0.value();
  data_to_car.boolean_1_val = boolean_to_car_1.value();

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
    #endif

#ifdef CAR

  exchange_data_to_car data_buffer; // Buffer für empfangene Daten über UART (Adrian)
  
  void Serial_val_exchange() { // Variablen an MEGA Senden und Empfangen (Adrian)

  // Daten zum Auto empfangen
  data_p_c = &data_buffer;      // Pointer vorbereiten
  byte_p = (uint8_t*)data_p_c; 

  if(Serial1.read() == '<') { // Auf start-byte checken

  for (int i = 0; i < sizeof(data_to_car); i++) { // Daten einlesen
    *byte_p = (byte)Serial1.read();
    byte_p++;
  }

  if (Serial1.read() == '>') {
    data_to_car = *data_p_c;
  }
  }

  data_p_d = &data_to_dash;
  byte_p = (uint8_t*)data_p_d;
  Serial1.write('<');
  for (int i = 0; i < sizeof(data_to_dash); i++) {
    //Serial.print(*byte_p, BIN);
    Serial1.write(*byte_p++);
  }
  //Serial.println();
  Serial1.write('>');
  Serial1.flush(); // Warten bis alle Daten im Puffer gesendet wurden

}
  
  
  
  #endif


#endif