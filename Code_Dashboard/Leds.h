// LED Pins
#define LED1  A6 // Blinker R
#define LED2  A7 // Blinker L
#define LED3  11 // Tempomat Aktiv
#define LED4  12 // Airbag
#define LED5  13 // ABS
#define BLINK 500

unsigned long t_blink_l, t_blink_r, t_blink_w = millis();

void Led_Setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
}

void blinker()  {

  if (boolean_to_dash_arr[0][2] && (millis() >= t_blink_w + BLINK)) { 
    digitalWrite(LED1, !digitalRead(LED1)); // Warnblinker einschalten
    digitalWrite(LED2, !digitalRead(LED2));
    t_blink_w = millis();
  }
  else if (!boolean_to_dash_arr[0][2]){ // Warnblinker abschalten
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }

  if (boolean_to_dash_arr[0][0] && (millis() >= t_blink_r + BLINK) && !boolean_to_dash_arr[0][2]) { 
    digitalWrite(LED1, !digitalRead(LED1)); // Blinker R einschalten
    t_blink_r = millis();
  }
  else if (!boolean_to_dash_arr[0][0]){ // Blinker R abschalten
    digitalWrite(LED1, LOW);
  }

  if (boolean_to_dash_arr[0][1] && (millis() >= t_blink_l + BLINK) && !boolean_to_dash_arr[0][2]) { 
    digitalWrite(LED2, !digitalRead(LED2)); // Blinker L einschalten
    t_blink_l = millis();
  }
  else if (!boolean_to_dash_arr[0][1]){ // Blinker L abschalten
    digitalWrite(LED2, LOW);
  }

}

void Led_Update() {

  // LEDs direkt nach boolean steuern
  digitalWrite(LED3, tempo_on ? HIGH : LOW); // Check Tempomat
  digitalWrite(LED4, boolean_to_dash_arr[0][13] ? HIGH : LOW); // Check Airbag
  digitalWrite(LED5, boolean_to_dash_arr[0][10] ? HIGH : LOW); // Check ABS

  blinker();

}