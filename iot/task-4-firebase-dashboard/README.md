# Task 4: Real-Time Firebase IoT Cloud Dashboard

**IoT Station 04 of 05** | Production NoSQL Realtime Telemetry Architecture

## 📋 Overview
Direct embedded-to-cloud telemetry and remote actuation using Google Firebase Realtime Database (RTDB) and Web SDK with sub-second synchronization latency.

- **Platform:** ESP32 Dual-Core DevKit V1
- **Database:** Google Firebase Realtime Database (REST / WebSockets)
- **Sensors:** DHT11 (Temperature & Humidity) on GPIO 4, LDR (Ambient Light) on GPIO 34
- **Actuator:** 5V Relay / Indicator on GPIO 18
- **Live Production App:** [smart-home-3309d.web.app/#dashboard](https://smart-home-3309d.web.app/#dashboard)
- **Live Interactive Report:** [firebase-dashboard.html](../firebase-dashboard.html)

---

## ⚡ Circuit & Pin Matrix
| ESP32 Pin | Component Lead | Signal Function |
| :--- | :--- | :--- |
| `GPIO 4` | DHT11 Data Pin (with 10kΩ pull-up) | Digital 1-Wire Protocol (Temp + Humidity) |
| `GPIO 34` | LDR Voltage Divider Node | 12-bit ADC Analog Voltage Input (0-3.3V) |
| `GPIO 18` | Relay IN / Optocoupler Input | Digital Output: HIGH = Energized, LOW = De-energized |
| `3V3 / 5V` | Sensor & Relay VCC | Regulated DC Power Supply Rails |
| `GND` | Common Ground Reference | System Ground Bus |

---

## 🚀 How to Run
1. Install **Firebase ESP Client by Mobizt** via Arduino Library Manager.
2. Open `firebase_dashboard.ino` and fill in `WIFI_SSID`, `WIFI_PASSWORD`, `API_KEY`, and `DATABASE_URL`.
3. Upload to ESP32 and open Serial Monitor at **115200 baud**.
4. Open the Firebase web dashboard at `https://smart-home-3309d.web.app/#dashboard` to monitor real-time sensor streams and toggle loads.

