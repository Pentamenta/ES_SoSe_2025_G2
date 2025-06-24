#ifndef leftDisplay_H
#define leftDisplay_H

//Funktion zur Darstellung der Geschwindigkeit und des Tempomatenstatus auf dem linken Display (Eva)

void writedispl(float speed){
  if(tempo_on){
    ldisplay.clearBuffer();
    String speedStr = String(tempo_speed) + " m/s";         // Umwandlung von speed float Wert in String und Hinzufügen von " m/s"
    ldisplay.setFont(u8g2_font_ncenB10_tr);          // 18pt Schriftgröße
    ldisplay.drawStr(1, 20, speedStr.c_str());        // Anzeige der Zahl mit Text
    ldisplay.setFont(u8g2_font_ncenB08_tr);          // 18pt Schriftgröße
    ldisplay.drawStr(1, 40, "Tempomat");        // Anzeige der Zahl mit Text
    ldisplay.drawStr(20, 60, "AN");        // Anzeige der Zahl mit Text
    ldisplay.sendBuffer();                           // An Display senden
  

  } else{
      ldisplay.clearBuffer();
      String speedStr = String(speed) + " m/s";         // Umwandlung von speed float Wert in String und Hinzufügen von " m/s"
      ldisplay.setFont(u8g2_font_ncenB10_tr);          // 18pt Schriftgröße
      ldisplay.drawStr(1, 20, speedStr.c_str());        // Anzeige der Zahl mit Text
      ldisplay.setFont(u8g2_font_ncenB08_tr);          // 18pt Schriftgröße
      ldisplay.drawStr(1, 40, "Tempomat");        // Anzeige der Zahl mit Text
      ldisplay.drawStr(20, 60, "AUS");        // Anzeige der Zahl mit Text
      ldisplay.sendBuffer();                           // An Display senden
  }
}

#endif