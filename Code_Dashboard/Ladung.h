#ifndef Ladung_H
#define Ladung_H

uint16_t load;

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

#endif