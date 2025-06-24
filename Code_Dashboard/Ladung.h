#ifndef Ladung_H
#define Ladung_H


// Funktion zur darstellung des Batteriezusandes
void writeload(uint16_t load) {
  display.clearDisplay();  // Anzeige löschen

  //Ausgabe Viereck
  display.drawRect(30, 11, 80, 37, WHITE);
  //Viereck ans obere Ende
  display.drawRect(23, 20, 8, 19, WHITE);
  //volle Vierecke für den Ladestand
  for(int i = 0; i <= load/10; i+=1){
    display.fillRect(110-i*8, 11, i*8, 37, WHITE);
  }

  //Textausgabe Ladezustand
  display.cp437(true);
  display.setRotation(3);
  display.setTextSize(2);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(16, 0);         // Position des Textes
  display.print(load);
  display.write(0x25);

  display.display(); // Anzeige aktualisieren
}

void setup() {
  Serial.begin(9600); // Serielle Ausgabe zum Debuggen

  // Display initialisieren – 0x3C ist die Standardadresse
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed")); // Fehlerausgabe
    while (true); // Endlosschleife bei Fehler
  }
  writeload(load);

}

void loop() {
 /* int sensorValue = analogRead(A0);       // Wert vom analogen Eingang A0 lesen
  float speed = sensorValue * 0.195503;     // Umrechnen auf Geschwindigkeit (ca. 0–200 km/h)
 */

  delay(200);                               // Kurze Pause (200 ms)
}
#endif