#ifndef EBS_BLE_H
#define EBS_BLE_H

/// BLE Variablen (Adrian)

struct exchange_data { // Alle Variablen werden in einem Struct gespeichert.
// Geschwindigkeitskontrolle
// speed_val = 0 Stehen bleiben
// speed_val > 0 vorwärts
// speed_val < 0 rückwärts
uint16_t speed_target_val = 0; // Zu erreichende Geschwindigkeit
uint16_t speed_actual_val = 0; // Aktuelle Geschwindigkeit

// Lenkung
// stear_val = 0 Stehen bleiben
// stear_val > 0 rechts
// stear_val < 0 links
uint16_t stear_target_val = 0; // Zu erreichende Lenkung
uint16_t stear_actual_val = 0; // Aktuelle Lenkung

// Boolean Übertragung an das Auto vom Dashboard
// angefangen mit LSB:

uint16_t boolean_to_car_val = 0;

// Boolean Übertragung an das Dashboard vom Auto
// angefangen mit LSB:

uint16_t boolean_to_dash_val = 0;

};

exchange_data data; // Struct für den Austausch der Daten

uint8_t *byte_p;        // Poitner für die UART Übertragung
exchange_data *data_p;


// Single Booleans to car

bool boolean_to_car_arr[16];

// Single Booleans to dash

bool boolean_to_dash_arr[16];


/// UUID und BLE Dekleration (Adrian)

// UUID des vom Dashboard bereitgestellten Services
const char* remote_service_Uuid     = "9aeb113b-c580-4bd1-8eb2-deefe0124d0d"; 

// UUID des vom Dashboard bereitgestellten Services
const char* speed_target_Uuid       = "014b35b7-5ae7-44e8-810b-95d17f2b9170";
const char* speed_actual_Uuid       = "a56a5afb-44ad-4a5b-9f31-36e0aa80f513";

const char* stear_target_Uuid       = "b6d4ffae-9e63-4a57-87b1-027d211b689b";
const char* stear_actual_Uuid       = "f285339c-25ff-47ab-b121-644cc70973ae";

const char* boolean_to_car_Uuid     = "6ab25277-1bf1-45c2-b375-0145246125a9";

const char* boolean_to_dash_Uuid    = "4620eee8-251b-41bf-8343-4c14ddf73621";

//const char* stear_target_Uuid = "";

#ifdef DASH  // Defines für Dashboard
    BLEDevice car;
    BLECharacteristic speed_target;
    //speed_target = car.characteristic(speed_target_Uuid); 
    BLECharacteristic speed_actual;
    //speed_actual = car.characteristic(speed_actual_Uuid);

    BLECharacteristic stear_target;
    //stear_target = car.characteristic(stear_target_Uuid);
    BLECharacteristic stear_actual;
    //stear_actual = car.characteristic(stear_actual_Uuid);

    BLECharacteristic boolean_to_car;
    //boolean_to_car = car.characteristic(boolean_to_car_Uuid);

    BLECharacteristic boolean_to_dash;
    //boolean_to_dash = car.characteristic(boolean_to_dash_Uuid);

    #endif

#ifdef CAR_CONNECT // Defines für Car
    BLEDevice dashboard; 

    //Services und Characteristics
    BLEService remote_service(remote_service_Uuid);
    BLEIntCharacteristic speed_target(speed_target_Uuid, BLERead | BLEWrite);
    BLEIntCharacteristic speed_actual(speed_actual_Uuid, BLERead | BLEWrite);

    BLEIntCharacteristic stear_target(stear_target_Uuid, BLERead | BLEWrite);
    BLEIntCharacteristic stear_actual(stear_actual_Uuid, BLERead | BLEWrite);

    BLEUnsignedIntCharacteristic boolean_to_car(boolean_to_car_Uuid, BLERead | BLEWrite);
    BLEUnsignedIntCharacteristic boolean_to_dash(boolean_to_dash_Uuid, BLERead | BLEWrite);
    #endif

#ifdef CAR



    #endif


#endif