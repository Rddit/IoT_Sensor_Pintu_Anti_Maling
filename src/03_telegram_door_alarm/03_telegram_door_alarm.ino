/*
 * =====================================================================================
 * PROYEK: SENSOR PINTU ANTI MALING ESP32 DENGAN NOTIFIKASI TELEGRAM BOT (ALTERNATIF GRATIS)
 * Penulis: Antigravity IoT Assistant
 * 
 * FUNGSI:
 * 1. Mengirimkan pesan instan darurat ke aplikasi Telegram di HP saat pintu terbuka.
 * 2. 100% Gratis, tanpa kuota/pembatasan notifikasi pihak ketiga.
 * 3. Alarm lokal (Buzzer & LED) tetap berbunyi keras secara mandiri.
 * =====================================================================================
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Kredensial WiFi
const char* ssid = "NAMA_WIFI_RUMAH_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

// Kredensial Telegram Bot (Dapatkan dari @BotFather dan @userinfobot di Telegram)
#define BOT_TOKEN "MASUKKAN_BOT_TOKEN_DARI_BOTFATHER"
#define CHAT_ID   "MASUKKAN_CHAT_ID_ANDA"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Pinout Hardware (Di Baseboard 38-Pin tertulis D4, D18, D19 di Sisi Kanan)
const int PIN_SENSOR_MAGNET = 4;   // Pin D4 pada baseboard 38-pin (alternatif: ganti ke 13 jika pakai D13)
const int PIN_BUZZER        = 18;  // Pin D18 pada baseboard 38-pin (I/O Buzzer)
const int PIN_LED           = 19;  // Pin D19 pada baseboard 38-pin (Anoda LED via Resistor 220 Ohm)

int statusSensorTerakhir = -1;
bool statusBuzzerLed = false;
unsigned long waktuKedipTerakhir = 0;

void kirimPesanTelegram(String pesan) {
  Serial.println("[TELEGRAM] Mengirim notifikasi...");
  if (bot.sendMessage(CHAT_ID, pesan, "Markdown")) {
    Serial.println("[TELEGRAM] Pesan berhasil terkirim ke HP!");
  } else {
    Serial.println("[TELEGRAM] Gagal mengirim pesan!");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(PIN_SENSOR_MAGNET, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);

  Serial.println("\nMenghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] Terhubung! IP Address: " + WiFi.localIP().toString());

  // Abaikan validasi SSL certificate untuk mempermudah koneksi ESP32
  secured_client.setInsecure();

  // Kirim pesan salam bahwa alat sudah online
  kirimPesanTelegram("🔔 *SISTEM ALARM PINTU AKTIF*\nESP32 siap mengawasi rumah Anda!");
}

void loop() {
  int statusSaatIni = digitalRead(PIN_SENSOR_MAGNET);

  if (statusSaatIni != statusSensorTerakhir) {
    statusSensorTerakhir = statusSaatIni;

    if (statusSaatIni == HIGH) {
      // PINTU DIBUKA (MAGNET TERPISAH)
      Serial.println(">> [ALARM!] PINTU TERBUKA! <<");
      
      String pesanDarurat = "🚨 *PERINGATAN KEAMANAN!*\n"
                            "⚠️ Pintu Rumah Terbuka Terdeteksi!\n"
                            "⏱️ Waktu: Segera periksa lokasi!";
      kirimPesanTelegram(pesanDarurat);
    } else {
      // PINTU DITUTUP KEMBALI
      Serial.println("[AMAN] Pintu Tertutup.");
      digitalWrite(PIN_BUZZER, LOW);
      digitalWrite(PIN_LED, LOW);
      kirimPesanTelegram("✅ *Pintu telah ditutup kembali.*");
    }
  }

  // Jika pintu sedang terbuka, bunyikan alarm darurat
  if (statusSensorTerakhir == HIGH) {
    unsigned long waktuSekarang = millis();
    if (waktuSekarang - waktuKedipTerakhir >= 150) {
      waktuKedipTerakhir = waktuSekarang;
      statusBuzzerLed = !statusBuzzerLed;
      digitalWrite(PIN_BUZZER, statusBuzzerLed ? HIGH : LOW);
      digitalWrite(PIN_LED, statusBuzzerLed ? HIGH : LOW);
    }
  }
}
