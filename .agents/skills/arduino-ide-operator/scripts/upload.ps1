param (
    [string]$SketchPath = "src/01_standalone_door_alarm",
    [string]$Port = "",
    [string]$Fqbn = "esp32:esp32:esp32"
)

$arduinoCli = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

if (-not (Test-Path $arduinoCli)) {
    Write-Host "[ERROR] Arduino CLI tidak ditemukan!" -ForegroundColor Red
    exit 1
}

# Auto-detect COM port jika tidak diisi
if ([string]::IsNullOrWhiteSpace($Port)) {
    $ports = [System.IO.Ports.SerialPort]::GetPortNames()
    if ($ports.Count -eq 0) {
        Write-Host "[ERROR] Tidak ada port COM yang terdeteksi! Colokkan ESP32 ke USB terlebih dahulu." -ForegroundColor Red
        exit 1
    }
    $Port = $ports[0]
    Write-Host "[INFO] Menggunakan port terdeteksi otomatis: $Port" -ForegroundColor Cyan
}

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "  UPLOADING FIRMWARE KE ESP32                 " -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "Sketch Target : $SketchPath" -ForegroundColor Yellow
Write-Host "Target Port   : $Port" -ForegroundColor Yellow
Write-Host "Board FQBN    : $Fqbn" -ForegroundColor Yellow
Write-Host "Tips: Jika muncul 'Connecting........_____', tekan tombol BOOT di ESP32 selama 2 detik." -ForegroundColor Gray

& $arduinoCli upload -p $Port --fqbn $Fqbn $SketchPath

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[BERHASIL] Upload berhasil 100%! Firmware sudah aktif di ESP32." -ForegroundColor Green
} else {
    Write-Host "`n[GAGAL] Upload gagal! Pastikan port COM benar dan tekan tombol BOOT saat upload." -ForegroundColor Red
}
