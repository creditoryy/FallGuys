@echo off
REM Get MAC Address Helper Script
REM This script helps you get your ESP32's MAC address

echo ===============================================
echo   FallGuys - Get ESP32 MAC Address
echo ===============================================
echo.
echo STEP 1: Backup current main.cpp
cd src
if exist main.cpp (
    echo Backing up main.cpp to main_backup.cpp...
    copy main.cpp main_backup.cpp >nul
    echo Done.
) else (
    echo No main.cpp found, skipping backup.
)
echo.

echo STEP 2: Copy MAC address finder
copy get_mac_address.cpp main.cpp >nul
echo Done.
echo.

echo STEP 3: Build and upload
cd ..
echo Building...
pio run -t upload
echo.

echo STEP 4: Monitor serial output
echo.
echo ===============================================
echo   COPY THE MAC ADDRESS FROM OUTPUT BELOW
echo ===============================================
echo.
pio device monitor --baud 115200
echo.

echo STEP 5: Restore original main.cpp
cd src
del main.cpp
if exist main_backup.cpp (
    ren main_backup.cpp main.cpp
    echo Original main.cpp restored.
) else (
    echo Warning: No backup found!
)
cd ..

echo.
echo ===============================================
echo   MAC Address retrieval complete!
echo ===============================================
pause
