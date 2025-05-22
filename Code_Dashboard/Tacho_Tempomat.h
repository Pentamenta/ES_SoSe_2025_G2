#ifndef Tacho_Tempomat_H
#define Tacho_Tempomat_H


//Der 0|0 Punkt auf dem Display ist links oben


//Software I2C Pininitialisierung (Eva) für mittleres Display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C mdisplay(U8G2_R0, /* clock=*/ 8, /* data=*/ 9, /* reset=*/ U8X8_PIN_NONE);

#define maxSpeed 200;  // Maximalgeschwindigkeit für Skala, 200 ist erstmal nur ein Beispielwert

//Variablen für Skala der Tachoanzeige (Eva)
#define centerX 64;    //X pixelwert des Mittelpunktes des Skalenkreis
#define centerY 60;    //Y pixelwert des Mittelpunktes des Skalenkreis
#define radius  40;     //Radius des Halbkreises

void writeTacho(uint16_t speed){
  display.clearBuffer();

  // Skala zeichnen (Halbkreis)
  for (int angle = -180; angle <= 0; angle += 15) { //geht einen Halbkreis in 15° Grad Schritten ab
    float rad = angle * 3.1416 / 180.0;             //Umrechnung Grad in Radiant
    int x1 = centerX + cos(rad) * (radius - 5);     //x-Anfangspunkt des Skala-Strichs beim aktuellen Winkel
    int y1 = centerY + sin(rad) * (radius - 5);     //y-Anfangspunkt des Skala-Strichs beim aktuellen Winkel
    int x2 = centerX + cos(rad) * radius;           //x-Endpunkt des Skala-Strichs beim aktuellen Winkel
    int y2 = centerY + sin(rad) * radius;           //y-Endpunkt des Skala-Strichs beim aktuellen Winkel
    display.drawLine(x1, y1, x2, y2);               //zeichnet eine Linie von (x1|y1) zu (x2|y2)   
  }

  //Werte zur Skala hinzufügen
  for (int angle = -180; angle <= 0; angle += 30) { //geht die Winkel in 30° Schritten durch, sodass bei jedem zweiten Strich ein Wert stehen wird
    float rad = angle * 3.1416 / 180.0;             //Umrechnung Grad in Radiant
    int x = centerX + cos(rad) * (radius + 2);      //x-Position berechnen
    int y = centerY + sin(rad) * (radius + 2);      //y-Position berechnen
    int value = map(angle, -180, 0, 0, maxSpeed);   //Umrechnung Winkel in Geschwindigkeit von 0 - maximale Geschwindigkeit
    display.setFont(u8g2_font_5x7_tr);              //Schriftgröße + Font bestimmen
    display.setCursor(x - 6, y);                    //setzt Anfangsposition für Wert, der per print() auf Display angezeigt wird
    display.print(value);                           //Wert auf Display an berechneter Position anzeigen
  }

  // Tachonadel zeichnen
  float angle = map(speed, 0, maxSpeed, -180, 0);       //anzuzeigende Geschwindigkeit in Winkel[Grad] umrechnen
  float rad = angle * 3.1416 / 180.0;                   //Umrechnung Grad in Winkel           
  int needleX = centerX + cos(rad) * (radius - 10);     //x-Endpunkt der Nadel berechnen
  int needleY = centerY + sin(rad) * (radius - 10);     //y-Endpunkt der Nadel berechnen
  display.drawLine(centerX, centerY, needleX, needleY); //Linie vom Mittelpunkt zum Endpunkt der Nadel zeichnen -> Tachonadel entsteht
}
#endif