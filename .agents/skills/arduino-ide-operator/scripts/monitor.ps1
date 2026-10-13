param (
    [string]$Port = "",
    [int]$BaudRate = 115200
)

$arduinoCli = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

if (-not (Test-Path $arduinoCli)) {
    Write-Host "[ERROR] Arduino CLI tidak ditemukan!" -ForegroundColor Red
    exit 1
}

if ([string]::IsNullOrWhiteSpace($Port)) {
    $ports = [System.IO.Ports.SerialPort]::GetPortNames()
    if ($ports.Count -eq 0) {
        Write-Host "[ERROR] Tidak ada port COM yang terdeteksi! Colokkan ESP32 ke USB." -ForegroundColor Red
        exit 1
    }
    $Port = $ports[0]
}

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "  SERIAL MONITOR ANTIGRAVITY (ESP32)          " -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "Port     : $Port" -ForegroundColor Yellow
Write-Host "Baudrate : $BaudRate" -ForegroundColor Yellow
Write-Host "Tekan Ctrl+C untuk keluar dari Serial Monitor.`n" -ForegroundColor Gray

& $arduinoCli monitor -p $Port -c baudrate=$BaudRate
