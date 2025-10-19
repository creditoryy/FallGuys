# Wearable Sensor Module

**Platform**: ESP32-WROOM-32D  
**Development OS**: Windows  
**Build System**: PlatformIO

## 📋 Overview

The wearable sensor module is the primary data acquisition component of the FallGuys fall detection system. It collects real-time sensor data from multiple sources and provides immediate user feedback through an OLED display.

## 🔧 Hardware Components

| Component | Model | Interface | Address/Pin | Description |
|-----------|-------|-----------|-------------|-------------|
| Microcontroller | ESP32-WROOM-32D | - | - | Main processor |
| IMU Sensor | MPU-6050 | I2C | 0x68 | Accelerometer + Gyroscope |
| Display | SSD1306 OLED | I2C | 0x3C | 128x64 pixel display |
| Heart Rate Sensor | MAX30102 (TBD) | I2C | 0x57 | Pulse oximeter |
| Power | Li-Po Battery | - | - | Rechargeable battery |

### Pin Configuration

```
I2C Bus (Shared):
- SDA: GPIO 21
- SCL: GPIO 22

Power:
- OLED VCC: 5V (requires 5V, not 3.3V)
- MPU-6050 VCC: 3.3V or 5V
- Heart Rate VCC: 3.3V

LED:
- Built-in LED: GPIO 2
```

## 🏗️ Architecture

The module follows a **Hardware Abstraction Layer (HAL)** pattern:

```
wearable-sensor-module/
├── platformio.ini          # PlatformIO configuration
├── build.bat              # Windows build script
├── src/
│   └── main.cpp           # Application entry point
└── hal/                   # Hardware Abstraction Layer
    ├── include/hal/
    │   ├── mpu6050.h      # IMU sensor interface
    │   ├── oled.h         # Display interface
    │   └── heartrate.h    # Heart rate sensor interface (TBD)
    └── src/
        ├── mpu6050.cpp    # MPU-6050 implementation
        ├── oled.cpp       # OLED implementation
        └── heartrate.cpp  # Heart rate implementation (TBD)
```

### HAL Benefits
- **Platform Independence**: Hardware-specific code isolated in HAL
- **Easy Testing**: Mock implementations for unit testing
- **Maintainability**: Clear interface boundaries
- **Reusability**: HAL modules can be used in other projects

## 🚀 Quick Start

### Prerequisites
- PlatformIO IDE or Core installed
- USB driver for ESP32 (CH340/CP2102)
- Windows PowerShell or Command Prompt

### Building

```cmd
cd wearable-sensor-module
build.bat
```

### Uploading to ESP32

```cmd
build.bat upload
```

### Monitoring Serial Output

```cmd
build.bat monitor
```

### Combined Upload + Monitor

```cmd
build.bat upload monitor
```

## 📚 HAL Module Documentation

### MPU-6050 (IMU Sensor)

**Interface**: `hal/include/hal/mpu6050.h`

```c
// Initialize the sensor
bool mpu6050_init(void);

// Read accelerometer data
bool mpu6050_read_accel(mpu6050_accel_t* accel);

// Read gyroscope data
bool mpu6050_read_gyro(mpu6050_gyro_t* gyro);

// Read temperature
bool mpu6050_read_temp(mpu6050_temp_t* temp);

// Read all sensors at once
bool mpu6050_read_all(mpu6050_accel_t* accel, 
                      mpu6050_gyro_t* gyro, 
                      mpu6050_temp_t* temp);

// Cleanup
void mpu6050_cleanup(void);
```

**Configuration**:
- Accelerometer range: ±8G
- Gyroscope range: ±500 deg/s
- Filter bandwidth: 21 Hz

### OLED Display

**Interface**: `hal/include/hal/oled.h`

```c
// Initialize display
bool oled_init(void);

// Clear display
void oled_clear(void);

// Update display (must call after drawing)
void oled_display(void);

// Text operations
void oled_set_cursor(int x, int y);
void oled_print(const char* text);
void oled_println(const char* text);
void oled_printf(const char* format, ...);

// Graphics operations
void oled_draw_pixel(int x, int y);
void oled_draw_line(int x0, int y0, int x1, int y1);
void oled_draw_rect(int x, int y, int w, int h);
void oled_fill_rect(int x, int y, int w, int h);

// Cleanup
void oled_cleanup(void);
```

**Display Resolution**: 128x64 pixels

## 📊 Current Features

✅ **Implemented**:
- MPU-6050 accelerometer reading
- MPU-6050 gyroscope reading
- MPU-6050 temperature reading
- OLED display with real-time sensor data
- HAL architecture for sensors
- Windows build scripts

🚧 **In Progress**:
- Heart rate sensor integration
- Fall detection algorithm
- User state display (idle → suspected → alarm)
- Communication protocol with hub

❌ **Not Started**:
- Emergency alert system
- Battery management
- Power optimization
- Physical enclosure design

## 🧪 Testing

### Sensor Calibration
```cmd
# Run sensor calibration test
cd ..\testing\sensor-calibration
# (Test scripts TBD)
```

### Fall Detection Tests
```cmd
# Run fall detection algorithm tests
cd ..\testing\fall-detection-tests
# (Test scripts TBD)
```

## 🔍 Troubleshooting

### OLED Not Displaying
1. Check power: OLED requires **5V**, not 3.3V
2. Verify I2C address: Should be 0x3C (use I2C scanner)
3. Check wiring: SDA→GPIO21, SCL→GPIO22

### MPU-6050 Not Reading
1. Verify I2C address: Should be 0x68
2. Check power: 3.3V or 5V
3. Ensure I2C pull-up resistors present (usually on module)

### Upload Failed
1. Close all serial monitors
2. Hold BOOT button on ESP32 during upload
3. Check USB cable (must be data cable, not charge-only)

## 📡 Communication Protocol

The wearable module will communicate with the communication hub using:
- **Interface**: UART or SPI (TBD)
- **Baud Rate**: 115200 (if UART)
- **Packet Format**: See `protocol/README.md`

Data transmitted:
- Accelerometer (x, y, z)
- Gyroscope (x, y, z)
- Temperature
- Heart rate
- Fall detection status
- Battery level

## 🔋 Power Management

**Target Specifications**:
- Operating voltage: 3.3V
- Battery: Li-Po 3.7V, 500-1000mAh
- Operating time: 24+ hours
- Charging: USB Type-C

**Power Optimization** (Future):
- Deep sleep when idle
- Reduced sampling rate when stationary
- Display auto-off after timeout

## 📅 Development Status

### MS1 Checklist
- [x] MPU-6050 integration
- [x] OLED display integration
- [x] HAL architecture
- [ ] Heart rate sensor integration
- [ ] State display on OLED
- [ ] Communication protocol implementation

### MS2 Checklist
- [ ] Fall detection algorithm
- [ ] Vital-based state machine
- [ ] Communication with hub module
- [ ] System integration testing

## 📖 Additional Documentation

- **HAL Guide**: See `../docs/README_HAL.md`
- **Quick Reference**: See `../docs/README_QUICK.md`
- **Windows Setup**: See `../docs/WINDOWS.md`
- **Datasheets**: See `../docs/datasheets/`

## 🤝 Contributing

When adding new sensors or features:
1. Create HAL interface in `hal/include/hal/`
2. Implement for ESP32 in `hal/src/`
3. Update `main.cpp` to use HAL functions
4. Test thoroughly before committing
5. Update this README

---

**Last Updated**: October 2025  
**Status**: Active Development
