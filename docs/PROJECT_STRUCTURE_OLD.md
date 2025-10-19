# FallGuys Project - Complete Structure

## Directory Tree

```
FallGuys/
│
├── platformio.ini              # PlatformIO configuration (ESP32)
├── build.bat                   # Windows build script
├── build.ps1                   # PowerShell build script (alternative)
├── README.md                   # Original project README
├── README_HAL.md              # Complete HAL documentation
├── README_QUICK.md            # Quick reference
├── WINDOWS.md                 # Windows-specific guide
│
├── src/                        # Application Layer
│   └── main.cpp               # Main application code (uses HAL)
│
├── hal/                        # Hardware Abstraction Layer
│   ├── include/
│   │   └── hal/
│   │       ├── mpu6050.h      # MPU6050 interface (accelerometer/gyro)
│   │       └── oled.h         # OLED display interface
│   │
│   └── src/
│       ├── mpu6050.cpp        # MPU6050 implementation (ESP32)
│       └── oled.cpp           # OLED implementation (ESP32)
│
└── resources/                  # Reference materials
    └── cmake_starter/          # Original starter template (reference only)
```

## Key Features

### 1. Hardware Abstraction Layer (HAL)
- **Modular design**: Each hardware component has its own module
- **Clean interfaces**: Well-defined APIs for each component
- **Portable**: Can be compiled for testing (CMake) or ESP32 (PlatformIO)
- **Testable**: Simulation mode for development without hardware

### 2. HAL Modules

#### MPU6050 Module (`hal/mpu6050.h`)
```c
// Initialize sensor
bool mpu6050_init(void);

// Read accelerometer (m/s²)
bool mpu6050_read_accel(mpu6050_accel_t *accel);

// Read gyroscope (rad/s)
bool mpu6050_read_gyro(mpu6050_gyro_t *gyro);

// Read temperature (°C)
bool mpu6050_read_temp(mpu6050_temp_t *temp);

// Read all at once (efficient)
bool mpu6050_read_all(mpu6050_accel_t *accel, 
                      mpu6050_gyro_t *gyro, 
                      mpu6050_temp_t *temp);

// Cleanup
void mpu6050_cleanup(void);
```

#### OLED Module (`hal/oled.h`)
```c
// Initialize display
bool oled_init(void);

// Display operations
void oled_clear(void);
void oled_display(void);

// Text operations
void oled_set_cursor(uint8_t x, uint8_t y);
void oled_set_text_size(oled_text_size_t size);
void oled_print(const char *str);
void oled_println(const char *str);
void oled_printf(const char *format, ...);

// Graphics operations
void oled_draw_hline(uint8_t x, uint8_t y, uint8_t width);
void oled_draw_vline(uint8_t x, uint8_t y, uint8_t height);
void oled_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill);

// Cleanup
void oled_cleanup(void);
```

### 3. Application Layer

The main application (`app/src/main.c`) demonstrates proper usage:

```c
int main(void) {
    // Initialize HAL modules
    oled_init();
    mpu6050_init();
    
    // Main loop
    while (running) {
        // Read sensor data
        mpu6050_read_all(&accel, &gyro, &temp);
        
        // Display data
        oled_clear();
        oled_printf("Accel: X=%.1f\n", accel.x);
        // ... more display code ...
        oled_display();
        
        delay(100);  // 10 Hz update
    }
    
    // Cleanup (reverse order)
    mpu6050_cleanup();
    oled_cleanup();
    
    return 0;
}
```

## Build System

### PlatformIO (For ESP32)

**Windows:**
```cmd
# Using build script (recommended)
build.bat build      # Build
build.bat upload     # Build and upload
build.bat monitor    # Serial monitor
build.bat clean      # Clean build

# Or use PlatformIO directly
platformio run
platformio run --target upload
platformio device monitor --baud 115200
```

**Linux/Mac:**
```bash
# Build
platformio run

# Upload to ESP32
platformio run --target upload

# Monitor
platformio device monitor --baud 115200
```

### Quick Reference

**Windows users:** See `WINDOWS.md` for detailed instructions.
**Everyone:** See `README_QUICK.md` for quick start guide.

## Hardware Connections

### I2C Bus Configuration
Both OLED and MPU6050 share the I2C bus:
- **SDA**: GPIO 21
- **SCL**: GPIO 22

### OLED (SSD1306)
- VCC → 5V
- GND → GND
- SCL → GPIO 22
- SDA → GPIO 21
- I2C Address: 0x3C

### MPU6050
- VCC → 3.3V or 5V
- GND → GND
- SCL → GPIO 22 (shared)
- SDA → GPIO 21 (shared)
- I2C Address: 0x68

## Design Principles

### 1. Separation of Concerns
- **HAL**: Hardware-specific code
- **App**: Business logic and user interface
- Clear boundaries between layers

### 2. Initialization Pattern
```c
static bool is_initialized = false;

bool module_init(void) {
    assert(!is_initialized);
    // ... initialization code ...
    is_initialized = true;
    return true;
}

void module_function(void) {
    assert(is_initialized);
    // ... function code ...
}

void module_cleanup(void) {
    assert(is_initialized);
    // ... cleanup code ...
    is_initialized = false;
}
```

### 3. Error Handling
- Functions return `bool` for success/failure
- Use `assert()` for preconditions
- Pointer parameters checked for NULL

### 4. Portability
- HAL can be compiled for testing (printf simulation)
- HAL can be compiled for ESP32 (actual hardware)
- TODO comments mark where ESP32-specific code goes

## Extending the Project

### Adding a New HAL Module

1. **Create header** (`hal/include/hal/newmodule.h`):
```c
#ifndef _NEWMODULE_H_
#define _NEWMODULE_H_

bool newmodule_init(void);
void newmodule_function(void);
void newmodule_cleanup(void);

#endif
```

2. **Create implementation** (`hal/src/newmodule.c`):
```c
#include "hal/newmodule.h"
#include <assert.h>

static bool is_initialized = false;

bool newmodule_init(void) {
    assert(!is_initialized);
    // ... init code ...
    is_initialized = true;
    return true;
}

void newmodule_cleanup(void) {
    assert(is_initialized);
    // ... cleanup code ...
    is_initialized = false;
}
```

3. **Use in application** (`app/src/main.c`):
```c
#include "hal/newmodule.h"

int main(void) {
    newmodule_init();
    // ... use module ...
    newmodule_cleanup();
}
```

## Advantages of This Architecture

1. **Testability**: Can test without hardware
2. **Maintainability**: Clear module boundaries
3. **Reusability**: HAL modules can be reused in other projects
4. **Scalability**: Easy to add new hardware modules
5. **Portability**: Can target different platforms
6. **Professional**: Industry-standard architecture

## Next Steps

1. **Build and test** the simulation (CMake)
2. **Connect hardware** as per wiring diagram
3. **Upload to ESP32** (PlatformIO)
4. **Verify** sensor readings on OLED
5. **Extend** with additional features

## Documentation

See `README_HAL.md` for complete documentation including:
- Detailed hardware setup
- Troubleshooting guide
- API reference
- Usage examples

---

**Project**: FallGuys ESP32 with HAL Architecture
**Course**: ENSC 351 - Fall 2025
**Architecture**: Based on cmake_starter template
