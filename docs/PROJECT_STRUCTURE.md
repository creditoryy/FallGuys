# FallGuys Project Structure Overview

**Last Updated**: October 2025

## 📁 Directory Organization

```
FallGuys/
│
├── README.md                          # Main project documentation
├── .gitignore                         # Git ignore patterns
│
├── wearable-sensor-module/           # ESP32 Sensor Module (Windows)
│   ├── README.md                     # Module documentation
│   ├── platformio.ini                # PlatformIO config
│   ├── build.bat                     # Build script
│   ├── src/
│   │   └── main.cpp                  # Application code
│   └── hal/                          # Hardware Abstraction Layer
│       ├── include/hal/
│       │   ├── mpu6050.h            # IMU interface
│       │   ├── oled.h               # Display interface
│       │   └── heartrate.h          # Heart rate interface (TBD)
│       └── src/
│           ├── mpu6050.cpp          # IMU implementation
│           ├── oled.cpp             # Display implementation
│           └── heartrate.cpp        # Heart rate impl (TBD)
│
├── communication-hub/                # ESP32 + BeagleBoard (Linux)
│   ├── README.md                     # Module documentation
│   ├── esp32/                        # ESP32 communication firmware
│   │   ├── README.md
│   │   ├── platformio.ini
│   │   └── src/
│   └── beagleboard/                  # BeagleBoard processing
│       ├── README.md
│       ├── Makefile
│       ├── src/
│       └── include/
│
├── protocol/                         # Shared communication protocol
│   ├── README.md                     # Protocol specification
│   └── protocol.h                    # Protocol header file
│
├── docs/                             # Project documentation
│   ├── README_HAL.md                # HAL architecture guide
│   ├── README_QUICK.md              # Quick reference
│   ├── WINDOWS.md                   # Windows setup
│   ├── PROJECT_STRUCTURE_OLD.md     # Old structure reference
│   ├── README_OLD.md                # Original README
│   ├── design/                      # Design documents
│   ├── datasheets/                  # Component datasheets
│   ├── flowcharts/                  # System diagrams
│   └── milestones/                  # Milestone reports
│
└── testing/                          # Test scripts
    ├── README.md
    ├── sensor-calibration/
    ├── fall-detection-tests/
    └── integration-tests/
```

## 🎯 Component Responsibilities

### Wearable Sensor Module (ESP32 #1)
**Development**: Windows + PlatformIO  
**Purpose**: Data acquisition and user interface

**Responsibilities**:
- Read MPU-6050 (accelerometer/gyroscope)
- Read heart rate sensor
- Display status on OLED
- Local fall detection
- Transmit data to communication hub
- User interaction (button/touch input)

**Build Commands**:
```cmd
cd wearable-sensor-module
build.bat          # Build firmware
build.bat upload   # Upload to ESP32
build.bat monitor  # Serial monitor
```

### Communication Hub (ESP32 #2 + BeagleBoard)
**Development**: Linux + Makefile/CMake  
**Purpose**: Data processing and emergency response

**Components**:
1. **ESP32 Communication Controller**
   - Receive data from wearable
   - Forward to BeagleBoard
   - Handle protocol

2. **BeagleBoard Main Processor**
   - Advanced fall detection algorithm
   - GPS location services
   - Network connectivity (WiFi/Cellular)
   - Emergency service contact
   - Data logging

**Build Commands** (TBD):
```bash
cd communication-hub/esp32
make build
make upload

cd ../beagleboard
make
sudo ./build/fallguys-hub
```

### Protocol
**Purpose**: Communication standard between wearable and hub

**Features**:
- Binary packet format
- CRC-16 error detection
- Multiple packet types
- Standardized data structures

## 🔄 Data Flow

```
┌─────────────────────┐
│  Wearable Sensor    │
│  Module (ESP32 #1)  │
│                     │
│  • MPU-6050         │
│  • Heart Rate       │
│  • OLED Display     │
└──────────┬──────────┘
           │ UART/SPI
           │ (Protocol)
           ▼
┌─────────────────────┐
│  Communication Hub  │
│  ESP32 #2           │
│                     │
│  • Protocol Handler │
│  • Data Forwarding  │
└──────────┬──────────┘
           │ UART
           ▼
┌─────────────────────┐
│  BeagleBoard        │
│                     │
│  • Fall Detection   │
│  • GPS              │
│  • Network          │
│  • Emergency        │
└─────────────────────┘
```

## 🚀 Quick Start

### First Time Setup

1. **Clone Repository**
   ```bash
   git clone <repository-url>
   cd FallGuys
   ```

2. **Install PlatformIO**
   ```bash
   pip install platformio
   ```

3. **Build Wearable Module** (Windows)
   ```cmd
   cd wearable-sensor-module
   build.bat
   ```

### Development Workflow

**Working on Wearable Module** (Windows):
```cmd
cd wearable-sensor-module
# Edit files in src/ or hal/
build.bat upload monitor
```

**Working on Communication Hub** (Linux):
```bash
cd communication-hub/esp32
# Edit files in src/
make build
make upload
```

## 📚 Documentation Guide

| Document | Purpose | Audience |
|----------|---------|----------|
| `README.md` | Project overview | Everyone |
| `wearable-sensor-module/README.md` | Wearable module details | Windows developers |
| `communication-hub/README.md` | Hub module details | Linux developers |
| `protocol/README.md` | Communication protocol | Both teams |
| `docs/README_HAL.md` | HAL architecture | Developers |
| `docs/WINDOWS.md` | Windows setup | Windows users |

## 🔧 Adding New Features

### Adding a New Sensor to Wearable Module

1. **Create HAL Interface**
   ```
   wearable-sensor-module/hal/include/hal/newsensor.h
   ```

2. **Implement for ESP32**
   ```
   wearable-sensor-module/hal/src/newsensor.cpp
   ```

3. **Update main.cpp**
   ```cpp
   #include "hal/newsensor.h"
   ```

4. **Update platformio.ini** (if new library needed)
   ```ini
   lib_deps =
       ...
       New Sensor Library
   ```

5. **Test and Document**

### Modifying Communication Protocol

1. **Update protocol.h** with new packet types/structures
2. **Update protocol/README.md** documentation
3. **Implement in wearable module** (src/main.cpp)
4. **Implement in hub** (esp32/src/protocol.cpp)
5. **Test communication**

## 📊 Project Status

### Current Status (MS1 In Progress)

| Component | Status | Progress |
|-----------|--------|----------|
| Wearable - MPU6050 | ✅ Complete | 100% |
| Wearable - OLED | ✅ Complete | 100% |
| Wearable - Heart Rate | 🚧 Not Started | 0% |
| Wearable - HAL | ✅ Complete | 100% |
| Protocol Definition | ✅ Complete | 100% |
| Hub - ESP32 | ❌ Not Started | 0% |
| Hub - BeagleBoard | ❌ Not Started | 0% |
| Fall Detection Algo | ❌ Not Started | 0% |
| GPS Integration | ❌ Not Started | 0% |
| Emergency System | ❌ Not Started | 0% |

### Milestone Timeline

**MS1 (Current)**: Individual module testing
- [x] Project structure
- [x] Wearable sensor integration
- [ ] Heart rate sensor
- [ ] Protocol implementation
- [ ] Communication setup

**MS2 (Next)**: System integration
- [ ] Full communication
- [ ] Fall detection algorithm
- [ ] Emergency response
- [ ] Testing & validation

## 🤝 Team Coordination

### Windows Team (Wearable Module)
- Work in `wearable-sensor-module/`
- Use PlatformIO + Windows
- Focus on sensor data acquisition

### Linux Team (Communication Hub)
- Work in `communication-hub/`
- Use Makefile/CMake + Linux
- Focus on data processing & emergency response

### Shared Responsibilities
- Protocol definition (`protocol/`)
- Documentation (`docs/`)
- Testing (`testing/`)

## 📞 Key Contacts

- **Project Repository**: creditoryy/FallGuys
- **Course**: ENSC 351 - Fall 2025
- **Institution**: Simon Fraser University

---

**Quick Links**:
- [Main README](../README.md)
- [Wearable Module](../wearable-sensor-module/README.md)
- [Communication Hub](../communication-hub/README.md)
- [Protocol Spec](../protocol/README.md)
