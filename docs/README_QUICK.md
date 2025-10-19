# FallGuys ESP32 - HAL Architecture Quick Reference

## 🎯 Quick Start

```cmd
build.bat upload    # Build and upload to ESP32
build.bat monitor   # View serial output
```

## 📁 Project Structure

```
FallGuys/
├── src/main.cpp           # Your application code
├── hal/                   # Hardware Abstraction Layer
│   ├── include/hal/
│   │   ├── mpu6050.h     # MPU6050 sensor interface
│   │   └── oled.h        # OLED display interface
│   └── src/
│       ├── mpu6050.cpp   # MPU6050 implementation
│       └── oled.cpp      # OLED implementation
└── platformio.ini         # Build configuration
```

## 🔌 Wiring

**I2C Bus (GPIO 21 = SDA, GPIO 22 = SCL):**
- OLED: VCC→5V, GND→GND, SDA→21, SCL→22
- MPU6050: VCC→3.3V/5V, GND→GND, SDA→21, SCL→22

## 💡 Using the HAL

### MPU6050 Sensor
```cpp
#include "hal/mpu6050.h"

mpu6050_init();  // Initialize

mpu6050_accel_t accel;
mpu6050_gyro_t gyro;
mpu6050_temp_t temp;
mpu6050_read_all(&accel, &gyro, &temp);

Serial.printf("X: %.2f m/s²\n", accel.x);
```

### OLED Display
```cpp
#include "hal/oled.h"

oled_init();  // Initialize

oled_clear();
oled_println("Hello World");
oled_printf("Value: %.2f\n", 123.45);
oled_display();  // Update screen
```

## 📚 Full Documentation

See `README_HAL.md` for complete API documentation and examples.

---

**Platform:** ESP32 + PlatformIO  
**Architecture:** Hardware Abstraction Layer (HAL)  
**Course:** ENSC 351 - Fall 2025
