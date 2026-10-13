---
name: iot-cloud-integrator
description: >-
  Use this skill to configure, connect, and debug cloud notification integrations
  for ESP32, specifically New Blynk IoT (v2.0) and Telegram Bot API.
---

# IoT Cloud Integrator

Workflow runbook for provisioning cloud communication, dashboard controls, and real-time smartphone push alerts.

## Blynk IoT (v2.0) Setup Runbook

1. **Create Template**:
   - Web Console: [blynk.cloud](https://blynk.cloud)
   - Hardware: `ESP32`, Connection: `WiFi`.
   - Copy `#define BLYNK_TEMPLATE_ID` and `#define BLYNK_TEMPLATE_NAME`.

2. **Configure Datastreams**:
   - `V0`: Door Status (Integer, 0 = Aman/Tertutup, 1 = Bahaya/Terbuka).
   - `V1`: Alarm Trigger Switch / Siren Arming (Integer 0/1).
   - `V2`: Notification Log String (String).

3. **Configure Events & Notifications**:
   - Go to *Settings > Events*.
   - Create Event `door_breach`:
     - Type: `Critical` or `Warning`.
     - Description: "Pintu dibuka secara tidak sah! Alarm aktif!"
     - Enable: Push Notification to device, Email alert.

4. **Firmware Integration**:
   - Install Library: `Blynk` by Volodymyr Shymanskyy.
   - Code snippet:
     ```cpp
     #define BLYNK_PRINT Serial
     #define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
     #define BLYNK_TEMPLATE_NAME "Door Alarm"
     #define BLYNK_AUTH_TOKEN "your_token"
     #include <WiFi.h>
     #include <WiFiClient.h>
     #include <BlynkSimpleEsp32.h>
     ```

## Telegram Bot Integration Alternative

For users wanting completely free, unlimited push notifications with zero platform restrictions:
1. Contact `@BotFather` on Telegram -> `/newbot` -> get `BOT_TOKEN`.
2. Contact `@userinfobot` or `@myidbot` -> get personal `CHAT_ID`.
3. Use `UniversalTelegramBot` library with `WiFiClientSecure`.
4. Send alert with markdown formatting and timestamp.
