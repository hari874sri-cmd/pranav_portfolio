#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "time.h"

// Network & Firebase Credentials
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"
#define API_KEY         "AIzaSyXXXXXXXXXXXXXXXXXXXXXXXXX"
#define DATABASE_URL    "https://smart-home-3309d-default-rtdb.firebaseio.com/"
#define USER_EMAIL      "user@smarthome.io"
#define USER_PASSWORD   "SecretPassword123"

// Pin Assignments
#define DHTPIN          4
#define DHTTYPE         DHT11
#define LDR_PIN         34
#define RELAY_PIN       18

// Instances
DHT dht(DHTPIN, DHTTYPE);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Automation & Logging Parameters
bool manualMode = false;       // false = AUTOMATIC, true = MANUAL
bool currentBulbState = false;
int ldrThreshold = 450;        // Calibrated ambient light cutoff
const int HYSTERESIS_DELTA = 35; // Deadband to prevent relay chatter

unsigned long lastSensorPublish = 0;
unsigned long lastLogAppend = 0;
const unsigned long SENSOR_INTERVAL = 2500;  // 2.5s live dashboard update
const unsigned long LOG_INTERVAL    = 30000; // 30s historical log append

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[Wi-Fi] Connected!");

  // Synchronize system clock via NTP
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");

  // Firebase Setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (!Firebase.ready()) return;

  // 1. Fetch Mode & Threshold Settings from Cloud
  if (Firebase.RTDB.getBool(&fbdo, "/controls/manual_mode")) {
    manualMode = fbdo.boolData();
  }
  if (Firebase.RTDB.getInt(&fbdo, "/controls/ldr_threshold")) {
    ldrThreshold = fbdo.intData();
  }

  // 2. Read Sensors
  int rawLdr = analogRead(LDR_PIN);
  int lightLevel = map(rawLdr, 0, 4095, 0, 1000); // 0 = Pitch Dark, 1000 = Direct Sun

  // 3. Automation Decision Engine
  if (manualMode) {
    // MANUAL MODE: Obey user toggle switch directly from cloud
    if (Firebase.RTDB.getBool(&fbdo, "/controls/bulb_state")) {
      currentBulbState = fbdo.boolData();
    }
  } else {
    // AUTOMATIC MODE: Closed-loop Hysteresis Lighting
    // If ambient light drops below lower threshold -> Turn ON
    if (lightLevel < (ldrThreshold - HYSTERESIS_DELTA) && !currentBulbState) {
      currentBulbState = true;
      Firebase.RTDB.setBool(&fbdo, "/controls/bulb_state", true);
      Serial.println("[AUTO ACTION] Dusk detected -> Relay ON");
    }
    // If ambient light rises above upper threshold -> Turn OFF
    else if (lightLevel > (ldrThreshold + HYSTERESIS_DELTA) && currentBulbState) {
      currentBulbState = false;
      Firebase.RTDB.setBool(&fbdo, "/controls/bulb_state", false);
      Serial.println("[AUTO ACTION] Daylight restored -> Relay OFF");
    }
  }

  // Actuate Hardware Relay
  digitalWrite(RELAY_PIN, currentBulbState ? HIGH : LOW);

  // 4. Update Live Telemetry Cards (Every 2.5 seconds)
  if (millis() - lastSensorPublish >= SENSOR_INTERVAL) {
    lastSensorPublish = millis();
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      Firebase.RTDB.setFloat(&fbdo, "/sensors/temperature", temp);
      Firebase.RTDB.setFloat(&fbdo, "/sensors/humidity", hum);
      Firebase.RTDB.setInt(&fbdo, "/sensors/light_level", lightLevel);
    }
  }

  // 5. Append Historical Time-Series Log (Every 30 seconds)
  if (millis() - lastLogAppend >= LOG_INTERVAL) {
    lastLogAppend = millis();
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      FirebaseJson json;
      json.set("epoch", (int)time(nullptr));
      json.set("temperature", temp);
      json.set("humidity", hum);
      json.set("light_level", lightLevel);
      json.set("bulb_state", currentBulbState);
      json.set("mode", manualMode ? "MANUAL" : "AUTO");

      // Push creates a unique chronologically sorted key
      Firebase.RTDB.pushJSON(&fbdo, "/logs", &json);
      Serial.println("[LOG] Historical snapshot written to Firebase /logs");
    }
  }
}

