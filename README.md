# IoT Sensor Pintu Anti-Maling ESP32

Sistem alarm keamanan pintu rumah berbasis mikrokontroler **ESP32** dengan sensor magnetik **MC-38**, alarm suara lokal (**Buzzer + LED**), dan notifikasi internet instan ke smartphone via platform **Blynk IoT** dan **Telegram Bot**.

Proyek ini telah dikonfigurasi penuh agar dapat dikembangkan, dikompilasi, di-upload, dan dipantau **secara visual dan mandiri** langsung dari dalam editor Antigravity/VS Code layaknya Arduino IDE bawaan.

> 📖 **Panduan Tutorial Lengkap Step-by-Step**: Baca [PANDUAN_TUTORIAL_LENGKAP_SENSOR_PINTU_ESP32.md](./PANDUAN_TUTORIAL_LENGKAP_SENSOR_PINTU_ESP32.md).  
> 📋 **Arsitektur & Konteks Proyek**: Pantau single source of truth di [PROJECT_CONTEXT.md](./PROJECT_CONTEXT.md).

---

## 📁 Struktur Direktori Proyek

```text
IoT Sensor Pintu Anti Maling/
├── .agents/
│   └── skills/
│       ├── superpower-skill-creator/     # Meta-skill pembuat & validator skill baru
│       │   └── SKILL.md
│       ├── project-context-keeper/       # Skill pelindung & pengeksekusi konteks proyek
│       │   └── SKILL.md
│       ├── esp32-firmware-engineer/      # Skill rekayasa firmware Arduino ESP32 & debounce
│       │   └── SKILL.md
│       ├── iot-cloud-integrator/         # Skill konfigurasi cloud Blynk IoT & Telegram
│       │   └── SKILL.md
│       ├── hardware-circuit-debugger/    # Skill diagnosis kelistrikan, resistor LED & wiring
│       │   └── SKILL.md
│       └── arduino-ide-operator/         # Skill pengoperasian Arduino IDE & CLI di Antigravity
│           ├── SKILL.md
│           └── scripts/                  # Skrip compile, upload, serial monitor, detect board
├── .vscode/
│   ├── settings.json                     # Konfigurasi integrasi engine Arduino CLI & port
│   ├── arduino.json                      # Pengaturan board default esp32 & sketch aktif
│   ├── tasks.json                        # Shortcut sistem build (Ctrl+Shift+B) & tasks upload
│   ├── c_cpp_properties.json             # IntelliSense C++ lengkap untuk library & core ESP32
│   └── extensions.json                   # Rekomendasi ekstensi workspace
├── PROJECT_CONTEXT.md                    # Single Source of Truth konteks & pinout proyek
├── PANDUAN_TUTORIAL_LENGKAP_SENSOR_PINTU_ESP32.md # Panduan komprehensif langkah demi langkah
└── src/
    ├── 01_standalone_door_alarm/         # Mode 1: Uji coba alarm lokal tanpa WiFi
    │   └── 01_standalone_door_alarm.ino
    ├── 02_blynk_door_alarm/              # Mode 2: Sesuai video demo Blynk IoT (Tokotronik)
    │   └── 02_blynk_door_alarm.ino
    └── 03_telegram_door_alarm/           # Mode 3: Notifikasi darurat gratis via Telegram Bot
        └── 03_telegram_door_alarm.ino
```

---

## ⚡ Skema Kelistrikan & Jalur Wiring (Baseboard 38-Pin)

| Komponen | Pin Komponen | Sablon Pin di Baseboard 38-Pin | Nomor di Koding | Posisi Fisik Baseboard 38-Pin | Logika / Status |
| :--- | :--- | :---: | :---: | :--- | :--- |
| **Sensor Magnet MC-38** | Kabel Putih 1 | **`D4`** *(atau `4`)* | `GPIO 4` | Sisi Kanan (Pin 32, antara D16 & D0) | Mode `INPUT_PULLUP`:<br>• **LOW (0V)** = Pintu tertutup rapat (Aman)<br>• **HIGH (3.3V)** = Pintu terbuka (Bahaya) |
| | Kabel Putih 2 | **`GND`** | `GND` | Sisi Kanan (Pin 20, 26, atau rail G) | Bebas tertukar 100% (saklar pasif tanpa polaritas) |
| **Buzzer Module (3-Pin)**| Pin `VCC` | **`5V`** *(atau `V`)* | - | Header Power Rail (5V/3.3V) | Catu daya modul buzzer |
| | Pin `GND` | **`GND`** | `GND` | Header Ground Rail (G) | Ground modul buzzer |
| | Pin `I/O` (Data) | **`D18`** *(atau `18`)* | `GPIO 18` | Sisi Kanan (Pin 28, tepat di bawah D19) | **HIGH** = Bunyi Sirine, **LOW** = Senyap |
| **LED Merah 5mm** | Anoda (+) via R 220Ω | **`D19`** *(atau `19`)* | `GPIO 19` | Sisi Kanan (Pin 27, tepat di atas D18) | **HIGH** = Berkedip Cepat, **LOW** = Padam |
| | Katoda (-) | **`GND`** | `GND` | Header Ground Rail (G atau Pin 26) | Ground kaki katoda LED |

> 📌 *Catatan Pin Fisik Baseboard 38-Pin*:
> - Pin **`D19`** (LED), **`D18`** (Buzzer), dan **`D4`** (Sensor Pintu) semuanya berkumpul rapi di **Sisi Kanan** baseboard!
> - Tepat di antara atau di sampingnya terdapat pin **`GND`** (Pin 20, Pin 26) serta deretan pin ground (`G`).
> - Jika pin D4 sulit ditemukan atau rusak, alternatif pin input yang sangat aman adalah **`D13`** atau **`D14`** (di Sisi Kiri baseboard).

---

## 🛒 Komponen Tambahan Spesifik (Jumlah Pas & To the Point)

| Komponen | Jumlah Pas | Spesifikasi | Kegunaan |
| :--- | :---: | :--- | :--- |
| **Resistor Pembatas LED** | **1 buah** | **220 Ω (1/4 Watt)** | Wajib di Anoda (+) LED agar GPIO ESP32 aman dari arus berlebih |
| **Kabel Jumper F-F** | **3 helai** | 20 cm (Female-to-Female) | Hubungkan 3 pin Buzzer (VCC, GND, I/O) ke Baseboard 38-pin |
| **Kabel Jumper F-M** | **4 helai** | 20 cm (Female-to-Male) | 2 helai untuk kaki LED, 2 helai untuk kawat sensor magnet |
| **Adaptor Charger HP / DC** | **1 unit** | DC 5V 1A - 3A (3A direkomendasikan) | Catu daya mandiri permanen di pintu (bebas brownout) |
| **Kabel USB Panjang** | **1 buah** | 1.0 m – 1.5 meter (Micro-USB / Type-C) | Gantikan kabel biru 30 cm yang terlalu pendek |
| **Double Tape Foam 3M** | **1 roll kecil** | Busa kuat 12mm / 24mm | Tempel sensor di kusen & magnet di daun pintu |
| **Solasi Listrik Hitam** | **1 buah** | Vinyl electrical tape | Pengaman lilitan kaki LED + Resistor agar tidak korslet |

> 💡 *Catatan Baseboard 38-Pin*: Baseboard 38-pin pada foto Anda sudah memiliki soket **Triple Power** (Jack DC 6.5V-16V, port Micro-USB, dan port Type-C) serta pin header ekspansi terdedikasi. Menggunakan breadboard biasa justru tidak cocok karena ESP32 38-pin sangat lebar (menutup semua baris lubang) dan sambungannya rawan kendor akibat getaran pintu.

---

## 🚀 3 Cara Menjalankan & Memprogram ESP32 di Editor Ini

### Cara 1: Menggunakan Tombol Visual Ekstensi (Paling Mudah)
Saat membuka file `.ino` (misalnya [`01_standalone_door_alarm.ino`](./src/01_standalone_door_alarm/01_standalone_door_alarm.ino)):
- **Ikon Pojok Kanan Atas Editor**:
  - Klik ikon **Verify (✔)** untuk mengompilasi program.
  - Klik ikon **Upload (➡)** untuk mengunggah firmware ke ESP32.
- **Status Bar (Pojok Kanan Bawah)**:
  - Klik nama port serial (misal `COM3`) untuk memilih port ESP32 Anda.
  - Board sudah otomatis disetel ke `ESP32 Dev Module`.

### Cara 2: Menggunakan Shortcut Keyboard
- Tekan **`Ctrl + Shift + B`** di keyboard untuk langsung memverifikasi dan mengompilasi program yang aktif.
- Buka menu atas: **Terminal** -> **Run Task...**:
  - Pilih `Arduino: Upload Firmware to ESP32 (Auto Detect Port)` untuk upload.
  - Pilih `Arduino: Open Serial Monitor (115200 Baud)` untuk membuka serial monitor.
  - Pilih `Arduino: Detect Connected ESP32 COM Port` untuk mendeteksi nomor COM port.

### Cara 3: Meminta Bantuan Antigravity (Pair Programming)
Cukup berikan instruksi di chat, seperti:
> *"Tolong upload koding blynk ke ESP32 saya"*  
> atau  
> *"Buka serial monitor untuk melihat status sensor pintu"*

Antigravity akan mengeksekusinya secara otonom melalui skill [`arduino-ide-operator`](./.agents/skills/arduino-ide-operator/SKILL.md).
