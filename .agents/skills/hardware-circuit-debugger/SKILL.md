---
name: hardware-circuit-debugger
description: >-
  Use this skill to diagnose hardware wiring, logic levels (3.3V vs 5V), pull-up/pull-down
  configurations, baseboard pinouts, buzzer driving circuits, and sensor positioning.
---

# Hardware & Circuit Debugger

Comprehensive hardware reference and troubleshooting guide for the ESP32 Door Anti-Theft Alarm project.

## Hardware Anatomy & Inspection

# Hardware & Circuit Debugger

Comprehensive hardware reference and troubleshooting guide for the ESP32 Door Anti-Theft Alarm project with the **38-Pin ESP32 DevKit & 38-Pin Expansion Baseboard**.

## Hardware Anatomy & Inspection

### 1. ESP32 DevKit 38-Pin & Baseboard Breakout (38 Pins)
- **Baseboard Mounting**: The expansion board features two 19-pin female header sockets (38 pins total) matching the ESP32 38-pin DevKit. Align all 19 pins on both sides carefully before pressing down evenly.
- **Triple Power Delivery Options**:
  - **Port Micro-USB**: Input 5V DC via standard USB cable.
  - **Port USB Type-C**: Input 5V DC via modern Type-C cable.
  - **DC Jack (5.5x2.1mm)**: Input DC 6.5V - 16V regulated via onboard AMS1117 5V & 3.3V LDO regulators.
  - *Current Capacity*: 5V 1A, 2A, or 3A+ adapters are 100% safe (ampere is available supply capacity; higher amperage ensures zero brownouts during WiFi transmit bursts). *Never plug multiple power supplies simultaneously.*
- **Voltage Selector Jumper (`V_SEL` / `3V3 - 5V`)**:
  - Configures the `V` (VCC) power rails on the breakout pin headers to deliver either **3.3V** or **5V**.
  - Default set to 5V (or 3.3V depending on module requirements). Active buzzer accepts 3.3V - 5V.

---

### 2. Complete 38-Pin Breakdown & GPIO Rules

#### Left Header (Pins 1 - 19)
| Pin # | Label | GPIO / Function | Status & Safety Rules |
| :---: | :--- | :--- | :--- |
| 1 | **3V3** | 3.3V Output | Power rail (regulated) |
| 2 | **EN** | Chip Enable / Reset | Active LOW reset button |
| 3 | **VP** | GPIO 36 (ADC1_0) | **INPUT ONLY** (No internal pullup/pulldown, no output) |
| 4 | **VN** | GPIO 39 (ADC1_3) | **INPUT ONLY** (No internal pullup/pulldown, no output) |
| 5 | **D34** | GPIO 34 (ADC1_6) | **INPUT ONLY** (No internal pullup/pulldown, no output) |
| 6 | **D35** | GPIO 35 (ADC1_7) | **INPUT ONLY** (No internal pullup/pulldown, no output) |
| 7 | **D32** | GPIO 32 (Touch 9) | General I/O, RTC, ADC1 |
| 8 | **D33** | GPIO 33 (Touch 8) | General I/O, RTC, ADC1 |
| 9 | **D25** | GPIO 25 (DAC 1) | General I/O, DAC output |
| 10 | **D26** | GPIO 26 (DAC 2) | General I/O, DAC output |
| 11 | **D27** | GPIO 27 (Touch 7) | General I/O (Alternative safe input) |
| 12 | **D14** | GPIO 14 (Touch 6) | General I/O (Safe sensor input alternative) |
| 13 | **D12** | GPIO 12 (Touch 5) | Strapping pin (Do not pull HIGH during boot) |
| 14 | **GND** | Ground | 0V Common ground |
| 15 | **D13** | GPIO 13 (Touch 4) | General I/O (Primary fallback for D4 sensor input!) |
| 16 | **D9** / SD2 | GPIO 9 | ⛔ **RESERVED FOR SPI FLASH - DO NOT CONNECT!** |
| 17 | **D10** / SD3| GPIO 10 | ⛔ **RESERVED FOR SPI FLASH - DO NOT CONNECT!** |
| 18 | **D11** / CMD| GPIO 11 | ⛔ **RESERVED FOR SPI FLASH - DO NOT CONNECT!** |
| 19 | **VIN / 5V** | External Power In/Out | 5V DC rail |

#### Right Header (Pins 20 - 38)
| Pin # | Label | GPIO / Function | Status & Safety Rules |
| :---: | :--- | :--- | :--- |
| 20 | **GND** | Ground | 0V Common ground |
| 21 | **D23** | GPIO 23 (VSPI MOSI) | General I/O |
| 22 | **D22** | GPIO 22 (I2C SCL) | General I/O, default I2C Clock |
| 23 | **TX / TX0**| GPIO 1 (UART0 TX) | Serial Monitor debug / upload (Do not use) |
| 24 | **RX / RX0**| GPIO 3 (UART0 RX) | Serial Monitor debug / upload (Do not use) |
| 25 | **D21** | GPIO 21 (I2C SDA) | General I/O, default I2C Data |
| 26 | **GND** | Ground | 0V Common ground |
| 27 | **D19** | GPIO 19 (VSPI MISO) | 🔴 **CONNECTED TO RED LED (+) via 220Ω Resistor** |
| 28 | **D18** | GPIO 18 (VSPI CLK) | 📢 **CONNECTED TO ACTIVE BUZZER (I/O)** |
| 29 | **D5** | GPIO 5 (VSPI CS0) | Strapping pin (Outputs PWM at startup) |
| 30 | **TX2 / D17**| GPIO 17 (UART2 TX) | General I/O |
| 31 | **RX2 / D16**| GPIO 16 (UART2 RX) | General I/O |
| 32 | **D4** | GPIO 4 (Touch 0) | 🚪 **CONNECTED TO MC-38 DOOR SENSOR (Wire 1)** |
| 33 | **D0** | GPIO 0 (BOOT) | Strapping pin / BOOT button (Pulled LOW to flash) |
| 34 | **D2** | GPIO 2 (LED onboard)| Strapping pin / Onboard blue LED |
| 35 | **D15** | GPIO 15 (Touch 3) | Strapping pin (Outputs debug messages at boot) |
| 36 | **D8** / SD1 | GPIO 8 | ⛔ **RESERVED FOR SPI FLASH - DO NOT CONNECT!** |
| 37 | **D7** / SD0 | GPIO 7 | ⛔ **RESERVED FOR SPI FLASH - DO NOT CONNECT!** |
| 38 | **D6** / CLK | GPIO 6 | ⛔ **RESERVED FOR SPI FLASH - DO NOT CONNECT!** |

---

### 3. Component Wiring Verification

1. **MC-38 Magnetic Door Sensor (2 Wires)**:
   - Wire 1 (either short or long) -> Pin **`D4`** (Pin 32 on the right header, between D16 and D0).
   - Wire 2 (either short or long) -> Pin **`GND`** (Pin 20, 26, or baseboard G rail).
   - *Logic*: Configured with `pinMode(4, INPUT_PULLUP)`. Door closed = `LOW` (0V). Door opened = `HIGH` (3.3V).
   - *Safe Fallbacks*: If D4 is obscured or damaged, use **`D13`** (Left Pin 15) or **`D14`** (Left Pin 12).

2. **Active Buzzer Module (3 Pins)**:
   - `VCC` -> Pin **`5V`** (or `3.3V` / V rail).
   - `GND` -> Pin **`GND`** (G rail).
   - `I/O` (or `SIG`) -> Pin **`D18`** (Pin 28 on the right header, right below D19).

3. **Red LED (5mm) & Current Limiter**:
   - Anode (Long Leg / +) via **220Ω (1/4W)** resistor -> Pin **`D19`** (Pin 27 on the right header).
   - Cathode (Short Leg / -) -> Pin **`GND`**.
   - *Safety Note*: Never connect 5mm LED directly to 3.3V without the 220Ω resistor.

---

## Required Additional Hardware Inventory (Exact Quantities)

| Component | Exact Qty | Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **Resistor** | **1 pcs** | 220 Ω, 0.25W (5% tolerance) | Current limiter for LED Anode |
| **Dupont Jumper F-F** | **3 pcs** | 20 cm, Female-to-Female | Buzzer module (VCC, GND, I/O) to Baseboard |
| **Dupont Jumper F-M** | **4 pcs** | 20 cm, Female-to-Male | 2 for LED legs, 2 for magnetic sensor bare wires |
| **Power Supply** | **1 unit** | DC 5V 1A - 3A USB Charger / DC Adapter | Permanent standalone power |
| **Long Micro-USB / Type-C** | **1 pcs** | 1.0 m – 1.5 m | Replaces short 30cm bench cable |
| **Adhesive Tape** | **1 roll** | 3M Foam Double Tape | Sensor & magnet mounting on door/frame |
| **Insulation Tape** | **1 roll** | Black PVC electrical tape | Insulates resistor lead connections |

### Baseboard 38-Pin vs Breadboard
- **38-Pin Baseboard is Highly Recommended**: The 38-pin ESP32 is even wider than the 30-pin variant. Placing it on a standard breadboard completely eliminates all access holes on one side. The 38-pin Baseboard provides dedicated headers, ground rails, and triple power inputs with vibration-proof sockets ideal for doors.

