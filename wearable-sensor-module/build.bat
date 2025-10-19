@echo off
REM Build script for FallGuys Project on Windows
REM Batch file version (no execution policy issues)

if "%1"=="" goto help
if "%1"=="help" goto help
if "%1"=="build" goto build
if "%1"=="upload" goto upload
if "%1"=="monitor" goto monitor
if "%1"=="clean" goto clean
goto help

:help
echo.
echo FallGuys Project - Build Commands (Windows)
echo ============================================
echo.
echo PlatformIO Commands (ESP32):
echo   build.bat build      - Build for ESP32
echo   build.bat upload     - Build and upload to ESP32
echo   build.bat monitor    - Open serial monitor
echo   build.bat clean      - Clean build artifacts
echo.
echo Or use PlatformIO directly:
echo   platformio run
echo   platformio run --target upload
echo   platformio device monitor --baud 115200
echo.
goto end

:build
echo Building for ESP32 with PlatformIO...
platformio run
goto end

:upload
echo Building and uploading to ESP32...
platformio run --target upload
goto end

:monitor
echo Opening serial monitor...
platformio device monitor --baud 115200
goto end

:clean
echo Cleaning build artifacts...
if exist build rmdir /s /q build
if exist .pio\build rmdir /s /q .pio\build
echo Clean complete!
goto end

:end
