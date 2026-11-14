#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "time.h"

// Wi-Fi Credentials
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// Firebase Project Credentials
#define API_KEY         "AIzaSyXXXXXXXXXXXXXXXXXXXXXXXXX"
#define DATABASE_URL    "https://smart-home-3309d-default-rtdb.firebaseio.com/"
#define USER_EMAIL      "user@smarthome.io"
#define USER_PASSWORD   "SecretPassword123"

// Hardware Pin Definitions
#define DHTPIN          4
#define DHTTYPE         DHT11
#define LDR_PIN         34
#define RELAY_PIN       18

// Sensor & Firebase Instances
DHT dht(DHTPIN, DHTTYPE);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Timing Control (Non-blocking intervals)
unsigned long lastSensorPublish = 0;
const unsigned long sensorInterval = 2500; // Sample every 2.5 seconds (respects DHT11)

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[Wi-Fi] Connected!");

  // Synchronize internal clock via NTP (prevents SSL token expiration)
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("[NTP] Clock synchronized");

  // Configure Firebase Credentials
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Initialize and authenticate
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("[Firebase] Initialized and authenticated!");
}

void loop() {
  // Periodically read sensors and publish to Firebase
  if (Firebase.ready() && (millis() - lastSensorPublish >= sensorInterval)) {
    lastSensorPublish = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int ldrRaw = analogRead(LDR_PIN);
    // Scale 12-bit ADC (0-4095) down to 0-1000 for intuitive dashboard viewing
    int lightLevel = map(ldrRaw, 0, 4095, 0, 1000);

    if (!isnan(temp) && !isnan(hum)) {
      Firebase.RTDB.setFloat(&fbdo, "/sensors/temperature", temp);
      Firebase.RTDB.setFloat(&fbdo, "/sensors/humidity", hum);
      Firebase.RTDB.setInt(&fbdo, "/sensors/light_level", lightLevel);
      Serial.printf("[TELEMETRY] Temp: %.1f C | Hum: %.1f %% | Light: %d\n", temp, hum, lightLevel);
    }
  }

  // Stream/Poll Relay Actuation State from Cloud
  if (Firebase.ready()) {
    if (Firebase.RTDB.getBool(&fbdo, "/controls/bulb_state")) {
      bool targetState = fbdo.boolData();
      digitalWrite(RELAY_PIN, targetState ? HIGH : LOW);
    }
  }
}

