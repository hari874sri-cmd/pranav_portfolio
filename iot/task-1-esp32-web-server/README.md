# Task 1: ESP32 Web Server & HTML LED Control

**IoT Station 01 of 05** | Embedded Local Web Architecture

## 📋 Overview
Direct embedded device control over a local Wi-Fi network using the ESP32 microcontroller as a standalone asynchronous HTTP web server serving a responsive web interface to toggle hardware GPIO states in real time.

- **Platform:** ESP32 Dual-Core Tensilica (DOIT ESP32 DevKit V1)
- **Protocol:** HTTP/1.1 (TCP Port 80)
- **Network Mode:** 802.11 b/g/n (Station Mode)
- **Actuator:** GPIO 2 (5mm Red Indicator LED + 330Ω Current Limiting Resistor)
- **Live Interactive Report:** [esp32-web-server.html](../esp32-web-server.html)

---

## ⚡ Circuit & Pin Connections
| ESP32 Pin | Component Lead | Signal Logic |
| :--- | :--- | :--- |
| `GPIO 2` | 330Ω Resistor → LED Anode (+) | HIGH (3.3V) = ON / LOW (0V) = OFF |
| `GND` | LED Cathode (-) Flat Edge | Common Ground (0V) |
| `Micro-USB / 5V` | Regulated Power Supply | 5V DC In to Onboard LDO |

---

## 🔌 API Endpoints Hosted on ESP32
| Endpoint | Method | Action | HTTP Response |
| :--- | :--- | :--- | :--- |
| `/` | `GET` | Serves responsive HTML5/CSS3 control dashboard | `200 OK` (text/html) |
| `/led/on` | `GET` | Drives GPIO 2 `HIGH`, sets state to ON | `303 See Other` -> `/` |
| `/led/off` | `GET` | Drives GPIO 2 `LOW`, sets state to OFF | `303 See Other` -> `/` |

---

## 🚀 How to Run
1. Open `esp32_web_server.ino` in the Arduino IDE (v2.x).
2. Install the **esp32 by Espressif Systems** board definitions in Boards Manager.
3. Replace `"YOUR_WIFI_SSID"` and `"YOUR_WIFI_PASSWORD"` with your 2.4GHz Wi-Fi credentials.
4. Select board **DOIT ESP32 DEVKIT V1** and COM port.
5. Upload sketch and open Serial Monitor at **115200 baud**.
6. Note the IP assigned (e.g., `192.168.1.184`) and visit `http://<IP>` in any browser on the same Wi-Fi.

