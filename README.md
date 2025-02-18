# PowerRing - PV-Visualisierung mit ESP8266 und LED-Ring für EVCC

## Beschreibung

Dieses Arduino-Projekt für den ESP8266-Mikrocontroller visualisiert Energiedaten einer Photovoltaikanlage auf einem 24 Bit NeoPixel LED-Ring. Es nutzt die von [EVCC](https://github.com/evcc-io/evcc) über die REST-API zur Verfügung gestellten Daten und stellt die PV-Leistung, den Hausverbrauch, den Netzbezug und den Ladestatus des Elektrofahrzeugs dar.
![grafik](https://github.com/user-attachments/assets/8217c266-0f09-45db-93c6-3c3f7e4cac4d)
![PXL_20250202_104355275](https://github.com/user-attachments/assets/f247cdca-fc6e-4127-9a42-6a3cc9124ebd)


## Funktionen

- Abruf von Energiedaten via EVCC REST-API
- Visualisierung auf 24-LED NeoPixel Ring
- Darstellung von PV-Produktion, Netzbezug/-einspeisung und Ladestatus anhand der Farbe und Anzahl der LEDs
- Sanftes Dimmen der LEDs bei Zustandsänderungen
- Fehleranzeige bei Verbindungsproblemen / Debug Infos über Serielle Schnittstelle

## Farbcodes auf dem LED Ring (analog zu EVCC)

- 🟢 Grün: Hausverbrauch der von der PV-Produktion gedeckt ist.
- 🟡 Gelb: in Netz eingespeiste überschüssige PV-Energie 
- 🔴 Rot: Netzbezug (in der EVCC GUI als grau ⚪ dargestellt)
- 🔵 Blau (Nur erste LED): Fahrzeug lädt

## Hardware-Anforderungen

- ESP8266 Mikrocontroller (Hier ESP01 aber auch z.B. NodeMCU, Wemos D1 Mini)
- WS2812B RGB LED-Ring (Anzahl der LEDs konfigurierbar)
- Stromversorgung für ESP8266 und LED-Ring:
  - z.B. AMS1117 DC-DC Step Down Buck Converter mit
  - Adafruit 1833 USB MICRO-B BREAKOUT BOARD

Hinweis von [Gelegenheitsbastler](https://github.com/Gelegenheitsbastler) (Danke!):
"Je nach verwendeten LEDs und Kabeln kann es sein, dass die Kommunikation zwischen dem ESP und dem LED-Stripe gar nicht oder nicht korrekt funktioniert. Das liegt dann daran, dass das Ausgangssignal des ESP 3V3 hat, der LED-Stripe aber einen Pegel von 5V erwartet. In solchen Fällen ist der Einsatz eines Pegelwandlers die Lösung. Da die Kommunikation nur in eine Richtung ESP --> LED statt findet, reicht ein BSS 138 völlig aus. Diese Level-Shifter gibt es für schmales Geld überall zu kaufen."

## Software-Anforderungen

- Arduino IDE
- ESP8266 Board-Unterstützung für Arduino IDE
- Folgende Bibliotheken:
  - ESP8266WiFi
  - ESP8266HTTPClient
  - ArduinoJson
  - Adafruit_NeoPixel (Achtung: Version 1.8.4)

## Installation

1. Laden Sie PowerRing.ino Datei runter.

2. Öffnen Sie die Arduino IDE und installieren Sie die ESP8266 Board-Unterstützung:
   - Fügen Sie `http://arduino.esp8266.com/stable/package_esp8266com_index.json` zu den zusätzlichen Boardverwalter-URLs in den Arduino-Einstellungen hinzu.
   - Installieren Sie das ESP8266-Paket über den Boardverwalter.

3. Installieren Sie die erforderlichen Bibliotheken über den Bibliotheksverwalter der Arduino IDE:
   - ESP8266WiFi
   - ESP8266HTTPClient
   - ArduinoJson
   - Adafruit_NeoPixel (Version 1.8.4)

      Achtung: Stellen Sie sicher, dass Sie die Adafruit_NeoPixel Bibliothek in der Version 1.8.4 installieren. Neuere Versionen sind nicht kompatibel mit dem ESP01. Um eine spezifische Version zu installieren:

      1. Öffnen Sie den Bibliotheksverwalter in der Arduino IDE (Sketch > Bibliothek einbinden > Bibliotheken verwalten...)
      2. Suchen Sie nach "Adafruit NeoPixel"
      3. Klicken Sie auf den Dropdown-Pfeil neben der Versionsnummer
      4. Wählen Sie Version 1.8.4 aus und klicken Sie auf "Installieren"

4. Öffnen Sie die `PowerRing.ino` Datei in der Arduino IDE.

5. Konfigurieren Sie die folgenden Variablen in der Datei:
   - WLAN-Zugangsdaten (`ssid` und `password`)
   - RestAPI-Adresse (`api_endpoint`)
   - LED-Pin und Anzahl der LEDs (`LED_PIN` und `NUM_LEDS`)
   - Die Leistung in Watt welche eine LED darstellt (`WATTS_PER_LED`)

6. Wählen Sie das korrekte ESP8266-Board und den COM-Port in der Arduino IDE aus.
  - Für ESP01s: "Generic ESP8266 Module"
  - Flash Size: "1M (no SPIFFS)" 

8. Sketch kompilieren und über seriellen Adapter aus den ESP8266 flashen.

## EVCC REST-API
Details zur verwendeten API: [EVCC REST-API Dokumentation](https://docs.evcc.io/docs/integrations/rest-api)

## Verkabelung

- Verbinden Sie den Daten-Pin (DIN) des LED-Rings mit dem in `LED_PIN` definierten Pin des ESP8266 (z.B. GPIO0 a.k.a. D3 = 0). --> Siehe Schematic weiter unten
- Stellen Sie sicher, dass der LED-Ring und der ESP8266 eine gemeinsame Masse haben.
- Versorgen Sie den LED-Ring und den ESP8266 mit ausreichend Strom. (z.B. passendes USB-Ladegerät)

Bei Verbindungsproblemen blinkt die erste LED orange.

## Fehlerbehebung

- Überprüfen Sie die serielle Konsole für Debug-Informationen.
- Orange blinkende LED: API-Verbindungsproblem
- Überprüfen Sie die Verkabelung des LED-Rings.

***

## Meine Referenz Hardware:

- 24Bit RGB LED Ring WS2812
- ESP-01S
- AMS1117 DC-DC Step Down Buck Converter 3.3 V BREAKOUT Board
- EVCC running on e.g. RaspberryPi or Synology Docker
- Adafruit 1833 USB MICRO-B BREAKOUT BOARD
- IKEA "Bergenes Halter für Mobiltelefon/Tablet" mit zwei Plexiglasscheiben

### Schematic:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/c3c2fa3b-7bf5-470b-b107-2ba651add660)

### Board:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/60877ca7-3e40-4be3-86e5-47ae1d2307df)
