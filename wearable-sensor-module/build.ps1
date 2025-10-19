# Build script for FallGuys Project on Windows
# PowerShell script for easy building

param(
    [Parameter(Position=0)]
    [string]$Command = "help"
)

function Show-Help {
    Write-Host ""
    Write-Host "FallGuys Project - Build Commands (Windows)" -ForegroundColor Cyan
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "PlatformIO Commands (ESP32):" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 build      - Build for ESP32"
    Write-Host "  .\build.ps1 upload     - Build and upload to ESP32"
    Write-Host "  .\build.ps1 monitor    - Open serial monitor"
    Write-Host "  .\build.ps1 clean      - Clean build artifacts"
    Write-Host ""
    Write-Host "CMake Commands (Simulation - requires Visual Studio):" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 cmake      - Build with CMake (simulation)"
    Write-Host "  .\build.ps1 run-sim    - Run CMake simulation"
    Write-Host ""
    Write-Host "Other:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 help       - Show this help"
    Write-Host ""
}

function Build-PlatformIO {
    Write-Host "Building for ESP32 with PlatformIO..." -ForegroundColor Green
    platformio run
}

function Upload-PlatformIO {
    Write-Host "Building and uploading to ESP32..." -ForegroundColor Green
    platformio run --target upload
}

function Monitor-PlatformIO {
    Write-Host "Opening serial monitor..." -ForegroundColor Green
    platformio device monitor --baud 115200
}

function Clean-Build {
    Write-Host "Cleaning build artifacts..." -ForegroundColor Green
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
        Write-Host "Removed build directory"
    }
    if (Test-Path ".pio\build") {
        Remove-Item -Recurse -Force ".pio\build"
        Write-Host "Removed PlatformIO build directory"
    }
    Write-Host "Clean complete!" -ForegroundColor Green
}

function Build-CMake {
    Write-Host "Building with CMake..." -ForegroundColor Green
    Write-Host "Note: Requires Visual Studio or MinGW installed" -ForegroundColor Yellow
    
    if (-not (Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" | Out-Null
    }
    
    Set-Location "build"
    
    # Try to detect Visual Studio
    if (Get-Command "cmake" -ErrorAction SilentlyContinue) {
        Write-Host "Configuring CMake..."
        cmake .. -G "Visual Studio 17 2022"
        
        Write-Host "Building..."
        cmake --build . --config Release
    } else {
        Write-Host "ERROR: CMake not found!" -ForegroundColor Red
        Write-Host "Please install CMake or use PlatformIO for ESP32 development" -ForegroundColor Yellow
    }
    
    Set-Location ..
}

function Run-Simulation {
    if (Test-Path "build\Release\fallguys.exe") {
        Write-Host "Running simulation..." -ForegroundColor Green
        & "build\Release\fallguys.exe"
    } elseif (Test-Path "build\Debug\fallguys.exe") {
        Write-Host "Running simulation (Debug)..." -ForegroundColor Green
        & "build\Debug\fallguys.exe"
    } else {
        Write-Host "ERROR: Simulation executable not found!" -ForegroundColor Red
        Write-Host "Run '.\build.ps1 cmake' first to build" -ForegroundColor Yellow
    }
}

# Main command dispatcher
switch ($Command.ToLower()) {
    "build" {
        Build-PlatformIO
    }
    "upload" {
        Upload-PlatformIO
    }
    "monitor" {
        Monitor-PlatformIO
    }
    "clean" {
        Clean-Build
    }
    "cmake" {
        Build-CMake
    }
    "run-sim" {
        Run-Simulation
    }
    "help" {
        Show-Help
    }
    default {
        Write-Host "Unknown command: $Command" -ForegroundColor Red
        Show-Help
    }
}
