#ifndef EBS_BLE_H
#define EBS_BLE_H

#define BOOLNUM 2 // Anzahl der verwendeten bool Ints
#define FAST_CYCLE 20 // Zeitintervall für schnelle BLE Übertragung + UART Übertragung
#define SLOW_CYCLE 100 // Zeitintervall für langsame BLE Übertragung

/// BLE Variablen (Adrian)
#pragma pack(1)
struct exchange_data_to_car { // Alle Variablen vom Dashboard zum Auto
// Geschwindigkeitskontrolle
// speed_val = 0 Stehen bleiben
// speed_val > 0 vorwärts
// speed_val < 0 rückwärts
float speed_target_val = 0; // Zu erreichende Geschwindigkeit

float dps_X_val;
float dps_Y_val;
float dps_Z_val;
float acc_X_val;
float acc_Y_val;
float acc_Z_val;

// Lenkung
// stear_val = 0 Stehen bleiben
// stear_val > 0 rechts
// stear_val < 0 links
int16_t stear_target_val = 0; // Zu erreichende Lenkung

int16_t angleX;
int16_t angleY;

// Boolean Übertragung an das Auto vom Dashboard
// angefangen mit LSB:
uint16_t boolean_0_val;
uint16_t boolean_1_val;
};

struct exchange_data_to_dash { // Alle Variablen vom Auto zum Dashboard

float speed_actual_val = 0; // Aktuelle Geschwindigkeit
float temperature_val;		// Temperatur am Auto
int16_t stear_actual_val = 0; // Aktuelle Lenkung
// Abstandssensoren
int16_t distance_f_val; // Sensor vorne
int16_t distance_b_val; // Sensor hinten
int16_t distance_r_f_val; //Sensor vorne rechts
int16_t distance_r_b_val; //Sensor hinten rechts
int16_t distance_l_f_val; //Sensor vorne links
int16_t distance_l_b_val; //Sensor hinten links

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

const char* temperature_Uuid		  = "3d7ae5af-3f77-4a6a-b17d-9ba9fd495f61";

const char* angleX_Uuid               = "d457688d-774c-4263-83fe-a5ff0ccadf18";
const char* angleY_Uuid               = "88596d2b-649e-4487-b654-f9b340242ebf";

//const char* dps_X_Uuid				= "bae5d570-f4be-41ac-8b4f-af6855bd2732";
//const char* dps_Y_Uuid				= "e48e5f2e-8f87-4df8-ad31-861477c22439";
//const char* dps_Z_Uuid				= "058f0832-d281-4a27-a67c-778ae1c86cac";

const char* distance_f_Uuid			= "bf0b2732-e7a1-4a5f-8a9e-9f090c689a22";
const char* distance_b_Uuid			= "b4b6ade5-78a0-4274-bd4d-b47a55852ab1";
const char* distance_r_f_Uuid		= "eb3fde35-0c2a-45dc-8bce-483143d776cb";
const char* distance_r_b_Uuid		= "b219fde3-140f-4330-a82b-3ce08ebf968a";
const char* distance_l_f_Uuid		= "40a50c5f-76d2-473d-94bf-55aa8d7f2802";
const char* distance_l_b_Uuid		= "65e1ce7d-84d9-4bd9-a605-7856dcc2608f";

//const char* stear_target_Uuid = "";

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
	
    BLECharacteristic temperature;

    BLECharacteristic angleX;
    BLECharacteristic angleY;
	
    BLECharacteristic distance_f;
    BLECharacteristic distance_b;
    BLECharacteristic distance_r_f;
    BLECharacteristic distance_r_b;
    BLECharacteristic distance_l_f;
    BLECharacteristic distance_l_b;


// Dashboard Funktionen

void BLE_Setup(){ //Öffnet die BLE-Schnittstelle und initiallisiert das Central Device (Adrian)
// Ist unbedingt erst nach Serial.begin() auszuführen.

  if (!BLE.begin()) { //Funktion des BLE Moduls überprüfen
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  
  BLE.setDeviceName("Dashboard (Central)"); //Namen des Central Devices Festlegen
  BLE.advertise();

  Serial.println("Dashboard (Central Device)");
  Serial.println(" ");
  
}

void connect_car(){ //Stellt Verbindung mit dem Auto her (Adrian)

  Serial.println("- Searching for car...");
  unsigned long t_wait = millis();

  BLE.scan();

  do  //Suche nach dem peripheral Device, bis es gefunden wurde
  {
    car = BLE.available();

    if (millis() >= t_wait + 500){
      Serial.println("- Still Searching...");
      t_wait = millis();
      digitalWrite(BLE_LED, !digitalRead(BLE_LED));
    }
  } while (!car);

  BLE.stopScan();

  Serial.println("- Connecting to peripheral Car...");
  if (car.connect()) { 
    Serial.println("* Connected to Car!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to Car failed!");
    Serial.println(" ");
    return;
  }

  if (car.discoverAttributes()) {
    Serial.println("Car Attributes discovered.");
  }
  else {
    Serial.println("Car Attributes discovery failed.");
    car.disconnect();
    return;
  }

    Serial.println("* Found the Car!");
    Serial.print("* Device MAC address: ");
    Serial.println(car.address());
    Serial.print("* Device name: ");
    Serial.println(car.deviceName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(car.advertisedServiceUuid());
    Serial.println(" ");

    digitalWrite(BLE_LED, HIGH);

  // Setting up Characteristics
  speed_target = car.characteristic(speed_target_Uuid); // Characteristika discovern
  speed_actual = car.characteristic(speed_actual_Uuid);

  stear_target = car.characteristic(stear_target_Uuid);
  stear_actual = car.characteristic(stear_actual_Uuid);

  boolean_to_car_0 = car.characteristic(boolean_to_car_0_Uuid);
  boolean_to_car_1 = car.characteristic(boolean_to_car_1_Uuid);
  
  temperature = car.characteristic(temperature_Uuid);

  boolean_to_dash_0 = car.characteristic(boolean_to_dash_0_Uuid);
  boolean_to_dash_1 = car.characteristic(boolean_to_dash_1_Uuid);

  angleX = car.characteristic(angleX_Uuid);
  angleY = car.characteristic(angleY_Uuid);
  
  distance_f = car.characteristic(distance_f_Uuid);
  distance_b = car.characteristic(distance_b_Uuid);
  distance_r_f = car.characteristic(distance_r_f_Uuid);
  distance_r_b = car.characteristic(distance_r_b_Uuid);
  distance_l_f = car.characteristic(distance_l_f_Uuid);
  distance_l_b = car.characteristic(distance_l_b_Uuid);
}

    #endif

#ifdef CAR_CONNECT // Defines für Car
    BLEDevice dashboard; 

    //Services und Characteristics
    BLEService remote_service(remote_service_Uuid);
    BLEFloatCharacteristic speed_target(speed_target_Uuid, BLERead | BLEWrite);
    BLEFloatCharacteristic speed_actual(speed_actual_Uuid, BLERead | BLEWrite);

    BLEIntCharacteristic stear_target(stear_target_Uuid, BLERead | BLEWrite);
    BLEIntCharacteristic stear_actual(stear_actual_Uuid, BLERead | BLEWrite);

    BLEUnsignedIntCharacteristic boolean_to_car_0(boolean_to_car_0_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic boolean_to_car_1(boolean_to_car_1_Uuid, BLERead | BLEWrite);

    BLEUnsignedIntCharacteristic boolean_to_dash_0(boolean_to_dash_0_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic boolean_to_dash_1(boolean_to_dash_1_Uuid, BLERead | BLEWrite);
	
    BLEFloatCharacteristic temperature(temperature_Uuid, BLERead | BLEWrite);

    BLEIntCharacteristic angleX(angleX_Uuid, BLERead | BLEWrite);
    BLEIntCharacteristic angleY(angleY_Uuid, BLERead | BLEWrite);
	
    BLEFloatCharacteristic distance_f(distance_f_Uuid, BLERead | BLEWrite);
    BLEFloatCharacteristic distance_b(distance_b_Uuid, BLERead | BLEWrite);
    BLEFloatCharacteristic distance_r_f(distance_r_f_Uuid, BLERead | BLEWrite);
    BLEFloatCharacteristic distance_r_b(distance_r_b_Uuid, BLERead | BLEWrite);
    BLEFloatCharacteristic distance_l_f(distance_l_f_Uuid, BLERead | BLEWrite);
    BLEFloatCharacteristic distance_l_b(distance_l_b_Uuid, BLERead | BLEWrite);

    exchange_data_to_dash data_buffer; // Buffer für empfangene Daten über UART (Adrian)

// Funktionen

void DashConnectHandler(BLEDevice central){

  dashboard = central;

  if (dashboard.connect()) {
    Serial.println("Connected to Central Device.");
  }
  else {
    Serial.println("Connection to Central Device failed.");
  }
  
  // Attribute erkennen
  Serial.println("- Discovering Dashboard attributes...");
  if (central.discoverAttributes()) { 
    Serial.println("* Dashboard attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Dashboard attributes discovery failed!");
    Serial.println(" ");
    central.disconnect();
    return;
  }
  
  Serial.println("Neues Central Device Verbunden!");
  Serial.print("Name: ");
  Serial.println(central.deviceName());
  Serial.print("Adresse: ");
  Serial.println(central.address());
  Serial.println();

  digitalWrite(BLE_LED, HIGH);
  digitalWrite(CONNECT_NOTIFY, HIGH);

}

void DashDisconnectHandler(BLEDevice central){

  Serial.println("Connection to Dashboard closed.");
  dashboard.disconnect();
}

void BLE_Setup(){ //Öffnet die BLE-Schnittstelle und initiallisiert das Peripherial Device (Adrian)
// Ist unbedingt erst nach Serial.begin() auszuführen.

  if (!BLE.begin()) { //Funktion des BLE Moduls überprüfen
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  
  BLE.setDeviceName("Car (Peripherial)"); //Namen des Central Devices Festlegen

  BLE.setAdvertisedService(remote_service); //Setzt die UUID des Services, wenn er advertised wird
  remote_service.addCharacteristic(speed_target); //Charakteristiken zum Service hinzufügen
  remote_service.addCharacteristic(speed_actual); 

  remote_service.addCharacteristic(stear_target); 
  remote_service.addCharacteristic(stear_actual); 

  remote_service.addCharacteristic(boolean_to_car_0);
  remote_service.addCharacteristic(boolean_to_car_1);

  remote_service.addCharacteristic(boolean_to_dash_0);
  remote_service.addCharacteristic(boolean_to_dash_1);

  remote_service.addCharacteristic(temperature);

  remote_service.addCharacteristic(angleX);
  remote_service.addCharacteristic(angleY);
  
  remote_service.addCharacteristic(distance_f);
  remote_service.addCharacteristic(distance_b);
  remote_service.addCharacteristic(distance_r_f);
  remote_service.addCharacteristic(distance_r_b);
  remote_service.addCharacteristic(distance_l_f);
  remote_service.addCharacteristic(distance_l_b);

  BLE.addService(remote_service); //Fügt den Service zu der Liste an verfügbaren Services hinzu.
  BLE.advertise();

  BLE.setEventHandler(BLEConnected, DashConnectHandler);
  BLE.setEventHandler(BLEDisconnected, DashDisconnectHandler);

  Serial.println("Car (Slave Device)");
  Serial.println(" ");
  
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
  //Serial1.flush(); // Warten bis alle Daten im Puffer gesendet wurden

  /*
  //Debug:
  Serial.println("data_to_dash");
  byte_p = (uint8_t*)data_p_d;
  for (int j = 0; j < sizeof(data_to_dash); j++) {
    Serial.print(*byte_p, BIN);
    byte_p++;
  }
  Serial.println();

  Serial.println("data_to_car");
  byte_p = (uint8_t*)data_p_c;
  for (int k = 0; k < sizeof(data_to_car); k++) {
    Serial.print(*byte_p, BIN);
    byte_p++;
  }
  Serial.println();
  */
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
  //Serial1.flush(); // Warten bis alle Daten im Puffer gesendet wurden

  /*
  //Debug:
  Serial.println("data_to_dash");
  byte_p = (uint8_t*)data_p_d;
  for (int j = 0; j < sizeof(data_to_dash); j++) {
    Serial.print(*byte_p, BIN);
    byte_p++;
  }
  Serial.println();

  Serial.println("data_to_car");
  byte_p = (uint8_t*)data_p_c;
  for (int k = 0; k < sizeof(data_to_car); k++) {
    Serial.print(*byte_p, BIN);
    byte_p++;
  }
  Serial.println();
*/
}
#endif

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


void BLE_fast_exchange() { // BLE Variablen Senden und Empfangen (Adrian)

  package_bool();
  
#ifdef DASH
  // Variablen Senden Dashboard
  speed_target.writeValue(&data_to_car.speed_target_val, 4);  // Float
  stear_target.writeValue(&data_to_car.stear_target_val, 4);  // Int
  boolean_to_car_0.writeValue(&data_to_car.boolean_0_val, 2); // Unsigned Int
  boolean_to_car_1.writeValue(&data_to_car.boolean_1_val, 2); // Unsigned Int
#endif
#ifdef CAR_CONNECT
  // Variablen Senden Car
  speed_actual.writeValue(data_to_dash.speed_actual_val);
  boolean_to_dash_0.writeValue(data_to_dash.boolean_0_val);
  boolean_to_dash_1.writeValue(data_to_dash.boolean_1_val);
#endif
#ifdef DASH
  // Variablen Lesen Dashboard
  speed_actual.readValue(&data_to_dash.speed_actual_val, 4);
  boolean_to_dash_0.readValue(&data_to_dash.boolean_0_val, 2);
  boolean_to_dash_1.readValue(&data_to_dash.boolean_1_val, 2);
#endif
#ifdef CAR_CONNECT
  // Variablen Lesen
  speed_target.readValue(&data_to_car.speed_target_val, 4);
  stear_target.readValue(&data_to_car.stear_target_val, 4);
  boolean_to_car_0.readValue(&data_to_car.boolean_0_val, 2);
  boolean_to_car_1.readValue(&data_to_car.boolean_1_val, 2);
#endif

  unpack_bool();
}

void BLE_slow_exchange() { // BLE Variablen Senden und Empfangen (Adrian)

#ifdef CAR_CONNECT
  // Variablen Senden Car
  stear_actual.writeValue(data_to_dash.stear_actual_val);
  angleX.writeValue(data_to_car.angleX);
  angleY.writeValue(data_to_car.angleY);
  temperature.writeValue(data_to_dash.temperature_val);
  distance_f.writeValue(data_to_dash.distance_f_val);
  distance_b.writeValue(data_to_dash.distance_b_val);
  distance_r_f.writeValue(data_to_dash.distance_r_f_val);
  distance_r_b.writeValue(data_to_dash.distance_r_b_val);
  distance_l_f.writeValue(data_to_dash.distance_l_f_val);
  distance_l_b.writeValue(data_to_dash.distance_l_b_val);
#endif
#ifdef DASH
  // Variablen Lesen Dashboard
  stear_actual.readValue(&data_to_dash.stear_actual_val, 4);
  angleX.readValue(&data_to_car.angleX, 2);
  angleY.readValue(&data_to_car.angleY, 2);
  temperature.readValue(&data_to_dash.temperature_val, 4);
  distance_f.readValue(&data_to_dash.distance_f_val, 2);
  distance_b.readValue(&data_to_dash.distance_b_val, 2);
  distance_r_f.readValue(&data_to_dash.distance_r_f_val, 2);
  distance_r_b.readValue(&data_to_dash.distance_r_b_val, 2);
  distance_l_f.readValue(&data_to_dash.distance_l_f_val, 2);
  distance_l_b.readValue(&data_to_dash.distance_l_b_val, 2);
#endif
  unpack_bool();
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
  Serial.println(data_to_car.angleX);
  
  Serial.print("AngleY: ");
  Serial.println(data_to_car.angleY);

}

#endif