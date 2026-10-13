/*
 * =====================================================================================
 * PROYEK: SENSOR PINTU ANTI MALING ESP32 (MODE STANDALONE / OFFLINE)
 * Penulis: Antigravity IoT Assistant
 * Deskripsi: Program alarm pintu mandiri tanpa WiFi. Mendeteksi pintu terbuka
 *            menggunakan sensor magnet MC-38, lalu membunyikan buzzer dan
 *            menyalakan LED sebagai alarm bahaya.
 * =====================================================================================
 * 
 * PINOUT KONEKSI PADA BASEBOARD 38-PIN:
 * - Sensor Magnet (MC-38) -> Pin bertuliskan "D4" (GPIO 4, sisi kanan, Pin 32 antara D16 & D0) & GND
 * - Buzzer Module (I/O)   -> Pin bertuliskan "D18" (GPIO 18, sisi kanan, Pin 28 tepat bawah D19)
 * - LED Indikator (+)     -> Pin bertuliskan "D19" (GPIO 19, sisi kanan, Pin 27 via Resistor 220 Ohm) & GND (-)
 * 
 * CATATAN BASEBOARD 38-PIN:
 * - Semua pin komponen (D4, D18, D19) berkumpul di SISI KANAN board bersama pin GND.
 * - Kabel putih sensor magnet bebas tertukar 100% (polaritas bebas).
 * - Jika pin D4 sulit dicari, alternatif pin aman di Sisi Kiri adalah 13 (D13) atau 14 (D14).
 */

// Definisi Pin GPIO (Di Baseboard 38-Pin tertulis D4, D18, D19 di Sisi Kanan)
const int PIN_SENSOR_MAGNET = 4;   // Pin D4 pada baseboard 38-pin (alternatif: ganti ke 13 jika pakai D13)
const int PIN_BUZZER        = 18;  // Pin D18 pada baseboard 38-pin (I/O Buzzer)
const int PIN_LED           = 19;  // Pin D19 pada baseboard 38-pin (Anoda LED via Resistor 220 Ohm)

// Variabel status sensor & debounce
int statusSensorSaatIni;
int statusSensorSebelumnya = HIGH;
unsigned long waktuDebounceTerakhir = 0;
const unsigned long jedaDebounce = 50; // Jeda 50 ms untuk stabilisasi mekanik saklar reed

// Variabel pola alarm (bunyi berkedip non-blocking menggunakan millis)
unsigned long waktuAlarmTerakhir = 0;
bool statusSuaraAlarm = false;
const unsigned long intervalAlarm = 150; // Frekuensi kedip dan bunyi cepat (150 ms)

void setup() {
  // Inisialisasi Serial Monitor untuk debugging
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=============================================");
  Serial.println("  SISTEM ALARM PINTU ANTI MALING ESP32       ");
  Serial.println("  MODE: STANDALONE (UJI COBA HARDWARE LOKAL) ");
  Serial.println("=============================================");

  // Konfigurasi pin input & output
  // INPUT_PULLUP: Mengaktifkan resistor pull-up internal ESP32
  // - Saat pintu tertutup (magnet nempel): Rangkaian tertutup ke GND -> Nilai = LOW
  // - Saat pintu terbuka (magnet lepas): Rangkaian terbuka -> Nilai ditarik ke 3.3V -> Nilai = HIGH
  pinMode(PIN_SENSOR_MAGNET, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // Pastikan saat awal menyala alarm dalam kondisi mati
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);

  Serial.println("[SIAP] Sensor aktif. Menunggu perubahan status pintu...");
}

void loop() {
  // Baca status sensor magnet secara real-time
  int bacaanMentah = digitalRead(PIN_SENSOR_MAGNET);

  // Debouncing sederhana
  if (bacaanMentah != statusSensorSebelumnya) {
    waktuDebounceTerakhir = millis();
  }

  if ((millis() - waktuDebounceTerakhir) > jedaDebounce) {
    if (bacaanMentah != statusSensorSaatIni) {
      statusSensorSaatIni = bacaanMentah;

      if (statusSensorSaatIni == HIGH) {
        // PINTU TERBUKA (MAGNET LEPAS DARI SENSOR)
        Serial.println(">> [PERINGATAN!] PINTU TERBUKA! ALARM AKTIF! <<");
      } else {
        // PINTU TERTUTUP (MAGNET MENEMPEL PADA SENSOR)
        Serial.println("[AMAN] Pintu tertutup rapat. Alarm nonaktif.");
        // Matikan buzzer dan LED seketika
        digitalWrite(PIN_BUZZER, LOW);
        digitalWrite(PIN_LED, LOW);
        statusSuaraAlarm = false;
      }
    }
  }

  statusSensorSebelumnya = bacaanMentah;

  // Logika Eksekusi Alarm:
  // Jika pintu dalam keadaan terbuka (HIGH), bunyikan buzzer dan kedipkan LED
  if (statusSensorSaatIni == HIGH) {
    unsigned long waktuSekarang = millis();
    if (waktuSekarang - waktuAlarmTerakhir >= intervalAlarm) {
      waktuAlarmTerakhir = waktuSekarang;
      statusSuaraAlarm = !statusSuaraAlarm;

      // Nyalakan / matikan buzzer dan LED secara serempak
      digitalWrite(PIN_BUZZER, statusSuaraAlarm ? HIGH : LOW);
      digitalWrite(PIN_LED, statusSuaraAlarm ? HIGH : LOW);
    }
  }
}
