#ifndef EBS_BLE_H
#define EBS_BLE_H

///BLE Variablen (Adrian)

int speed_target = 0;
int speed_actual = 0;

///UUID Dekleration (Adrian)

//UUID des vom Dashboard bereitgestellten Services
const char* remote_service_Uuid = "9aeb113b-c580-4bd1-8eb2-deefe0124d0d"; 

//UUID des vom Dashboard bereitgestellten Services
const char* speed_target_Uuid = "014b35b7-5ae7-44e8-810b-95d17f2b9170"; //Soll-Geschwindigkeit, die ans Fahrzeug gesendet wird
const char* speed_actual_Uuid = "a56a5afb-44ad-4a5b-9f31-36e0aa80f513"; //Ist-Geschwindigkeit, die das Fahrzeug zurück gibt

#endif