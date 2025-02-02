# PowerRing - PV-Visualisierung mit ESP8266 und LED-Ring für EVCC

## Beschreibung

PowerRing ist ein Arduino-Projekt für den ESP8266, das einen RGB LED-Ring zur Visualisierung von Photovoltaik-Daten verwendet. Es empfängt MQTT-Nachrichten von einem EVCC-System und stellt die PV-Leistung, den Hausverbrauch, den Netzbezug und den Ladestatus eines Elektrofahrzeugs dar.

## Funktionen

- Visualisierung von PV-Leistung, Hausverbrauch und Netzbezug auf einem LED-Ring
- Anzeige des EV-Ladestatus
- MQTT-Integration für Datenaktualisierung
- Sanftes Dimmen der LEDs bei Zustandsänderungen
- Fehleranzeige bei Verbindungsproblemen

## Hardware-Anforderungen

- ESP8266 Mikrocontroller (Hier ESP01 aber z.B. NodeMCU, Wemos D1 Mini)
- WS2812B RGB LED-Ring (Anzahl der LEDs konfigurierbar)
- Stromversorgung für ESP8266 und LED-Ring

## Software-Anforderungen

- Arduino IDE
- ESP8266 Board-Unterstützung für Arduino IDE
- Folgende Bibliotheken:
  - ESP8266WiFi
  - PubSubClient
  - Adafruit_NeoPixel (Version 1.8.4)

## Installation

1. Klonen Sie dieses Repository oder laden Sie den Quellcode herunter.

2. Öffnen Sie die Arduino IDE und installieren Sie die ESP8266 Board-Unterstützung:
   - Fügen Sie `http://arduino.esp8266.com/stable/package_esp8266com_index.json` zu den zusätzlichen Boardverwalter-URLs in den Arduino-Einstellungen hinzu.
   - Installieren Sie das ESP8266-Paket über den Boardverwalter.

3. Installieren Sie die erforderlichen Bibliotheken über den Bibliotheksverwalter der Arduino IDE:
   - PubSubClient
   - Adafruit_NeoPixel (Version 1.8.4)

Wichtiger Hinweis: Stellen Sie sicher, dass Sie die Adafruit_NeoPixel Bibliothek in der Version 1.8.4 installieren. Neuere Versionen sind nicht kompatibel mit dem ESP01. Um eine spezifische Version zu installieren:

1. Öffnen Sie den Bibliotheksverwalter in der Arduino IDE (Sketch > Bibliothek einbinden > Bibliotheken verwalten...)
2. Suchen Sie nach "Adafruit NeoPixel"
3. Klicken Sie auf den Dropdown-Pfeil neben der Versionsnummer
4. Wählen Sie Version 1.8.4 aus und klicken Sie auf "Installieren"

4. Öffnen Sie die `PowerRing.ino` Datei in der Arduino IDE.

5. Konfigurieren Sie die folgenden Variablen in der Datei:
   - WLAN-Zugangsdaten (`ssid` und `password`)
   - MQTT-Broker-Adresse (`mqtt_server`)
   - LED-Pin und Anzahl der LEDs (`LED_PIN` und `NUM_LEDS`)
   - MQTT-Topics für die verschiedenen Messwerte

6. Wählen Sie das korrekte ESP8266-Board und den COM-Port in der Arduino IDE aus.

7. Kompilieren und hochladen Sie den Code auf Ihren ESP8266.

## Verkabelung

- Verbinden Sie den Daten-Pin des LED-Rings mit dem in `LED_PIN` definierten Pin des ESP8266 (Standard: D4).
- Stellen Sie sicher, dass der LED-Ring und der ESP8266 eine gemeinsame Masse haben.
- Versorgen Sie den LED-Ring und den ESP8266 mit ausreichend Strom.

## Verwendung

Nach dem Hochladen des Codes und der korrekten Verkabelung wird der ESP8266 versuchen, sich mit dem konfigurierten WLAN und MQTT-Broker zu verbinden. Die LEDs zeigen dann die empfangenen PV-Daten wie folgt an:

- Grün: Hausverbrauch aus PV-Leistung
- Gelb: Überschüssige PV-Leistung
- Rot: Netzbezug
- Blau (erste LED): EV lädt

Bei Verbindungsproblemen blinkt die erste LED orange.

## Fehlerbehebung

- Überprüfen Sie die serielle Konsole für Debug-Informationen.
- Stellen Sie sicher, dass die WLAN- und MQTT-Broker-Einstellungen korrekt sind.
- Überprüfen Sie die Verkabelung des LED-Rings.

***

## Referenz Hardware:

- 24Bit RGB LED Ring WS2812
- ESP-01S (or any other board running ESP Easy FW)
- AMS1117 DC-DC Step Down Buck Converter 3.3 V BREAKOUT Board
- EVCC running on e.g. RaspberryPi with MQTT configured
- (optional) Adafruit 1833 USB MICRO-B BREAKOUT BOARD

### Schematic:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/c3c2fa3b-7bf5-470b-b107-2ba651add660)

### Board:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/60877ca7-3e40-4be3-86e5-47ae1d2307df)

Soldered board example (ESP01S + AMS1117)

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/bb434075-1779-4083-a693-986484157e78)

First breadboard setup with ESP12 and the 24 LED ring in action:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/9307b9b5-e34e-4c3a-8112-3f387d36da28)


