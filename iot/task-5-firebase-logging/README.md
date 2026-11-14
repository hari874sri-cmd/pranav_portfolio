# Task 5: Firebase Time-Series Data Logging & CSV Export

**IoT Station 05 of 05** | Historical Analytics & Autonomous Actuation

## 📋 Overview
Autonomous ambient-adaptive closed-loop lighting control combined with periodic non-volatile time-series cloud logging in Google Firebase and browser-side RFC 4180 CSV export.

- **Platform:** ESP32 Dual-Core DevKit V1
- **Database:** Google Firebase Realtime Database (`/logs`)
- **Control Strategy:** Closed-loop Hysteresis (Threshold = 450, Δ = 35)
- **Log Interval:** 30 Seconds Periodic Append via `pushJSON()`
- **Export Format:** RFC 4180 CSV via Web Blob API
- **Live Interactive Report:** [firebase-logging.html](../firebase-logging.html)

---

## ⚡ Circuit & Pin Matrix
| ESP32 Pin | Component | Signal / Role |
| :--- | :--- | :--- |
| `GPIO 4` | DHT11 Data Pin | Temperature & Relative Humidity |
| `GPIO 34` | LDR Voltage Divider | Ambient Light Intensity (ADC 0-4095) |
| `GPIO 18` | 5V Relay IN | Load / Lighting Actuator |
| `3V3 / 5V` | System Power | VCC Power Rail |
| `GND` | Ground | Common System Ground |

---

## 🚀 How to Run
1. Open `firebase_logging.ino` and configure Wi-Fi and Firebase credentials.
2. Upload to ESP32 and open Serial Monitor at **115200 baud**.
3. ESP32 synchronizes time via NTP, samples telemetry every 2.5s, executes hysteresis lighting, and commits time-series log entries every 30s to Firebase `/logs`.
4. Open the web dashboard to inspect historical trends and trigger CSV data download.

