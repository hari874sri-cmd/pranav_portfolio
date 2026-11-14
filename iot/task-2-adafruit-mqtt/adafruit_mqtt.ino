#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Wi-Fi Configuration
#define WLAN_SSID       "YOUR_WIFI_SSID"
#define WLAN_PASS       "YOUR_WIFI_PASSWORD"

// Adafruit IO Broker Configuration
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // Unencrypted MQTT port
#define AIO_USERNAME    "pranav222006"         // Your Adafruit IO Username
#define AIO_KEY         "aio_XXXXXXXXXXXXXXXXXXXX" // 32-character AIO secret key

// Hardware Pins
const int LED_PIN = 2;

// Initialize Wi-Fi & MQTT Client Instances
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup Feed for Subscription
// Topic structure: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe ledFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led-control");

// Forward Declaration for Watchdog Reconnection
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println(F("\n======================================"));
  Serial.println(F("ESP32 Adafruit IO MQTT Client Starting"));
  Serial.println(F("======================================"));

  // Connect to Wi-Fi
  Serial.print(F("Connecting to Wi-Fi SSID: "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("\n[Wi-Fi] Connected! IP: "));
  Serial.println(WiFi.localIP());

  // Register MQTT Subscriptions
  mqtt.subscribe(&ledFeed);
  Serial.println(F("[MQTT] Subscribed to topic: pranav222006/feeds/led-control"));
}

void loop() {
  // Ensure the connection to the MQTT server is alive
  MQTT_connect();

  // Read subscription packets with a non-blocking timeout of 5000ms
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &ledFeed) {
      char *payload = (char *)ledFeed.lastread;
      Serial.print(F("[MQTT INCOMING] Feed 'led-control': "));
      Serial.println(payload);

      // Actuate Hardware based on received payload string
      if (strcmp(payload, "1") == 0 || strcasecmp(payload, "ON") == 0) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println(F("[ACTUATION] -> GPIO 2 driven HIGH (LED ON)"));
      } else if (strcmp(payload, "0") == 0 || strcasecmp(payload, "OFF") == 0) {
        digitalWrite(LED_PIN, LOW);
        Serial.println(F("[ACTUATION] -> GPIO 2 driven LOW (LED OFF)"));
      }
    }
  }

  // Ping the broker to maintain the active TCP keep-alive session
  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print(F("[MQTT] Connecting to Adafruit IO Broker... "));

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect() returns 0 on success
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println(F("[MQTT] Retrying connection in 5 seconds..."));
    mqtt.disconnect();
    delay(5000);  // Wait 5 seconds before retrying
    retries--;
    if (retries == 0) {
      Serial.println(F("[MQTT ERROR] Critical connection failure. Resetting socket..."));
      return;
    }
  }
  Serial.println(F("[MQTT] Successfully Connected to Adafruit IO!"));
}

