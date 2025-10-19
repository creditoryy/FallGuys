# FallGuys Project - Directory Structure

Generated: October 18, 2025

```
FallGuys/                                    # Root repository
│
├── 📄 README.md                            # Main project documentation
├── 📄 MIGRATION_GUIDE.md                   # Guide for new structure
├── 📄 .gitignore                           # Git ignore patterns
│
├── 📁 wearable-sensor-module/              # ESP32 Sensor Module (Windows)
│   │
│   ├── 📄 README.md                        # Module documentation
│   ├── 📄 platformio.ini                   # PlatformIO configuration
│   ├── 📄 build.bat                        # Windows build script
│   ├── 📄 build.ps1                        # PowerShell build script
│   │
│   ├── 📁 src/
│   │   └── 📄 main.cpp                     # Main application
│   │
│   └── 📁 hal/                             # Hardware Abstraction Layer
│       ├── 📁 include/hal/
│       │   ├── 📄 mpu6050.h               # MPU-6050 interface
│       │   └── 📄 oled.h                  # OLED interface
│       └── 📁 src/
│           ├── 📄 mpu6050.cpp             # MPU-6050 implementation
│           └── 📄 oled.cpp                # OLED implementation
│
├── 📁 communication-hub/                   # ESP32 + BeagleBoard (Linux)
│   │
│   ├── 📄 README.md                        # Hub documentation
│   │
│   ├── 📁 esp32/                           # ESP32 #2 firmware
│   │   ├── 📄 README.md
│   │   └── 📁 src/                         # (To be implemented)
│   │
│   └── 📁 beagleboard/                     # BeagleBoard software
│       ├── 📄 README.md
│       ├── 📁 src/                         # (To be implemented)
│       └── 📁 include/                     # (To be implemented)
│
├── 📁 protocol/                            # Communication protocol
│   ├── 📄 README.md                        # Protocol specification
│   └── 📄 protocol.h                       # Protocol header file
│
├── 📁 docs/                                # Documentation
│   ├── 📄 README_HAL.md                   # HAL architecture guide
│   ├── 📄 README_QUICK.md                 # Quick reference
│   ├── 📄 WINDOWS.md                      # Windows setup guide
│   ├── 📄 PROJECT_STRUCTURE.md            # Structure overview
│   ├── 📄 PROJECT_STRUCTURE_OLD.md        # Old structure reference
│   ├── 📄 README_OLD.md                   # Original README
│   │
│   ├── 📁 design/                          # Design documents
│   ├── 📁 datasheets/                      # Component datasheets
│   ├── 📁 flowcharts/                      # System diagrams
│   └── 📁 milestones/                      # Milestone reports
│
├── 📁 testing/                             # Testing & validation
│   ├── 📄 README.md
│   ├── 📁 sensor-calibration/
│   ├── 📁 fall-detection-tests/
│   └── 📁 integration-tests/
│
└── 📁 resources/                           # Reference materials
    └── 📁 cmake_starter/                   # Original CMake template

```

## 🎯 Key Directories

### For Windows Development (Wearable Module)
```
wearable-sensor-module/     ← Your primary workspace
├── src/main.cpp           ← Main application code
├── hal/                   ← Hardware abstraction
├── platformio.ini         ← Build configuration
└── build.bat             ← Build & upload script
```

**Quick Start**:
```cmd
cd wearable-sensor-module
build.bat upload monitor
```

### For Linux Development (Communication Hub)
```
communication-hub/
├── esp32/                 ← ESP32 #2 firmware (future)
└── beagleboard/          ← BeagleBoard software (future)
```

**Quick Start** (when ready):
```bash
cd communication-hub/esp32
make upload
```

### Shared Components
```
protocol/                  ← Communication protocol
├── protocol.h            ← Shared header file
└── README.md             ← Protocol specification
```

## 📊 Development Status

| Component | Status | Location |
|-----------|--------|----------|
| ✅ Wearable Module | Active | `wearable-sensor-module/` |
| ✅ HAL Architecture | Complete | `wearable-sensor-module/hal/` |
| ✅ MPU-6050 Sensor | Working | `hal/src/mpu6050.cpp` |
| ✅ OLED Display | Working | `hal/src/oled.cpp` |
| ✅ Protocol Spec | Complete | `protocol/` |
| 🚧 Heart Rate Sensor | Planned | `hal/src/heartrate.cpp` (TBD) |
| 🚧 Communication Hub | Planned | `communication-hub/` |
| 🚧 Fall Detection Algo | Planned | `communication-hub/beagleboard/` |

Legend:
- ✅ Complete and working
- 🚧 Planned for future development
- ❌ Not yet started

## 🔄 Data Flow Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    WEARABLE MODULE                          │
│                  (ESP32-WROOM-32D)                          │
│                                                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐                │
│  │ MPU-6050 │  │Heart Rate│  │   OLED   │                │
│  │  Sensor  │  │  Sensor  │  │ Display  │                │
│  └────┬─────┘  └────┬─────┘  └────▲─────┘                │
│       │             │              │                       │
│       └─────┬───────┴──────────────┘                       │
│             │                                              │
│        ┌────▼─────┐                                        │
│        │   HAL    │                                        │
│        │  Layer   │                                        │
│        └────┬─────┘                                        │
│             │                                              │
│        ┌────▼─────┐                                        │
│        │ main.cpp │                                        │
│        └────┬─────┘                                        │
│             │                                              │
│        ┌────▼─────┐                                        │
│        │ Protocol │                                        │
│        └────┬─────┘                                        │
└─────────────┼────────────────────────────────────────────┘
              │ UART/SPI
              │ (Binary Protocol)
              ▼
┌─────────────────────────────────────────────────────────────┐
│              COMMUNICATION HUB - ESP32 #2                   │
│                                                             │
│        ┌────────────┐                                       │
│        │  Protocol  │                                       │
│        │  Handler   │                                       │
│        └────┬───────┘                                       │
└─────────────┼────────────────────────────────────────────┘
              │ UART
              ▼
┌─────────────────────────────────────────────────────────────┐
│                    BEAGLEBOARD                              │
│                                                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │     Fall     │  │     GPS      │  │   Network    │     │
│  │  Detection   │  │   Location   │  │ Connectivity │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│                                                             │
│  ┌──────────────────────────────────────────────────┐      │
│  │         Emergency Response System                │      │
│  └──────────────────────────────────────────────────┘      │
└─────────────────────────────────────────────────────────────┘
```

## 📝 File Purpose Quick Reference

### Wearable Module Files

| File | Purpose | Edit Frequency |
|------|---------|----------------|
| `src/main.cpp` | Main application logic | Often |
| `hal/include/hal/mpu6050.h` | MPU-6050 interface | Rarely |
| `hal/src/mpu6050.cpp` | MPU-6050 implementation | Sometimes |
| `hal/include/hal/oled.h` | OLED interface | Rarely |
| `hal/src/oled.cpp` | OLED implementation | Sometimes |
| `platformio.ini` | Build configuration | Rarely |
| `build.bat` | Build script | Never |

### Documentation Files

| File | Purpose | Reader |
|------|---------|--------|
| `README.md` | Project overview | Everyone |
| `MIGRATION_GUIDE.md` | New structure guide | Team members |
| `wearable-sensor-module/README.md` | Module documentation | Windows developers |
| `communication-hub/README.md` | Hub documentation | Linux developers |
| `protocol/README.md` | Protocol spec | Both teams |
| `docs/PROJECT_STRUCTURE.md` | Structure reference | Everyone |

### Protocol Files

| File | Purpose | Users |
|------|---------|-------|
| `protocol/protocol.h` | C/C++ header | Both ESP32 modules |
| `protocol/README.md` | Human-readable spec | Developers |

## 🚀 Common Tasks

### Building Wearable Module
```cmd
cd wearable-sensor-module
build.bat
```

### Uploading to ESP32
```cmd
cd wearable-sensor-module
build.bat upload
```

### Monitoring Serial Output
```cmd
cd wearable-sensor-module
build.bat monitor
```

### Adding New Sensor
1. Create `hal/include/hal/newsensor.h`
2. Create `hal/src/newsensor.cpp`
3. Update `src/main.cpp` to use it
4. Update `platformio.ini` if new library needed

### Viewing Documentation
- Main overview: `README.md`
- Your module: `wearable-sensor-module/README.md`
- Structure guide: `docs/PROJECT_STRUCTURE.md`
- Protocol: `protocol/README.md`

---

**Repository**: creditoryy/FallGuys  
**Last Updated**: October 2025  
**Status**: Active Development (MS1)
