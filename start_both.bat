@echo off
cd /d "%~dp0"
echo ========================================================
echo  Uruchamianie Systemu Zarzadzania Wypozyczalnia Mediow
echo ========================================================
echo.
echo Otwieranie dwoch niezaleznych terminali:
echo  1. Panel Administratora (library_app.exe)
echo  2. Panel Uzytkownika (user_app.exe)
echo.

start "Panel Administratora - Biblioteka Mediow" cmd /k "bin\library_app.exe"
start "Panel Uzytkownika - Biblioteka Mediow" cmd /k "bin\user_app.exe"
