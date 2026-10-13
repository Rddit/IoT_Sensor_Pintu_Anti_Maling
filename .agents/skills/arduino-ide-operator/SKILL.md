---
name: arduino-ide-operator
description: >-
  Use this skill to configure, operate, compile, flash, monitor, and manage the Arduino / ESP32
  toolchain directly within Antigravity, turning Antigravity into a fully functional Arduino IDE development environment.
---

# Arduino IDE Operator for Antigravity

This skill transforms Antigravity into a complete, integrated embedded development suite equivalent to the **Arduino IDE 2.x**, allowing both **visual independent GUI operation (via editor extensions & keyboard shortcuts)** and **autonomous pair programming (via CLI scripts & agent tools)**.

---

## 1. Toolchain & Environment Architecture

- **Editor Extensions**:
  - `vscode-arduino.vscode-arduino-community`: Visual GUI toolbar (Verify ✔, Upload ➡, Board & Port selector).
  - `ms-vscode.vscode-serial-monitor`: Dedicated interactive serial monitor panel with auto-reconnect.
  - `ms-vscode.cpptools`: C/C++ IntelliSense configured with complete ESP32 core include headers.
- **Arduino CLI Executable**:
  `C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe`
- **Core Platform**: `esp32:esp32` v3.3.12 (Espressif Systems)
- **Board FQBN**: `esp32:esp32:esp32` (ESP32 Dev Module)
- **Installed Project Libraries**:
  - `Blynk` (v1.3.5)
  - `UniversalTelegramBot` (v1.3.0)
  - `ArduinoJson` (v7.4.3)
  - `BlynkNcpDriver` (v0.7.0)

---

## 2. Visual Independent Operations (GUI & Shortcuts)

1. **Verify & Compile via Shortcut**:
   - Tekan **`Ctrl + Shift + B`** di editor untuk langsung mengompilasi sketch yang aktif.
2. **Tombol GUI di Editor**:
   - Di pojok kanan atas file `.ino`: Klik ikon **✔ (Verify)** atau **➡ (Upload)**.
   - Di status bar bawah: Klik port COM aktif atau board selector.
3. **Menu Task Editor**:
   - Buka menu **Terminal** -> **Run Task...**:
     - `Arduino: Verify / Compile (Standalone Alarm)`
     - `Arduino: Verify / Compile (Blynk IoT Alarm)`
     - `Arduino: Upload Firmware to ESP32 (Auto Detect Port)`
     - `Arduino: Detect Connected ESP32 COM Port`
     - `Arduino: Open Serial Monitor (115200 Baud)`

---

## 3. Autonomous Agent & Terminal Operations (CLI)

### A. Deteksi Board & Port COM
```powershell
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/detect_board.ps1"
```

### B. Compile / Verify Sketch
```powershell
# Compile Mode 1 (Standalone)
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/compile.ps1" "src/01_standalone_door_alarm"

# Compile Mode 2 (Blynk IoT)
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/compile.ps1" "src/02_blynk_door_alarm"

# Compile Mode 3 (Telegram)
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/compile.ps1" "src/03_telegram_door_alarm"
```

### C. Upload / Flashing ke ESP32
```powershell
# Auto-detect COM port
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/upload.ps1" "src/01_standalone_door_alarm"

# Spesifik COM port (misal COM3)
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/upload.ps1" "src/02_blynk_door_alarm" "COM3"
```
*Catatan*: Jika terminal menampilkan `Connecting........_____`, tekan tombol **BOOT** fisik pada ESP32 selama 2 detik lalu lepaskan.

### D. Buka Serial Monitor di Antigravity
```powershell
powershell -ExecutionPolicy Bypass -File ".agents/skills/arduino-ide-operator/scripts/monitor.ps1" "COM3"
```

---

## 4. Helper Scripts Reference

- [`setup_esp32_env.ps1`](./scripts/setup_esp32_env.ps1): Script otomatisasi pembaruan core dan library.
- [`detect_board.ps1`](./scripts/detect_board.ps1): Script pendeteksi port COM aktif.
- [`compile.ps1`](./scripts/compile.ps1): Wrapper compile cepat dengan FQBN `esp32:esp32:esp32`.
- [`upload.ps1`](./scripts/upload.ps1): Wrapper upload otomatis ke ESP32.
- [`monitor.ps1`](./scripts/monitor.ps1): Wrapper serial monitor interaktif di terminal.
