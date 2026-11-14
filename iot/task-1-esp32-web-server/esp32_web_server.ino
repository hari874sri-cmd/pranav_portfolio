#include <WiFi.h>
#include <WebServer.h>

// Network Credentials
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Hardware Pin Definitions
const int LED_PIN = 2; // Onboard LED / External Circuit on GPIO 2
bool ledState = false;

// Create WebServer listening on standard HTTP TCP port 80
WebServer server(80);

// Generate Responsive HTML5 Dashboard
String generateHtml() {
  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 LED Control</title>";
  html += "<style>";
  html += "body{font-family:'Segoe UI',sans-serif;background:#0d1117;color:#c9d1d9;text-align:center;padding:50px 20px;}";
  html += ".card{max-width:420px;margin:auto;background:#161b22;border:1px solid #30363d;border-radius:16px;padding:36px;box-shadow:0 10px 30px rgba(0,0,0,0.5);}";
  html += "h1{color:#58a6ff;font-size:24px;margin-bottom:6px;}";
  html += ".status{font-size:16px;margin:24px 0;font-weight:600;}";
  html += ".btn{display:inline-block;width:100%;padding:14px;font-size:16px;font-weight:bold;border-radius:10px;text-decoration:none;transition:0.2s;}";
  html += ".btn-on{background:#238636;color:white;} .btn-on:hover{background:#2ea043;}";
  html += ".btn-off{background:#da3633;color:white;} .btn-off:hover{background:#f85149;}";
  html += "</style></head><body>";
  html += "<div class='card'>";
  html += "<h1>ESP32 Web Server</h1>";
  html += "<p style='color:#8b949e;font-size:13px;'>Direct Local Embedded Control</p>";
  html += "<div class='status'>Current State: <span style='color:" + String(ledState ? "#3fb950" : "#f85149") + "'>" + (ledState ? "ON" : "OFF") + "</span></div>";
  if (ledState) {
    html += "<a class='btn btn-off' href='/led/off'>TURN LED OFF</a>";
  } else {
    html += "<a class='btn btn-on' href='/led/on'>TURN LED ON</a>";
  }
  html += "</div></body></html>";
  return html;
}

// Route Handlers
void handleRoot() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.send(200, "text/html", generateHtml());
}

void handleLedOn() {
  ledState = true;
  digitalWrite(LED_PIN, HIGH);
  Serial.println("[HTTP] Route /led/on executed -> GPIO 2 HIGH");
  server.sendHeader("Location", "/");
  server.send(303); // Redirect back to root to refresh view
}

void handleLedOff() {
  ledState = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("[HTTP] Route /led/off executed -> GPIO 2 LOW");
  server.sendHeader("Location", "/");
  server.send(303); // Redirect back to root to refresh view
}

void handleFavicon() {
  server.send(204); // No content for browser favicon requests
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Endpoint Not Found on ESP32");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize Wi-Fi Connection
  Serial.print("Connecting to Wi-Fi SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[Wi-Fi] Connected successfully!");
  Serial.print("[Wi-Fi] Assigned IP Address: ");
  Serial.println(WiFi.localIP());

  // Register Web Server Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led/on", HTTP_GET, handleLedOn);
  server.on("/led/on/", HTTP_GET, handleLedOn);
  server.on("/led/off", HTTP_GET, handleLedOff);
  server.on("/led/off/", HTTP_GET, handleLedOff);
  server.on("/favicon.ico", HTTP_GET, handleFavicon);
  server.onNotFound(handleNotFound);

  // Start HTTP Daemon
  server.begin();
  Serial.println("[HTTP] WebServer started on port 80");
}

void loop() {
  // Listen and process incoming client connections
  server.handleClient();
}

