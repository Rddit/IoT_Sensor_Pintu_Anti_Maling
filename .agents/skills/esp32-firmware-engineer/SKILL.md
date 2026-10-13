---
name: esp32-firmware-engineer
description: >-
  Use this skill when designing, compiling, configuring, and optimizing ESP32 firmware
  for the IoT Door Security Alarm project in Arduino IDE or PlatformIO.
---

# ESP32 Firmware Engineer

Specialized guide for developing robust, production-grade embedded firmware on the ESP32 (ESP-WROOM-32 DevKit V1) for security alarm systems.

## Recommended Pin Selection Guide for ESP32 DevKit

When wiring on the ESP32 baseboard, always avoid strapping pins and ADC2 conflict pins when WiFi is active:

| Component | Default Recommended GPIO | Notes |
| :--- | :--- | :--- |
| **Magnetic Sensor (MC-38)** | **GPIO 4** | Supports internal pullup (`INPUT_PULLUP`), wake-from-sleep capable |
| **Buzzer Module** | **GPIO 18** | Standard digital output, safe at boot |
| **LED Indicator** | **GPIO 19** | Standard digital output with current-limiting resistor |
| *Alternative Pin for Sensor* | GPIO 14 / GPIO 27 | Safe digital input pins |
| *Pins to AVOID* | GPIO 0, 2, 12, 15 (Strapping pins), GPIO 6-11 (Flash memory), GPIO 34-39 (Input-only, no internal pull-up) |

## Core Implementation Patterns

### 1. Reed Switch Debouncing & State Detection
Magnetic reed switches bounce mechanically on movement. Always implement software debounce or state-change detection:
```cpp
const int SENSOR_PIN = 4;
int lastSensorState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(SENSOR_PIN, INPUT_PULLUP);
}
```

### 2. Non-Blocking Alarm Tone & Blinking
Avoid using `delay()`, which halts WiFi and cloud keep-alive packets:
```cpp
unsigned long lastBeepTime = 0;
bool buzzerState = false;

void handleAlarm() {
  if (millis() - lastBeepTime >= 200) {
    lastBeepTime = millis();
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
    digitalWrite(LED_PIN, buzzerState ? HIGH : LOW);
  }
}
```

### 3. Failsafe WiFi Reconnection
Ensure firmware recovers seamlessly if home WiFi disconnects without blocking the local physical alarm.
