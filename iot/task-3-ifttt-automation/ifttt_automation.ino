#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Wi-Fi Configuration
#define WLAN_SSID       "YOUR_WIFI_SSID"
#define WLAN_PASS       "YOUR_WIFI_PASSWORD"

// Adafruit IO Broker Credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "pranav222006"
#define AIO_KEY         "aio_XXXXXXXXXXXXXXXXXXXX"

// Actuator Pin (Relay or Indicator LED)
const int RELAY_PIN = 2;

// Networking & MQTT Client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Subscribe to feed manipulated by IFTTT Applet
Adafruit_MQTT_Subscribe ledFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led-control");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Default to Safe State (OFF)

  Serial.println(F("\n[INIT] Starting IFTTT Automated IoT Receiver..."));

  // Connect to Wi-Fi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("\n[Wi-Fi] Connected!"));

  // Subscribe to the Adafruit IO feed
  mqtt.subscribe(&ledFeed);
  Serial.println(F("[MQTT] Listening for IFTTT Automation Triggers..."));
}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  // Non-blocking subscription check (up to 4000ms wait window)
  while ((subscription = mqtt.readSubscription(4000))) {
    if (subscription == &ledFeed) {
      char *payload = (char *)ledFeed.lastread;
      Serial.print(F("[AUTOMATION TRIGGER RECEIVED] Feed Value: "));
      Serial.println(payload);

      // Evaluate Automated Trigger Command
      if (strcmp(payload, "1") == 0 || strcasecmp(payload, "ON") == 0) {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println(F("[ACTION EXECUTED] -> Appliance Activated (RELAY ON)"));
      } else if (strcmp(payload, "0") == 0 || strcasecmp(payload, "OFF") == 0) {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println(F("[ACTION EXECUTED] -> Appliance Deactivated (RELAY OFF)"));
      }
    }
  }

  // Keep-alive ping
  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) return;

  Serial.print(F("[MQTT] Re-establishing connection... "));
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) return;
  }
  Serial.println(F("Connected!"));
}

