$CurrentDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $CurrentDir

Write-Host "Uruchamianie dwoch niezaleznych terminali:" -ForegroundColor Cyan
Write-Host " 1. Panel Administratora (library_app.exe)" -ForegroundColor Green
Write-Host " 2. Panel Uzytkownika (user_app.exe)" -ForegroundColor Yellow

Start-Process cmd -ArgumentList '/k', 'bin\library_app.exe'
Start-Process cmd -ArgumentList '/k', 'bin\user_app.exe'
