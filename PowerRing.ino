// Arduino IDE setting For ESP01s:
// Board: "Generic ESP8266 Module"
// Flash Size: "1M (no SPIFFS)"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h> // Use version 1.8.4 as this is compatible with ESP01s module

// WiFi-Einstellungen
const char* ssid = "YOUR_WLAN_ID";
const char* password = "YOUR_WLAN_PASSWORD";

// MQTT-Broker-Einstellungen
const char* mqtt_server = "192.168.178.xx";
const int mqtt_port = 1883;

// LED-Ring-Einstellungen
const int LED_PIN = 0; // GPIO-0
const int NUM_LEDS = 24;
const int WATTS_PER_LED = 400; // An die maximale PV Leistung anpassen

// MQTT-Topics
const char* TOPIC_PV_POWER = "evcc/site/pvPower";
const char* TOPIC_GRID_POWER = "evcc/site/gridPower";
const char* TOPIC_HOME_POWER = "evcc/site/homePower";
const char* TOPIC_CHARGE_POWER = "evcc/loadpoints/1/chargePower";
bool isConnected = false;

// Globale Variablen für Messwerte
float pvPower = 0, gridPower = 0, homePower = 0, chargePower = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastUpdateTime = 0;
bool connectionLost = false;

// Struktur für LED-Farben
struct Color {
  uint8_t r, g, b;
  Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {}
};

// Array für aktuelle und Ziel-LED-Farben
Color currentColors[NUM_LEDS];
Color targetColors[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  strip.begin();
  strip.show();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Verbinde mit ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi verbunden");
  Serial.println("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  float value = message.toFloat();

  if (strcmp(topic, TOPIC_PV_POWER) == 0) pvPower = value;
  else if (strcmp(topic, TOPIC_GRID_POWER) == 0) gridPower = value;
  else if (strcmp(topic, TOPIC_HOME_POWER) == 0) homePower = value;
  else if (strcmp(topic, TOPIC_CHARGE_POWER) == 0) chargePower = value;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Verbinde mit MQTT-Broker...");
    if (client.connect("ESP8266Client")) {
      Serial.println("verbunden");
      client.subscribe(TOPIC_PV_POWER);
      client.subscribe(TOPIC_GRID_POWER);
      client.subscribe(TOPIC_HOME_POWER);
      client.subscribe(TOPIC_CHARGE_POWER);
      connectionLost = false;
    } else {
      Serial.print("Fehlgeschlagen, rc=");
      Serial.print(client.state());
      Serial.println(" Versuche erneut in 5 Sekunden");
      connectionLost = true;
      delay(5000);
    }
  }
}

void updateLEDs() {
  // Ziel-Farben basierend auf den aktuellen Werten setzen
  setTargetColors();

  // Dimmen über 1 Sekunde
  for (int step = 0; step <= 100; step++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      Color c;
      c.r = currentColors[i].r + (targetColors[i].r - currentColors[i].r) * step / 100;
      c.g = currentColors[i].g + (targetColors[i].g - currentColors[i].g) * step / 100;
      c.b = currentColors[i].b + (targetColors[i].b - currentColors[i].b) * step / 100;
      strip.setPixelColor(i, strip.Color(c.r, c.g, c.b));
    }
    strip.show();
    delay(10); // 100 Schritte in 1 Sekunde
  }

  // Aktuelle Farben auf Zielfarben setzen
  for (int i = 0; i < NUM_LEDS; i++) {
    currentColors[i] = targetColors[i];
  }
}

void setTargetColors() {
  for (int i = 0; i < NUM_LEDS; i++) {
    targetColors[i] = Color(0, 0, 0); // Alle LEDs zunächst ausschalten
  }

  if (pvPower > (homePower + chargePower)) {
    // Grüne LEDs für Hausverbrauch
    int ledsToLight = min(NUM_LEDS, int(round((homePower + chargePower) / WATTS_PER_LED)));
    Serial.print("ledsToLight: ");
    // Serial.println(homePower / WATTS_PER_LED);
    // Serial.println(round(homePower / WATTS_PER_LED));
    Serial.println(ledsToLight);

    for (int i = 0; i < ledsToLight; i++) {
      targetColors[i] = Color(0, 125, 0);
    }
    // Gelbe LEDs für überschüssige PV-Leistung
    int surplusLeds = min(NUM_LEDS - ledsToLight, int(round((pvPower - homePower - chargePower) / WATTS_PER_LED)));
    Serial.print("surplusLeds: ");
    // Serial.println((pvPower - homePower) / WATTS_PER_LED);
    // Serial.println(round((pvPower - homePower) / WATTS_PER_LED));
    Serial.println(surplusLeds);
    for (int i = ledsToLight; i < ledsToLight + surplusLeds; i++) {
      targetColors[i] = Color(125, 125, 0);
    }
  } else {
    // Grüne LEDs für PV-Leistung
    int pvLeds = min(NUM_LEDS, int(round(pvPower / WATTS_PER_LED)));
    Serial.print("pvLeds: ");
    // Serial.println(pvPower / WATTS_PER_LED);
    // Serial.println(round(pvPower / WATTS_PER_LED));
    Serial.println(pvLeds);
    for (int i = 0; i < pvLeds; i++) {
      targetColors[i] = Color(0, 125, 0);
    }
    // Rote LEDs für Netzstrombezug
    int gridLeds = min(NUM_LEDS - pvLeds, int(round(gridPower / WATTS_PER_LED)));
    Serial.print("gridLeds: ");
    // Serial.println(gridPower / WATTS_PER_LED);
    // Serial.println(round(gridPower / WATTS_PER_LED));
    Serial.println(gridLeds);
    for (int i = pvLeds; i < pvLeds + gridLeds; i++) {
      targetColors[i] = Color(125, 0, 0);
    }
  }
  // Erste LED blau, wenn Fahrzeug lädt
  if (chargePower > 0) {
    targetColors[0] = Color(0, 0, 125);
  }
  // Debugging print outs
  Serial.println("");
  Serial.println("gridPower: ");
  Serial.println(gridPower);
  Serial.println("pvPower: ");
  Serial.println(pvPower);
  Serial.println("homePower: ");
  Serial.println(homePower);
  Serial.println("");

  for (int i = 0; i < NUM_LEDS; i++) {
    Serial.print(i);
    Serial.print(" R: ");
    Serial.print(targetColors[i].r);
    Serial.print(" G: ");
    Serial.print(targetColors[i].g);
    Serial.print(" B: ");
    Serial.print(targetColors[i].b);
    Serial.println("");
  }

}

void loop() {
  if (!client.connected()) {
    Serial.print("Reconnecting ...");
    reconnect();
  }
  client.loop();

  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= 2000) {
    updateLEDs();
    lastUpdateTime = currentTime;
  }

  if (connectionLost) {
    // Erste LED orange blinken lassen
    if ((currentTime / 1000) % 2 == 0) {
      strip.setPixelColor(0, strip.Color(125, 80, 0));
    } else {
      strip.setPixelColor(0, strip.Color(0, 0, 0));
    }
    strip.show();
  }
}
