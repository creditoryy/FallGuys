# FallGuys ESP32 Project

A modular ESP32 project featuring MPU6050 IMU sensor and OLED display with proper Hardware Abstraction Layer (HAL) architecture.

## Project Structure

```
FallGuys/
├── platformio.ini         # PlatformIO configuration (ESP32)
├── README_HAL.md         # This file
├── README_QUICK.md       # Quick reference guide
├── build.bat             # Windows build script
├── src/                  # Application layer
│   └── main.cpp         # Main application code
└── hal/                  # Hardware Abstraction Layer
    ├── include/
    │   └── hal/
    │       ├── mpu6050.h  # MPU6050 interface
    │       └── oled.h     # OLED interface
    └── src/
        ├── mpu6050.cpp    # MPU6050 implementation (ESP32)
        └── oled.cpp       # OLED implementation (ESP32)
```

## Hardware Requirements

### Components
- ESP32-WROOM-32D development board
- MPU6050 IMU sensor (accelerometer + gyroscope)
- SSD1306 OLED display (128x64, I2C)

### Wiring Diagram

#### OLED Display (SSD1306)
| OLED Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC      | 5V        | Power supply |
| GND      | GND       | Ground |
| SCL/SCK  | GPIO 22   | I2C Clock |
| SDA      | GPIO 21   | I2C Data |

#### MPU6050 Sensor
| MPU6050 Pin | ESP32 Pin | Description |
|-------------|-----------|-------------|
| VCC         | 3.3V/5V   | Power supply |
| GND         | GND       | Ground |
| SCL         | GPIO 22   | I2C Clock (shared with OLED) |
| SDA         | GPIO 21   | I2C Data (shared with OLED) |
| XDA         | -         | Not connected |
| XCL         | -         | Not connected |
| ADD         | -         | Not connected (floating = 0x68) |
| INT         | -         | Not connected |

**Note:** Both devices share the same I2C bus (GPIO 21 & 22).

## Software Architecture

### Hardware Abstraction Layer (HAL)

The HAL provides clean interfaces to hardware components, making the code portable and testable.

#### MPU6050 HAL (`hal/mpu6050.h`)

**Functions:**
- `mpu6050_init()` - Initialize sensor
- `mpu6050_read_accel()` - Read accelerometer (m/s²)
- `mpu6050_read_gyro()` - Read gyroscope (rad/s)
- `mpu6050_read_temp()` - Read temperature (°C)
- `mpu6050_read_all()` - Read all sensors at once (efficient)
- `mpu6050_cleanup()` - Release resources

**Data Structures:**
```c
typedef struct {
    float x, y, z;  // Acceleration in m/s²
} mpu6050_accel_t;

typedef struct {
    float x, y, z;  // Rotation in rad/s
} mpu6050_gyro_t;

typedef struct {
    float celsius;  // Temperature in °C
} mpu6050_temp_t;
```

#### OLED HAL (`hal/oled.h`)

**Functions:**
- `oled_init()` - Initialize display
- `oled_clear()` - Clear display buffer
- `oled_display()` - Update physical display
- `oled_set_cursor()` - Set text cursor position
- `oled_set_text_size()` - Set text size (1-3)
- `oled_print()` - Print text
- `oled_println()` - Print text with newline
- `oled_printf()` - Print formatted text
- `oled_draw_hline()` - Draw horizontal line
- `oled_draw_vline()` - Draw vertical line
- `oled_draw_rect()` - Draw rectangle
- `oled_cleanup()` - Release resources

**Constants:**
- `OLED_WIDTH` = 128 pixels
- `OLED_HEIGHT` = 64 pixels
- `OLED_TEXT_SIZE_SMALL`, `MEDIUM`, `LARGE`

### Application Layer (`app/main.c`)

The main application demonstrates:
1. Hardware initialization
2. Welcome screen display
3. Continuous sensor data reading and display
4. Proper cleanup on exit

## Building the Project

### PlatformIO (For ESP32)

**Windows PowerShell:**
```powershell
# Build the project
platformio run

# Build and upload to ESP32
platformio run --target upload

# Monitor serial output
platformio device monitor --baud 115200
```

**Or use the convenient build script:**
```cmd
build.bat build      # Build
build.bat upload     # Build and upload
build.bat monitor    # Open serial monitor
build.bat clean      # Clean build files
```

## Usage

### PlatformIO (ESP32)

1. Connect hardware as per wiring diagram
2. Upload the code: `platformio run --target upload` or `build.bat upload`
3. Open serial monitor to see debug output: `build.bat monitor`
4. OLED will display:
   - Accelerometer data (X, Y, Z in m/s²)
   - Gyroscope data (X, Y, Z in rad/s)
   - Temperature (in °C)
5. Data updates 10 times per second

## Modifying the Code

### Adding New HAL Modules

1. Create header in `hal/include/hal/yourmodule.h`
2. Create implementation in `hal/src/yourmodule.c`
3. Follow the pattern:
   - `yourmodule_init()` for initialization
   - `yourmodule_cleanup()` for cleanup
   - Use `static bool is_initialized` flag
   - Use `assert()` for preconditions

### Changing Display Content

Edit `display_sensor_data()` in `app/src/main.c`:

```c
void display_sensor_data(void) {
    // Read sensors
    mpu6050_read_all(&accel, &gyro, &temp);
    
    // Clear and prepare display
    oled_clear();
    oled_set_cursor(0, 0);
    
    // Your custom display code here
    oled_println("Custom Text");
    oled_printf("Value: %.2f\n", accel.x);
    
    // Update display
    oled_display();
}
```

## Dependencies

### PlatformIO
- Platform: espressif32
- Framework: arduino
- Libraries (auto-installed):
  - Adafruit SSD1306
  - Adafruit GFX Library
  - Adafruit MPU6050
  - Adafruit Unified Sensor

## Troubleshooting

### OLED Not Working
- Check I2C address (usually 0x3C, sometimes 0x3D)
- Verify power: OLED needs 5V on some modules
- Check wiring: SDA→21, SCL→22
- Try swapping SDA/SCL if nothing appears

### MPU6050 Not Detected
- Verify I2C address (default 0x68, 0x69 if AD0 is high)
- Check power: Most MPU6050 work with 3.3V or 5V
- Verify wiring: SDA→21, SCL→22
- Run I2C scanner to detect device

### Boot Loop
- Check power supply (needs stable 5V/3.3V)
- Verify no short circuits
- Check serial monitor for error messages

## License

Educational project for ENSC 351 at SFU.

## Authors

- Project: FallGuys ESP32
- Course: ENSC 351 - Fall 2025
