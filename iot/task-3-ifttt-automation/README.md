# Task 3: Multi-Cloud IFTTT & Webhook Event Automation

**IoT Station 03 of 05** | Event-Driven Cross-Cloud Architecture

## 📋 Overview
Autonomous event-driven IoT control integrating conditional triggers (time-of-day, location, webhooks) via IFTTT (If This Then That) with Adafruit IO and the ESP32 microcontroller.

- **Platform:** ESP32 Dual-Core DevKit V1
- **Orchestrator:** IFTTT (Applets / Webhooks)
- **Broker:** Adafruit IO MQTT (`io.adafruit.com`)
- **Actuator:** GPIO 2 (Relay / LED load)
- **Live Interactive Report:** [ifttt-automation.html](../ifttt-automation.html)

---

## ⚡ Architecture Flow
```
[Event Source: Time/Webhook]
        │
        ▼
[IFTTT Applet Engine]
        │ (REST API POST)
        ▼
[Adafruit IO Cloud Broker]
        │ (MQTT Topic Publish)
        ▼
[ESP32 Edge Microcontroller] ──► GPIO 2 Actuation
```

---

## 🚀 How to Run
1. Configure `ifttt_automation.ino` with your Wi-Fi and Adafruit IO credentials.
2. Flash to ESP32 and ensure MQTT connection is established.
3. In IFTTT:
   - **IF THIS:** Date & Time (e.g., 6:00 PM daily) or Webhook (`smart_light_auto`).
   - **THEN THAT:** Send data to Adafruit IO feed `led-control` with value `1` or `0`.

