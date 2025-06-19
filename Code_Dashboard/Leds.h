// LED Pins
const int led1_bluetooth = 2;
const int led2_blinkerL  = 3;
const int led3_blinkerR  = 4;
const int led4_temp      = 5;
const int led5_airbag    = 6;
const int led6_abs       = 7;

// Simulierte Daten
bool boolean_to_car_val[1][16]; // Steuerdaten vom Dashboard
float temperature_val = 52.5;   // Temperatur in °C

void setup_Leds() {
  pinMode(led1_bluetooth, OUTPUT);
  pinMode(led2_blinkerL, OUTPUT);
  pinMode(led3_blinkerR, OUTPUT);
  pinMode(led4_temp, OUTPUT);
  pinMode(led5_airbag, OUTPUT);
  pinMode(led6_abs, OUTPUT);

 
}

void loop_Leds() {
  // Beispielhafte Signale (du kannst das später durch echte Daten ersetzen)
  boolean_to_car_val[0][0] = true;   // Bluetooth aktiv
  boolean_to_car_val[0][1] = true;   // Blinker links
  boolean_to_car_val[0][2] = false;  // Blinker rechts
  boolean_to_car_val[0][3] = true;   // Airbag
  boolean_to_car_val[0][4] = false;  // ABS
  temperature_val = 52.5;

  // LEDs direkt nach boolean steuern
  digitalWrite(led1_bluetooth, boolean_to_car_val[0][0] ? HIGH : LOW);
  digitalWrite(led2_blinkerL,  boolean_to_car_val[0][1] ? HIGH : LOW);
  digitalWrite(led3_blinkerR,  boolean_to_car_val[0][2] ? HIGH : LOW);
  digitalWrite(led4_temp,      temperature_val > 50.0 ? HIGH : LOW);
  digitalWrite(led5_airbag,    boolean_to_car_val[0][3] ? HIGH : LOW);
  digitalWrite(led6_abs,       boolean_to_car_val[0][4] ? HIGH : LOW);

  // Debug anzeigen
  Serial.print("Temp: ");
  Serial.print(temperature_val);
  Serial.print(" °C | ABS: ");
  Serial.print(boolean_to_car_val[0][4]);
  Serial.print(" | Airbag: ");
  Serial.println(boolean_to_car_val[0][3]);

  delay(200);
}
