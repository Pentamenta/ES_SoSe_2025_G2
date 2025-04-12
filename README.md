# ES_SoSe_2025_G1
 Projekt zur Entwicklung eines automatisierten Fahrzeuges und eines Dashboards für den Kurs Embedded Systems 1 an der BHT Berlin.

## Workflow

Es wird mit Git gearbeitet. 
Vor jeder Arbeitsphase wird das Repository gepullt.
Gearbeitet wird nur in Branches, niemals direkt im Main. Es wird nur gemerged, wenn der Branch erfolgreich getestet wurde.
Vor dem commit wird ein weiteres mal gepullt, um Mergefehler durch paralleles Arbeiten zu vermeiden.

## Hardware

Als Basis für beide Projektteile wird ein Arduino Nano BLE Sense verwendet.
Weitere Arduinos können ins Projekt implementiert werden, der Nano BLE muss aber die Zentrale Anlaufstelle bleiben.
Um das Board zu bespielen:
 1. Unter Werkzeuge --> Board --> Boardverwalter wählen
 2. Nach "Arduino Mbed OS Nano Boards" suchen und installieren
 3. Unter Board erscheint nun die neue Option "Arduino Mbed OS Nano Boards"
 4. Dort "Arduino Nano 33 BLE" wählen

## Software

### Bibliotheken (Bitte erweitern)

Für das Projekt zu installierende Bibliotheken:
- ArduinoBLE

### Softwarestruktur

Im Repository befinden sich zwei Ordner mit darin jeweils gleichnamigen Arduino Programmen.
- Code_Car	  --> Code_Car.ino
- Code_ Dashboard --> Code_Dashboard.ino

In diesen Dateien wird programmiert. Ein Aufteilen in mehrere Dateien ist unter Arduino NICHT möglich.
Um den main loop jeder Datei möglichst sauber zu halten, ist so viel wie möglich über Funktionen zu regeln.

### BLE Documentation

Die Drahtlosverbindung zwischen Dashboard und Auto wird über Bluetooth Low Energy gelöst. 
Hierbei dient das Dashboard als Central Device und das Auto als Peripheral Device.

### Einführung BLE

Folgende Kommunikationen sind über BLE möglich:

- Reading: Das Peripherial Device ließt Daten vom Central Device aus
	- Dies wird in unserem Falle nur generelle Statusabfragen betreffen.
- Writing: Das Peripherial Device schreibt Daten andas Central Device.
	- Hiermit werden laufende Daten wie Sensor - Messwerte und Geschwindigkeit an das Dashboard übergeben.
- Notifying: Das Central Device schreibt Daten an ein Peripherial Device
	- Hiermit werden Anweisungen wie zu fahrende Geschwindigkeit und andere Aktionen an das Auto übergeben.

### Datenstruktur BLE im Projekt

Datenübertragung mit BLE funktioniert über sogenannte Service.
Ein Central Device stellt eine Reihe von klar Definierten Service zur verfügung. 
Das Peripherial Device, welches diesen Service kennt, kann mit dem Central Device eine Verbindung aufbauen.
Service bestehen aus so genannten Charakteristiken, welche im Endeffekt einzelne Variablen sind.
Eine Gruppe aus Service nennt man Profil.

Jeder Service besitzt eine einzigartige UUID. Entdeckt ein Peripheral Device einen Service kann dieses, die darin enthaltenen Charakteristiken, lesen, beschreiben, oder überwachen.
Jede Charakteristik besitzt ebenfalls eine einzigartige UUID.

## Fahrzeug

Eigenes Chassy oder eigenes Design?

Features:
- Blinker / Warnlicht
- Lenkung
	- Heckantrieb mit Servolenkung vorne?
	- Panzerkontrolle mit Mekanium?
- Abstandssensor mit Buzzer
- Geschwindigkeitssensor
- Tempomat

## Dashboard

- Akkustand
- Steuerung
	- Servolenkung mit Lenkrad und Pedal
	- Joystick bei Tank Controlls
- Tacho auf Bildschirm
- Taster für Blinker
- Soundausgabe 