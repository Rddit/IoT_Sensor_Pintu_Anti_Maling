/*
 * =====================================================================================
 * PROYEK: SENSOR PINTU ANTI MALING ESP32 DENGAN NEW BLYNK IOT (SESUAI VIDEO DEMO)
 * Referensi Video: https://www.youtube.com/watch?v=wkzndpY5nb0 (Tokotronik)
 * 
 * FUNGSI:
 * 1. Deteksi buka-tutup pintu secara realtime.
 * 2. Alarm lokal (Buzzer & LED berkedip cepat saat pintu dibuka paksa).
 * 3. Kirim status ke aplikasi Blynk di Smartphone (Android / iOS).
 * 4. Kirim Push Notification instan ke smartphone via Blynk Event saat maling membuka pintu.
 * 5. Fitur Remote Mute / Arming switch dari aplikasi Blynk (Virtual Pin V1).
 * =====================================================================================
 */

// GANTI BAGIAN INI DENGAN DATA DARI KONSOL BLYNK ANDA (https://blynk.cloud)
#define BLYNK_TEMPLATE_ID   "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME "Alarm Pintu ESP32"
#define BLYNK_AUTH_TOKEN    "Masukkan_Auth_Token_Blynk_Anda"

// Sertakan Library yang Dibutuhkan
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Konfigurasi WiFi Rumah / Hotspot HP
char ssid[] = "NAMA_WIFI_RUMAH_ANDA";
char pass[] = "PASSWORD_WIFI_ANDA";

// Definisi Pin GPIO (Di Baseboard 38-Pin tertulis D4, D18, D19 di Sisi Kanan)
const int PIN_SENSOR_MAGNET = 4;   // Pin D4 pada baseboard 38-pin (alternatif: ganti ke 13 jika pakai D13)
const int PIN_BUZZER        = 18;  // Pin D18 pada baseboard 38-pin (I/O Buzzer)
const int PIN_LED           = 19;  // Pin D19 pada baseboard 38-pin (Anoda LED via Resistor 220 Ohm)

// Timer Blynk untuk eksekusi berkala tanpa delay()
BlynkTimer timer;

// Status & Variabel Kontrol
int statusSensorTerakhir = -1;
bool sistemAktif = true;           // Saklar Arming/Disarm dari Blynk (V1)
bool statusBuzzerLed = false;
unsigned long waktuKedipTerakhir = 0;

// =====================================================================================
// BLYNK WRITE: Menerima Perintah dari Aplikasi Blynk di Smartphone
// =====================================================================================
// Virtual Pin V1: Tombol Switch di Aplikasi Blynk untuk Menghidupkan/Mematikan Sistem Alarm
BLYNK_WRITE(V1) {
  int nilaiSwitch = param.asInt();
  if (nilaiSwitch == 1) {
    sistemAktif = true;
    Serial.println("[BLYNK] Sistem Alarm Diaktifkan (ARMED)");
  } else {
    sistemAktif = false;
    // Matikan alarm seketika saat disarm
    digitalWrite(PIN_BUZZER, LOW);
    digitalWrite(PIN_LED, LOW);
    Serial.println("[BLYNK] Sistem Alarm Dinonaktifkan (DISARMED)");
  }
}

// =====================================================================================
// FUNGSI CEK SENSOR SECARA BERKALA (DIPANGGIL OLEH BLYNK TIMER)
// =====================================================================================
void periksaSensorPintu() {
  int statusSaatIni = digitalRead(PIN_SENSOR_MAGNET);

  // Jika terjadi perubahan kondisi (dari tertutup ke terbuka atau sebaliknya)
  if (statusSaatIni != statusSensorTerakhir) {
    statusSensorTerakhir = statusSaatIni;

    if (statusSaatIni == HIGH) {
      // PINTU TERBUKA (MAGNET LEPAS)
      Serial.println(">> [BAHAYA] Pintu Terbuka Terdeteksi!");
      
      // Update nilai ke widget Blynk (V0: 1 = Terbuka / Merah)
      Blynk.virtualWrite(V0, 1);

      // Kirim Notifikasi Push ke Smartphone jika sistem dalam status aktif
      if (sistemAktif) {
        // Nama event di Event Console Blynk: "door_breach" (atau sesuaikan dengan setting Blynk)
        Blynk.logEvent("door_breach", "PERINGATAN! Pintu rumah Anda terbuka!");
        Serial.println("[NOTIFIKASI] Push Notification terkirim ke Android!");
      }
    } else {
      // PINTU TERTUTUP (MAGNET MENEMPEL)
      Serial.println("[AMAN] Pintu Tertutup Rapat.");
      
      // Update nilai ke widget Blynk (V0: 0 = Tertutup / Hijau)
      Blynk.virtualWrite(V0, 0);

      // Matikan bunyi dan lampu alarm
      digitalWrite(PIN_BUZZER, LOW);
      digitalWrite(PIN_LED, LOW);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nMemulai Sistem Alarm Pintu ESP32 - Blynk IoT...");

  // Konfigurasi Pin
  pinMode(PIN_SENSOR_MAGNET, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);

  // Hubungkan ke WiFi dan Cloud Blynk
  Serial.print("Menghubungkan ke WiFi & Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Jadwalkan pemeriksaan sensor setiap 100 milidetik (sangat responsif tanpa membebani WiFi)
  timer.setInterval(100L, periksaSensorPintu);

  Serial.println("\n[BERHASIL] Terhubung ke Blynk Cloud!");
}

void loop() {
  // Jalankan service utama Blynk
  Blynk.run();
  timer.run();

  // Jika sistem sedang aktif DAN pintu terbuka, bunyikan alarm berkedip (pulsing buzzer)
  if (sistemAktif && statusSensorTerakhir == HIGH) {
    unsigned long waktuSekarang = millis();
    if (waktuSekarang - waktuKedipTerakhir >= 150) {
      waktuKedipTerakhir = waktuSekarang;
      statusBuzzerLed = !statusBuzzerLed;
      digitalWrite(PIN_BUZZER, statusBuzzerLed ? HIGH : LOW);
      digitalWrite(PIN_LED, statusBuzzerLed ? HIGH : LOW);
    }
  }
}
