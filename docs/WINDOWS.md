# Windows Quick Start Guide

This project is designed to work on Windows with PlatformIO. The CMake build system is optional and primarily for Linux/cross-platform testing.

## ✅ Recommended: PlatformIO (ESP32)

### Quick Commands (PowerShell)

```powershell
# Build for ESP32
platformio run

# Upload to ESP32
platformio run --target upload

# Open Serial Monitor
platformio device monitor --baud 115200

# Clean build
platformio run --target clean
```

### Or use the build script (easier):

**Option 1: Batch file (no execution policy issues):**
```cmd
build.bat help       # Show help
build.bat build      # Build
build.bat upload     # Upload
build.bat monitor    # Monitor
build.bat clean      # Clean
```

**Option 2: PowerShell script (if execution policy allows):**
```powershell
# Enable scripts first (run as Administrator, one time only):
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# Then use:
.\build.ps1 help
.\build.ps1 build
.\build.ps1 upload
.\build.ps1 monitor
.\build.ps1 clean
```

**Recommended:** Use `build.bat` - it works without any setup!



## 📝 Development Workflow

### For ESP32 Development (Recommended):

1. **Edit code** in VS Code
2. **Build**: `platformio run`
3. **Upload**: `platformio run --target upload`
4. **Debug**: Open serial monitor with `platformio device monitor --baud 115200`

### PlatformIO Integration in VS Code:

- Use the **PlatformIO toolbar** at the bottom of VS Code
- Click the **arrow** icon to upload
- Click the **plug** icon to open serial monitor
- No need to run commands manually!

## 🔍 Troubleshooting

### "platformio not recognized"
- Make sure PlatformIO is installed in VS Code
- Or install globally: `pip install platformio`

### "Permission denied on COM port"
- Close any open serial monitors
- Disconnect and reconnect the ESP32
- Check Device Manager for the correct COM port

## 📂 Project Structure

```
FallGuys/
├── platformio.ini      ← PlatformIO configuration
├── build.bat          ← Easy build commands
├── src/               ← Your application code
│   └── main.cpp      ← Main application (uses HAL)
└── hal/              ← Hardware Abstraction Layer
    ├── include/hal/  ← HAL interfaces (.h files)
    └── src/          ← HAL implementations (.cpp files)
```

## 🚀 Getting Started

1. **Open project** in VS Code
2. **PlatformIO will auto-detect** the `platformio.ini` file
3. **Edit** `src/main.cpp` with your code
4. **Click Upload** in PlatformIO toolbar (bottom of screen)
5. **Done!** Your code runs on ESP32

No CMake needed for normal development! 🎉

## 💡 Command Reference

| Task | Tool | Command |
|------|------|---------|
| **Build for ESP32** | PlatformIO | `build.bat build` |
| **Upload to ESP32** | PlatformIO | `build.bat upload` |
| **Serial Monitor** | PlatformIO | `build.bat monitor` |
| **Clean builds** | PlatformIO | `build.bat clean` |

## 📚 More Information

- **PlatformIO Docs**: https://docs.platformio.org/
- **ESP32 Arduino Docs**: https://docs.espressif.com/projects/arduino-esp32/
- **Project README**: See `README_HAL.md`

---

**TL;DR for Windows users:**
```cmd
build.bat upload
```
That's it! 🎯
