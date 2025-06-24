# ES_SoSe_2025_G1
 Projekt zur Entwicklung eines automatisierten Fahrzeuges und eines Dashboards für den Kurs Embedded Systems 1 an der BHT Berlin.

 Dozent: Prof. Dr.-Ing. Ronny Jahnke

## Workflow

### Git

Es wird mit Git gearbeitet.  
Vor jeder Arbeitsphase wird das Repository gepullt.
Gearbeitet wird nur in Branches, niemals direkt im Main. Es wird nur gemerged, wenn der Branch erfolgreich getestet wurde.
Vor dem Commit wird ein weiteres mal gepullt, um Mergefehler durch paralleles Arbeiten zu vermeiden.

## Hardware

Als Basis für beide Projektteile wird ein Arduino Nano BLE 33 verwendet. Das Auto verfügt zusätzlich über einen Arduino MEGA, da weitere Pins für die Steuerung der IOs benötigt wurden. Der Arduino Nano BLE 33 bleicht zentrale Anlaufstelle für die Drahtloskommunikation.

Um das Board Arduino Nano BLE 33 zu bespielen:
 1. Unter Werkzeuge --> Board --> Boardverwalter wählen
 2. Nach "Arduino Mbed OS Nano Boards" suchen und installieren
 3. Unter Board erscheint nun die neue Option "Arduino Mbed OS Nano Boards"
 4. Dort "Arduino Nano 33 BLE" wählen

## Software

### Bibliotheken (Fortlaufend zu erweitern)

Für das Projekt zu installierende Bibliotheken:
- ArduinoBLE
- U8g2 von olikraus (beim Inkludieren werden 3 Bibliotheken eingefügt, wir brauchen nur die U8g2lib)
- Arduino_BMI270_BMM150
- Adafruit_SSD1306
- Adafruit_GFX
- Wire
- Servo

### Softwarestruktur

Im Repository befinden sich zwei Ordner mit darin jeweils gleichnamigen Arduino Programmen.
- Code_ Dashboard --> Code_Dashboard.ino
- Code_Car_Connect --> Code_Car_Connect.ino
- Code_Car --> Code_Car.ino

In diesen Dateien wird programmiert. Ein Aufteilen in ausgelagerte .h Dateien ist erwünscht.  

In der externen Datei EBS_BLE.h werden Variablen für die BLE Kommunikation definiert, die in allen Programmen gleich seien müssen. Aufgrund von beschränkungen der Arduino IDE kann diese Datei nicht extern verlinkt werden, sondern muss in jedem Programmordner einzeln existieren. Es ist extrem wichtig, dass diese Dateien immer auf dem selben Stand sind.

### Coding

- Es ist soviel wir möglich über Funktionen zu lösen, um den Main loop so übersichtlich wie möglich zu halten.
- Delays und längere Loops sind zu vermeiden. Das Programm muss den Loop ständig durchlaufen, um die Drahtloskpommunikation nicht zu behindern.
- Der Code ist ausführlich zu kommentieren und am Ende der Kommentare der Name des bearbeiters hinzuzufügen, um Rückfragen zu vereinfachen. Es Reicht wenn der Namen einmal zu Begin einer Funktion notiert wird.

## BLE Documentation

Die Drahtlosverbindung zwischen Dashboard und Auto wird über Bluetooth Low Energy gelöst. 
Hierbei dient das Dashboard als Central Device und das Auto als Peripheral Device.

### Einführung BLE

- Das Peripherial Device ließt Daten vom Central Device aus
	- Darunter fallen sämtliche Anweisungen an das Auto, wie zu erreichende Geschwindigkeit und Lenkeinschlag
- Das Peripherial Device schreibt Daten andas Central Device.
	- Hiermit werden laufende Daten wie Sensor - Messwerte und Geschwindigkeit an das Dashboard übergeben.

### Datenstruktur BLE im Projekt

Datenübertragung mit BLE funktioniert über sogenannte Service.
Ein Central Device stellt eine Reihe von klar Definierten Service zur verfügung. 
Das Peripherial Device, welches diesen Service kennt, kann mit dem Central Device eine Verbindung aufbauen.
Service bestehen aus so genannten Charakteristiken, welche im Endeffekt einzelne Variablen sind.
Eine Gruppe aus Service nennt man Profil.

Jeder Service besitzt eine einzigartige UUID. Entdeckt ein Peripheral Device einen Service kann dieses, die darin enthaltenen Charakteristiken, lesen, beschreiben, oder überwachen.
Jede Charakteristik besitzt ebenfalls eine einzigartige UUID.

Die Uuids un dazugehörigen Charakteristika werden in der Datei "EBS_BLE.h" deklariert. Mittels bedigter Kompilierung werden Funktionen an den jeweiligen Code angepasst.

Die Tags für die bedingte Programmierung der einzelnen Programme lauten:
- Code_Dashboard.ino --> DASH
- Code_Car_Connect.ino --> CAR_CONNECT
- Code_Car.ino --> CAR

## Features

### Fahrzeug

- Blinker / Warnlicht
- Lenkung
	- Heckantrieb mit Servolenkung vorne
- Abstandssensor mit Buzzer
- Geschwindigkeitssensor
- Tempomat

### Dashboard

- Steuerung
	- Servolenkung über Joystick
	- Gas/Bremse über Joystick
- Tacho auf Bildschirm 
- Taster für Blinker, Tempomat, Scheinwerfer
- Bildschirme (1-3): Tachoanzeige, Parkanzeige, Tempomat Geschwindigkeit, Lüfter, Heizung