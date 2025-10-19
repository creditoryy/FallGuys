# FallGuys - Wearable Fall Detection System

ENSC 351 Project - Fall 2025

## 📋 Project Description

A wearable fall detection module designed to protect vulnerable individuals, particularly seniors, by detecting falls and facilitating rapid emergency response. The system uses an accelerometer/gyroscope for fall detection, heart rate monitoring for vital verification, and a communication system to relay critical information.

## 🎯 Key Features

- **Fall Detection** - Real-time detection using MPU-6050 (accelerometer + gyroscope)
- **Vitals Monitoring** - Heart rate sensor for pulse verification and filtering false alarms
- **Emergency Contact** - Automatic emergency service/caretaker notification
- **Location Tracking** - GPS location ping for emergency response
- **Smart Filtering** - Differentiates between actual falls and normal activities (jumping, sitting)
- **User Confirmation** - OLED display with "I'm OK?" prompt to prevent false alarms

## 🏗️ System Architecture

The system consists of two main components:

### 1. **Wearable Sensor Module** (ESP32 #1 - Windows Development)
- Location: `wearable-sensor-module/`
- Platform: ESP32-WROOM-32D
- Development OS: Windows
- Responsibilities:
  - MPU-6050 sensor data acquisition (accelerometer + gyroscope)
  - Heart rate sensor monitoring
  - OLED display for user interface
  - Local fall detection algorithms
  - Data transmission to communication module

### 2. **Communication Hub** (ESP32 #2 + BeagleBoard - Linux Development)
- Location: `communication-hub/`
- Platform: ESP32 + BeagleBoard
- Development OS: Linux
- Responsibilities:
  - Receive sensor data from wearable module
  - Process and analyze data on BeagleBoard
  - Emergency service/caretaker contact system
  - GPS location services
  - Network connectivity and alerts

## 📁 Project Structure

```
FallGuys/
├── README.md                          # This file
├── docs/                             # Project documentation
│   ├── design/                       # Design documents
│   ├── datasheets/                   # Component datasheets
│   ├── flowcharts/                   # System flowcharts & state diagrams
│   └── milestones/                   # Milestone reports
│
├── wearable-sensor-module/           # ESP32 Sensor Module (Windows)
│   ├── README.md                     # Module-specific documentation
│   ├── platformio.ini                # PlatformIO configuration
│   ├── build.bat                     # Windows build script
│   ├── src/                          # Application code
│   │   └── main.cpp                  # Main application
│   └── hal/                          # Hardware Abstraction Layer
│       ├── include/hal/
│       │   ├── mpu6050.h            # MPU-6050 interface
│       │   ├── oled.h               # OLED display interface
│       │   └── heartrate.h          # Heart rate sensor interface
│       └── src/
│           ├── mpu6050.cpp          # MPU-6050 implementation
│           ├── oled.cpp             # OLED implementation
│           └── heartrate.cpp        # Heart rate implementation
│
├── communication-hub/                # ESP32 + BeagleBoard (Linux)
│   ├── README.md                     # Module-specific documentation
│   ├── esp32/                        # ESP32 communication firmware
│   │   ├── platformio.ini
│   │   ├── Makefile
│   │   └── src/
│   └── beagleboard/                  # BeagleBoard processing code
│       ├── Makefile
│       ├── include/
│       └── src/
│
├── protocol/                         # Communication protocol definitions
│   ├── README.md                     # Protocol documentation
│   └── protocol.h                    # Shared protocol header
│
└── testing/                          # Test scripts and validation
    ├── sensor-calibration/
    ├── fall-detection-tests/
    └── integration-tests/
```

## 🎓 Team & Development

- **Course**: ENSC 351 - Embedded Systems
- **Term**: Fall 2025
- **Institution**: Simon Fraser University

### Development Environment Split
- **Wearable Module**: Windows + PlatformIO + ESP32
- **Communication Hub**: Linux + BeagleBoard + ESP32

## 🚀 Quick Start

### Wearable Sensor Module (Windows)
```cmd
cd wearable-sensor-module
build.bat upload
build.bat monitor
```

### Communication Hub (Linux)
```bash
cd communication-hub/esp32
make upload
make monitor
```

## 📊 Project Timeline

### Milestone 1 (MS1) - Independent Module Testing
**Status**: In Progress

- [x] Header files for software modules
- [x] HAL architecture for wearable sensors
- [x] MPU-6050 sensor integration
- [x] OLED display integration
- [ ] Heart rate sensor integration
- [ ] Communication protocol design
- [ ] OLED state display (idle → suspected → alarm)
- [ ] System flowchart/state diagram
- [ ] Physical design mockup

### Milestone 2 (MS2) - System Integration
**Status**: Not Started

- [ ] Wearable ↔ Communication module integration
- [ ] Fall detection algorithm implementation
- [ ] Vital-based state determination
- [ ] Emergency contact system
- [ ] System calibration with professional equipment
- [ ] Complete software integration

### Post-MS2 - Finalization
- [ ] User documentation (how-to guide)
- [ ] System documentation
- [ ] Feature table
- [ ] System calibration
- [ ] Final testing and validation

## 🔧 Hardware Components

### Wearable Sensor Module
- ESP32-WROOM-32D microcontroller
- MPU-6050 IMU (accelerometer + gyroscope)
- Heart rate sensor (e.g., MAX30102)
- SSD1306 OLED display (128x64, I2C)
- Power management (battery + charging)

### Communication Hub
- ESP32 (communication controller)
- BeagleBoard (main processor)
- GPS module
- Network connectivity (WiFi/Cellular)

## 📡 Communication Protocol

Communication between the wearable module and communication hub uses:
- **Physical**: UART/SPI/I2C (TBD)
- **Protocol**: Custom packet-based protocol
- **Data**: Sensor readings, fall detection status, vitals

See `protocol/README.md` for detailed specifications.

## 🔬 Fall Detection Algorithm

The system uses a multi-stage detection process:

1. **Motion Detection** - Accelerometer detects sudden acceleration changes
2. **Orientation Check** - Gyroscope verifies fall trajectory
3. **Vital Verification** - Heart rate sensor confirms consciousness
4. **User Confirmation** - OLED prompt: "I'm OK?"
5. **Emergency Decision** - Based on vitals + user response

### False Alarm Prevention
- Heart rate stability check during motion
- Motion pattern analysis (jump vs. fall)
- User confirmation prompt
- Configurable sensitivity thresholds

## 📚 Documentation

- **Design Documents**: See `docs/design/`
- **Datasheets**: See `docs/datasheets/`
- **System Diagrams**: See `docs/flowcharts/`
- **Milestone Reports**: See `docs/milestones/`

## 🤝 Contributing

This is an academic project for ENSC 351. Development follows the milestone timeline outlined above.

### Development Workflow
1. Create feature branch from `main`
2. Develop and test in respective module directory
3. Submit PR with description
4. Code review and merge

## 📄 License

Educational project - ENSC 351, Fall 2025

---

**Repository**: creditoryy/FallGuys
**Last Updated**: October 2025
