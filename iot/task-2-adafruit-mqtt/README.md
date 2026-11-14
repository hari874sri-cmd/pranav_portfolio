# Task 2: Adafruit IO & MQTT Cloud Control

**IoT Station 02 of 05** | Publish-Subscribe Cloud IoT Architecture

## 📋 Overview
Bi-directional IoT communication bridging local hardware actuators to the internet via the lightweight OASIS MQTT standard protocol and Adafruit IO cloud message broker.

- **Platform:** ESP32 Dual-Core (DOIT ESP32 DevKit V1)
- **Broker:** `io.adafruit.com` (Port 1883)
- **Protocol:** MQTT v3.1.1 (TCP Keep-Alive)
- **Topic:** `pranav222006/feeds/led-control`
- **Live Interactive Report:** [adafruit-mqtt.html](../adafruit-mqtt.html)

---

## ⚡ Circuit & Pin Connections
| ESP32 Pin | Component Lead | Signal Logic |
| :--- | :--- | :--- |
| `GPIO 2` | 330Ω Resistor → LED Anode (+) | HIGH = ON / LOW = OFF |
| `GND` | LED Cathode (-) Flat Edge | Common Ground (0V) |
| `Micro-USB / 5V` | Regulated Power Supply | 5V DC In |

---

## 🚀 How to Run
1. Install **Adafruit MQTT Library** in Arduino IDE Library Manager.
2. Open `adafruit_mqtt.ino`.
3. Set `WLAN_SSID`, `WLAN_PASS`, `AIO_USERNAME`, and `AIO_KEY`.
4. Upload to ESP32 and open Serial Monitor at **115200 baud**.
5. In Adafruit IO dashboard, create a toggle block connected to feed `led-control`. Toggling the button will instantly actuate GPIO 2 anywhere in the world.

