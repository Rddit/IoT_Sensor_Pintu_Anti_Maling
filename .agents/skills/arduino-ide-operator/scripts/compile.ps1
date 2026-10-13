param (
    [string]$SketchPath = "src/01_standalone_door_alarm",
    [string]$Fqbn = "esp32:esp32:esp32"
)

$arduinoCli = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

if (-not (Test-Path $arduinoCli)) {
    Write-Host "[ERROR] Arduino CLI tidak ditemukan di path default!" -ForegroundColor Red
    exit 1
}

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "  COMPILING SKETCH ESP32 DENGAN ANTIGRAVITY   " -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "Sketch Target : $SketchPath" -ForegroundColor Yellow
Write-Host "Board FQBN    : $Fqbn" -ForegroundColor Yellow
Write-Host "Sedang mengompilasi program, mohon tunggu..." -ForegroundColor Gray

& $arduinoCli compile --fqbn $Fqbn $SketchPath

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[BERHASIL] Kompilasi selesai tanpa error!" -ForegroundColor Green
    Write-Host "Program siap di-upload ke board ESP32." -ForegroundColor Green
} else {
    Write-Host "`n[GAGAL] Terjadi kesalahan saat kompilasi! Periksa baris error di atas." -ForegroundColor Red
}
