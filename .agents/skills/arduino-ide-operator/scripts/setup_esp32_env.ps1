<#
.SYNOPSIS
    Script otomatis untuk setup lingkungan kerja Arduino CLI & ESP32 pada Antigravity.
#>

$arduinoCli = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
$esp32Url = "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "  SETUP LINGKUNGAN ARDUINO ESP32 ANTIGRAVITY " -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan

if (-not (Test-Path $arduinoCli)) {
    Write-Host "[ERROR] Arduino IDE / CLI tidak ditemukan!" -ForegroundColor Red
    exit 1
}

# 1. Update Board Index
Write-Host "[1/3] Memperbarui index board ESP32..." -ForegroundColor Yellow
& $arduinoCli core update-index --additional-urls $esp32Url

# 2. Pastikan Core ESP32 Terinstall
Write-Host "`n[2/3] Memeriksa & menginstal board esp32:esp32..." -ForegroundColor Yellow
& $arduinoCli core install esp32:esp32 --additional-urls $esp32Url

# 3. Pastikan Library Proyek Terinstall
Write-Host "`n[3/3] Memeriksa & menginstal library IoT (Blynk, Telegram, ArduinoJson)..." -ForegroundColor Yellow
& $arduinoCli lib install "Blynk" "UniversalTelegramBot" "ArduinoJson"

Write-Host "`n[SELESAI] Antigravity telah siap 100% untuk mengompilasi dan meng-upload program ESP32!" -ForegroundColor Green
