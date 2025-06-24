#ifndef Abstandsmessung_H
#define Abstandsmessung_H

 
// Funktion Abstandssensor
void abstand(int f, int fl, int fr, int b, int bl, int br, int p_l, int p_r, int l_on, int lw_on, int h, int luft) {
  display.clearDisplay();  // Anzeige löschen

  //Ausgabe Viereck
  display.drawRect(29, 20, 70, 23, WHITE);
  //Ausgabe Viereck als Windschutzscheibe
  display.drawRect(72, 23, 17, 17, WHITE);
  //Ausgabe Vierech als Heckscheibe
  display.drawRect(37, 23, 10, 17, WHITE);


  //Ausgabe 6 Linien für die Abstandsmessung
  // Linie front
  display.drawLine(99+4*f, 20, 99+4*f, 43, WHITE);
  // Linie back
  display.drawLine(29-4*b, 20, 29-4*b, 43, WHITE);
  // Linie front left
  display.drawLine(97, 22-4*fl, 66, 22-4*fl, WHITE); 
  // Linie back left
  display.drawLine(62, 22-4*bl, 31, 22-4*bl, WHITE); 
  // Linie front right
  display.drawLine(97, 43+4*fr, 66, 43+4*fr, WHITE); 
  //Linie back right
  display.drawLine(62, 43+4*br, 31, 43+4*br, WHITE); 

  // Parken links möglich
  if (p_l == 1){
  display.setRotation(1);
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(2, 0);         // Position des Textes
  display.print("P");
  display.setRotation(0);
  }

  // Parken rechts möglich
  if (p_l == 1){
  display.setRotation(1);
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(58, 0);         // Position des Textes
  display.print("P");
  display.setRotation(0);
  }

  // licht an 
  if (l_on == 1){
  display.drawCircle( 99, 25, 4, WHITE);
  display.drawCircle( 99, 37, 4, WHITE);
  }

  // Fernlicht an
  if (lw_on == 1){
  display.fillCircle( 99, 25, 4, WHITE);
  display.fillCircle( 99, 37, 4, WHITE);
  }

  // wenn es heiß ist
  if (h == 1){
  display.drawLine(2, 3, 2, 10, WHITE);
  display.cp437(true);
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(4, 2);         // Position des Textes
  display.write(0xF7);
  display.setCursor(4, 5);         // Position des Textes
  display.write(0xF7);
  }

  // wenn Lüfter an
  if (luft == 1){
  display.drawCircle(5, 58, 4, WHITE);
  display.cp437(true);
  display.setTextSize(1);           // Textgröße
  display.setTextColor(WHITE);      // Textfarbe
  display.setCursor(3, 55);         // Position des Textes
  display.write(0x2A);
  }

  display.display(); // Anzeige aktualisieren
}
#endif