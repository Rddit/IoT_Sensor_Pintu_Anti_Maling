# Project Context: IoT Sensor Pintu Anti-Maling (ESP32)

## 1. Identitas & Tujuan Proyek
- **Nama Proyek**: IoT Sensor Pintu Anti-Maling berbasis ESP32
- **Tujuan Utama**: Mendeteksi pembukaan pintu/jendela secara real-time menggunakan sensor saklar magnetik (reed switch), membunyikan alarm lokal (Buzzer + LED), dan mengirimkan peringatan darurat ke smartphone pengguna via koneksi Internet/WiFi (Blynk IoT & Telegram Bot).
- **Referensi Demo Alat**: [Video Tutorial & Demo Tokotronik (YouTube)](https://www.youtube.com/watch?v=wkzndpY5nb0)
- **Status Proyek**: Fase Implementasi, Toolchain Aktif & Dokumentasi Lengkap

---

## 2. Inventaris Hardware & Analisis Kebutuhan

| No | Komponen Tersedia (Sesuai Foto) | Kondisi / Tipe | Catatan & Analisis |
|:---|:---|:---|:---|
| 1 | **ESP32 DevKit 38-Pin** | 38-Pin (ESP-WROOM-32 / DevKit V4) | Mikrokontroler utama Dual-Core WiFi+Bluetooth (19 pin kiri, 19 pin kanan) |
| 2 | **Baseboard Breakout ESP32 38-Pin** | Slot 38-pin, Triple Power (DC Jack, Micro-USB, Type-C) | Memudahkan ekspansi pin GPIO, VCC, dan GND tanpa breadboard |
| 3 | **Sensor Magnet MC-38** | 2 Bagian (Reed switch kabel + Magnet) | Detektor buka/tutup pintu (dry contact switch pasif) |
| 4 | **Buzzer Module** | Modul 3-Pin (VCC, GND, I/O) | Alarm suara lokal (Active Buzzer Module 3.3V - 5V) |
| 5 | **LED Merah 5mm** | Komponen pasif 2 kaki (Anoda/Katoda) | Indikator visual darurat (wajib resistor 220Ω) |
| 6 | **Kabel USB Biru** | USB-A to Micro-USB | Untuk flashing firmware & catu daya pengujian dari laptop/PC |

### Koreksi Komponen Tambahan Spesifik (Angka Pas & Siap Pakai):
| No | Komponen Tambahan | Jumlah Pas | Spesifikasi Teknis | Keterangan |
|:---:|:---|:---:|:---|:---|
| 1 | **Resistor 220 Ω** | **1 buah** | 1/4 Watt, Toleransi 5% | Pembatas arus seri untuk LED 5mm agar GPIO ESP32 aman dari overcurrent |
| 2 | **Kabel Jumper F-F** | **3 helai** | Panjang 20 cm, Female-to-Female | Menghubungkan 3 pin Buzzer (VCC, GND, I/O) ke Baseboard 38-pin |
| 3 | **Kabel Jumper F-M** | **4 helai** | Panjang 20 cm, Female-to-Male | 2 helai untuk LED+Resistor, 2 helai untuk kabel sensor magnet |
| 4 | **Adaptor Charger HP / DC** | **1 unit** | DC 5V 1A - 3A | Catu daya mandiri di kusen pintu (3A sangat direkomendasikan mencegah brownout) |
| 5 | **Kabel Micro-USB / Type-C** | **1 buah** | Panjang 1.0 m - 1.5 m | Kabel power panjang dari stopkontak ke kusen pintu |
| 6 | **Double Tape Foam 3M** | **1 roll kecil** | Busa kuat lebar 12mm/24mm | Perekatan modul sensor dan magnet ke pintu |
| 7 | **Solasi Listrik Hitam** | **1 buah** | Vinyl electrical tape | Pengaman lilitan kaki resistor dan LED agar tidak korslet |

> **Analisis Baseboard 38-Pin vs Breadboard**: Modul ESP32 38-pin memiliki fisik yang sangat lebar (19 pin per sisi). Jika dipaksakan pada breadboard biasa, seluruh lubang di satu sisi akan tertutup total dan sambungannya rawan kendor akibat getaran pintu. Baseboard 38-pin menyediakan 3 soket catu daya mandiri (DC Jack 6.5V-16V, port Micro-USB, dan port Type-C), rail VCC/GND terdedikasi, serta cengkeraman soket yang kuat.

---

## 3. Skema Pinout Baseboard 38-Pin (Wiring Matrix & Sablon PCB)

### A. Tabel Sambungan Komponen Proyek
| Komponen | Pin Komponen | Sablon di Baseboard 38-Pin | Nomor GPIO Koding | Posisi Fisik Baseboard 38-Pin | Logika / Status |
|:---|:---|:---:|:---:|:---|:---|
| **Sensor Magnet MC-38** | Kabel Putih 1 | **`D4`** *(atau `4`)* | `GPIO 4` | Sisi Kanan (urutan ke-13 dari atas, antara D16 & D0) | `INPUT_PULLUP`: **LOW (0V)** tertutup / **HIGH (3.3V)** terbuka |
| | Kabel Putih 2 | **`GND`** *(atau `G`)* | `GND` | Sisi Kanan (pin 20, pin 26, atau rail G) | Bebas tertukar 100% (saklar pasif tanpa polaritas) |
| **Buzzer Module** | Pin `VCC` | **`5V`** *(atau `V`)* | - | Jalur Power (VCC 5V/3.3V) | Daya modul 3.3V – 5V |
| | Pin `GND` | **`GND`** *(atau `G`)* | `GND` | Jalur Ground (rail G) | Ground |
| | Pin `I/O` (Signal) | **`D18`** *(atau `18`)* | `GPIO 18` | Sisi Kanan (urutan ke-9 dari atas, tepat bawah D19) | **HIGH** = Bunyi alarm, **LOW** = Senyap |
| **LED Merah 5mm** | Anoda (+) via R 220Ω | **`D19`** *(atau `19`)* | `GPIO 19` | Sisi Kanan (urutan ke-8 dari atas, tepat atas D18) | **HIGH** = Kedip darurat, **LOW** = Mati |
| | Katoda (-) | **`GND`** *(atau `G`)* | `GND` | Jalur Ground (rail G atau pin 26) | Ground kaki negatif LED |

### B. Pemetaan Lengkap Seluruh 38 Pin ESP32 (Sisi Kiri & Kanan)

```text
               [ ANTENA WIFI ESP32 (SISI ATAS) ]
     SISI KIRI (19 Pin)                SISI KANAN (19 Pin)
  1. [3V3]  - Output 3.3V           20. [GND]  - Ground (0V)
  2. [EN]   - Reset Chip            21. [D23]  - GPIO 23 (VSPI MOSI)
  3. [VP]   - GPIO 36 (Input-Only)  22. [D22]  - GPIO 22 (I2C SCL)
  4. [VN]   - GPIO 39 (Input-Only)  23. [TX]   - GPIO 1  (Serial TX)
  5. [D34]  - GPIO 34 (Input-Only)  24. [RX]   - GPIO 3  (Serial RX)
  6. [D35]  - GPIO 35 (Input-Only)  25. [D21]  - GPIO 21 (I2C SDA)
  7. [D32]  - GPIO 32               26. [GND]  - Ground (0V)
  8. [D33]  - GPIO 33               27. [D19]  - GPIO 19  <-- [LED ANODA (+) + R 220Ω]
  9. [D25]  - GPIO 25               28. [D18]  - GPIO 18  <-- [BUZZER I/O]
 10. [D26]  - GPIO 26               29. [D5]   - GPIO 5  (Strapping)
 11. [D27]  - GPIO 27               30. [D17]  - GPIO 17 (UART2 TX)
 12. [D14]  - GPIO 14 (Alt Input)   31. [D16]  - GPIO 16 (UART2 RX)
 13. [D12]  - GPIO 12 (Strapping)   32. [D4]   - GPIO 4   <-- [SENSOR MAGNET KABEL 1]
 14. [GND]  - Ground (0V)           33. [D0]   - GPIO 0  (Tombol BOOT)
 15. [D13]  - GPIO 13 (Alt Input)   34. [D2]   - GPIO 2  (LED Onboard)
 16. [D9]   - SPI Flash (JANGAN!)   35. [D15]  - GPIO 15 (Strapping)
 17. [D10]  - SPI Flash (JANGAN!)   36. [D8]   - SPI Flash (JANGAN!)
 18. [D11]  - SPI Flash (JANGAN!)   37. [D7]   - SPI Flash (JANGAN!)
 19. [VIN]  - 5V In / Out           38. [D6]   - SPI Flash (JANGAN!)
               [ PORT MICRO-USB / TYPE-C / DC JACK ]
```

> **Aturan Penting Baseboard 38-Pin**:
> 1. **Pin D4, D18, D19 Berkumpul di Sisi Kanan**: Memudahkan pengkabelan yang sangat rapi dan berdekatan dengan pin GND sisi kanan.
> 2. **Pin Terlarang (D6 s/d D11)**: Terhubung langsung ke chip memori SPI Flash internal ESP32. Jangan gunakan sama sekali.
> 3. **Pin Input-Only (VP, VN, D34, D35)**: Tidak memiliki transistor output dan tidak memiliki resistor pull-up internal.
> 4. **Pin Alternatif Sensor Magnet**: Jika pin `D4` rusak atau sulit dijangkau, gunakan pin `D13` (Sisi Kiri urutan 15) atau `D14` (Sisi Kiri urutan 12). Cukup ubah angka `4` menjadi `13` pada koding.

---

## 4. Arsitektur Firmware & Opsi Eksekusi
1. **Mode 1 - Standalone Alarm (Lokal Tanpa Internet)**:
   - File: [`src/01_standalone_door_alarm/01_standalone_door_alarm.ino`](./src/01_standalone_door_alarm/01_standalone_door_alarm.ino)
   - Status: Terkompilasi sukses (272 KB / 20% flash memory).
2. **Mode 2 - IoT Cloud Alarm (Blynk IoT v2.0)**:
   - File: [`src/02_blynk_door_alarm/02_blynk_door_alarm.ino`](./src/02_blynk_door_alarm/02_blynk_door_alarm.ino)
   - Status: Terkompilasi sukses dengan stack Blynk + WiFi (916 KB / 69% flash memory).
3. **Mode 3 - IoT Telegram Notification Alarm**:
   - File: [`src/03_telegram_door_alarm/03_telegram_door_alarm.ino`](./src/03_telegram_door_alarm/03_telegram_door_alarm.ino)
   - Status: Siap dikompilasi & di-upload dengan library UniversalTelegramBot.

---

## 5. Toolchain & Lingkungan Kerja Mandiri (IDE & Extension)
1. **Ekstensi Mandiri yang Terpasang di Editor**:
   - `vscode-arduino.vscode-arduino-community`: Antarmuka visual tombol Arduino (Verify ✔, Upload ➡, Board & Port selector).
   - `ms-vscode.vscode-serial-monitor`: Serial Monitor visual bawaan dengan auto-reconnect & baudrate 115200.
   - `ms-vscode.cpptools`: C/C++ IntelliSense untuk autocomplete dan sintaks highlight tanpa error squiggles.
2. **Konfigurasi Workspace di [`.vscode/`](./.vscode/)**:
   - [`settings.json`](./.vscode/settings.json): Konfigurasi Arduino CLI engine & baudrate.
   - [`arduino.json`](./.vscode/arduino.json): Konfigurasi board `esp32:esp32:esp32` dan target sketch.
   - [`tasks.json`](./.vscode/tasks.json): Shortcut sistem build `Ctrl + Shift + B` dan menu tasks upload/monitor.
   - [`c_cpp_properties.json`](./.vscode/c_cpp_properties.json): Path include lengkap ESP32 core v3.3.12 dan library Arduino.
3. **Toolchain Kompilator Terpasang**:
   - `arduino-cli`: v1.5.1
   - Core Platform: `esp32:esp32` v3.3.12 (Espressif Systems)
   - Libraries: `Blynk` (v1.3.5), `UniversalTelegramBot` (v1.3.0), `ArduinoJson` (v7.4.3).

---

## 6. Milestone & Roadmap Eksekusi
- [x] Analisis hardware riil dari foto user & video demo Tokotronik
- [x] Pembuatan superpower meta-skill [`.agents/skills/superpower-skill-creator`](./.agents/skills/superpower-skill-creator/SKILL.md)
- [x] Pembuatan context persistence skill [`.agents/skills/project-context-keeper`](./.agents/skills/project-context-keeper/SKILL.md)
- [x] Pembuatan skill embedded [`.agents/skills/esp32-firmware-engineer`](./.agents/skills/esp32-firmware-engineer/SKILL.md)
- [x] Pembuatan skill cloud [`.agents/skills/iot-cloud-integrator`](./.agents/skills/iot-cloud-integrator/SKILL.md)
- [x] Pembuatan skill hardware [`.agents/skills/hardware-circuit-debugger`](./.agents/skills/hardware-circuit-debugger/SKILL.md)
- [x] Pembuatan skill Arduino IDE operator [`.agents/skills/arduino-ide-operator`](./.agents/skills/arduino-ide-operator/SKILL.md)
- [x] Penulisan source code Arduino lengkap (Mode Standalone, Blynk IoT, Telegram Bot)
- [x] Penulisan panduan tutorial komprehensif step-by-step
- [x] Setup otomatis toolchain Arduino CLI, board ESP32, dan libraries (Blynk, Telegram, ArduinoJson)
- [x] Instalasi dan konfigurasi ekstensi IDE mandiri (`vscode-arduino-community` & `vscode-serial-monitor`)
- [x] Konfigurasi `.vscode/tasks.json`, `.vscode/settings.json`, dan `.vscode/c_cpp_properties.json`
- [x] Uji coba kompilasi binary ESP32 sukses 100% (Mode 1, Mode 2, Mode 3)
- [x] Migrasi dan pemetaan menyeluruh arsitektur ke ESP32 38-Pin & Baseboard Breakout 38-Pin
- [x] Publikasi repositori resmi ke GitHub: [Rddit/IoT_Sensor_Pintu_Anti_Maling](https://github.com/Rddit/IoT_Sensor_Pintu_Anti_Maling)
