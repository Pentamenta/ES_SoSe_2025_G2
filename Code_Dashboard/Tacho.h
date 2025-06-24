#ifndef Tacho_H
#define Tacho_H

// Variable für die anzuzeigende Geschwindigkeit
float speed_actual_val;
int speed = (int) speed_actual_val;

// Arrays zur Zwischenspeicherung der Skalenkoordinaten (12 Markierungen)
int x1 [13];  // Startpunkt x (innen)
int x2 [13];  // Endpunkt x (außen)
int yeins [13];  // Startpunkt y (innen)
int y2 [13];  // Endpunkt y (außen)

// Mittelpunkt und Radius der Tachoskala
int centerX = 64;
int centerY = 60;
int radius = 40;

// Funktion zur Vorberechnung der Skalenlinien
void tachorech() {
  for (int i = 0; i <= 12; i += 1) {
    // Winkel im Bereich von -180° bis 0° in 15°-Schritten
    int angle = -180 + i * 15;
    float rad = angle * 3.1416 / 180.0;  // Umrechnung Grad → Bogenmaß

    // Koordinaten berechnen
    x1 [i] = centerX + cos(rad) * (radius - 5);  // Innenpunkt der Skalenlinie
    yeins [i] = centerY + sin(rad) * (radius - 5);
    x2 [i] = centerX + cos(rad) * radius;        // Außenpunkt
    y2 [i] = centerY + sin(rad) * radius;
  }
}

// Funktion zum Zeichnen des Tachos mit Nadel und Textanzeige
void writeTacho(uint16_t ms, uint16_t tempo, int tempo_on, int temp, int neig) {
  display.clearDisplay();  // Anzeige löschen

  // Skala zeichnen
  for (int p = 0; p <= 12; p += 1) {
    display.drawLine(x1[p], yeins[p], x2[p], y2[p], WHITE);
  }

  // Geschwindigkeit → Winkel umrechnen (zwischen -180° und 0°)
  float angle = map(ms, 0, MAX_SPEED, -180, 0);
  float rad = angle * 3.1416 / 180.0;

  // Koordinaten für Nadelspitze berechnen
  int needleX = centerX + cos(rad) * (radius - 10);
  int needleY = centerY + sin(rad) * (radius - 10);

  // Nadel zeichnen (vom Zentrum zum Nadelpunkt)
  display.drawLine(centerX, centerY, needleX, needleY, WHITE);

  // dicke Nadel zeichnen für den Tempomat zeichnen (vom Zentrum zum Nadelpunkt)
  if (tempo_on == 1){
    // Tempomatsnadel in Winkel umrechnen
    int maxTacho = 200;
    float angleTacho = map(tempo, 0, maxTacho, -180, 0);
    float radTacho = angleTacho *  3.1416 / 180.0;

    //Koordinaten für die Temponatsnadel
    int nadelX = centerX + cos(radTacho) * (radius - 10);
    int nadelY = centerY + sin(radTacho) * (radius - 10);

    display.drawLine(centerX, centerY, nadelX, nadelY, WHITE);
    display.drawLine(centerX-1, centerY-1, nadelX-1, nadelY-1, WHITE);
    display.drawLine(centerX+1, centerY+1, nadelX+1, nadelY+1, WHITE);
  }

  // Textausgabe der Geschwindigkeit
  String dataStr = String(ms) + " m/s";
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(47, 0);         // Position des Textes
  display.print(dataStr.c_str());   // Text ausgeben

  // Textausgabe Temperatur
  display.cp437(true);
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(2, 0);         // Position des Textes
  display.print(temp);
  display.write(0xF8);
  display.print("C");

  // Textausgabe Neigung
  display.cp437(true);
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(110, 0);         // Position des Textes
  if(neig >= 100 || neig < 0) {display.setCursor(104, 0);}
  if(neig <= -100) {display.setCursor(98, 0);}
  // display.print("Neig.");
  display.print(neig);
  display.write(0xF8);

  // Skalenbeschriftung (0 links, 200 rechts)
  display.setCursor(1, 53);
  display.print("0");
  display.setCursor(110, 53);
  display.print("200");

  display.display(); // Anzeige aktualisieren
}
#endif
