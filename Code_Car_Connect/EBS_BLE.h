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
    #endif

#ifdef CAR

  exchange_data_to_car data_buffer; // Buffer für empfangene Daten über UART (Adrian)
  #endif


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

}