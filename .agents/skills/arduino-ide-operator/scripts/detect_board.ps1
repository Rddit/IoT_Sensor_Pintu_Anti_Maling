<#
.SYNOPSIS
    Mendeteksi board ESP32 dan COM Port yang terhubung pada sistem Windows.
#>

$arduinoCli = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "  PENCARIAN BOARD ESP32 & PORT SERIAL (COM)   " -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan

# 1. Cek lewat Arduino CLI
if (Test-Path $arduinoCli) {
    Write-Host "[1] Memeriksa via Arduino CLI..." -ForegroundColor Yellow
    & $arduinoCli board list
}

# 2. Cek lewat SerialPort .NET
Write-Host "`n[2] Port Serial Aktif pada Windows:" -ForegroundColor Yellow
$ports = [System.IO.Ports.SerialPort]::GetPortNames()

if ($ports.Count -eq 0) {
    Write-Host "  [!] Tidak ada port COM yang terdeteksi." -ForegroundColor Red
    Write-Host "      Pastikan kabel USB sudah tercolok kencang antara ESP32 dan laptop/PC." -ForegroundColor Gray
    Write-Host "      Jika belum terdeteksi, periksa driver CP2102 atau CH340 di Device Manager." -ForegroundColor Gray
} else {
    foreach ($p in $ports) {
        Write-Host "  -> Port Ditemukan: $p" -ForegroundColor Green
    }
}
