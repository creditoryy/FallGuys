# ESP-NOW Communication Setup Guide

## 📡 Overview

ESP-NOW is a fast, connectionless communication protocol developed by Espressif for ESP32 devices. It's ideal for the FallGuys project because:

- **Low Latency**: ~10-20ms vs WiFi's 100ms+
- **Low Power**: No WiFi association needed
- **Reliable**: Built-in acknowledgments
- **Simple**: Direct peer-to-peer communication

## 🔧 Step-by-Step Setup

### Step 1: Get Your Wearable ESP32's MAC Address

1. **Temporarily rename your current main.cpp**:
   ```cmd
   cd wearable-sensor-module\src
   ren main.cpp main_backup.cpp
   ```

2. **Copy the MAC address finder**:
   ```cmd
   copy get_mac_address.cpp main.cpp
   ```

3. **Upload and monitor**:
   ```cmd
   cd ..
   build.bat upload monitor
   ```

4. **Copy the MAC address** shown in the output:
   ```
   Array format: {0x24,0x0A,0xC4,0xXX,0xXX,0xXX}
   ```

5. **Restore original main**:
   ```cmd
   cd src
   del main.cpp
   ren main_backup.cpp main.cpp
   ```

### Step 2: Get Communication Hub's MAC Address

Repeat Step 1 on the **Communication Hub ESP32** to get its MAC address.

### Step 3: Configure ESP-NOW

1. **Edit `main_espnow.cpp`** (line 42):
   ```cpp
   // Replace with your Communication Hub's MAC address
   uint8_t HUB_PEER_MAC[6] = {0x24, 0x0A, 0xC4, 0xXX, 0xXX, 0xXX};
   ```

2. **On the Communication Hub**, set the wearable's MAC address (from Step 1)

### Step 4: Switch to ESP-NOW Version

**Option A: Temporarily test ESP-NOW**
```cmd
cd wearable-sensor-module\src
ren main.cpp main_hal.cpp
copy main_espnow.cpp main.cpp
cd ..
build.bat upload monitor
```

**Option B: Permanently use ESP-NOW**
```cmd
cd wearable-sensor-module\src
del main.cpp
ren main_espnow.cpp main.cpp
cd ..
build.bat upload monitor
```

## 📦 Data Structures

### Wearable → Hub (sensor_data_t)
```cpp
struct sensor_data_t {
    float accel_x, accel_y, accel_z;  // m/s²
    float gyro_x, gyro_y, gyro_z;     // rad/s
    float temperature;                 // °C
    uint32_t timestamp;                // milliseconds
};
// Size: 32 bytes
```

### Hub → Wearable (fall_status_t)
```cpp
struct fall_status_t {
    uint8_t state;          // 0=IDLE, 1=MONITORING, 2=FALL_SUSPECTED, 3=FALL_CONFIRMED
    uint8_t fall_severity;  // 0-255
    float fall_confidence;  // 0.0-1.0
    uint32_t timestamp;     // milliseconds
    uint8_t reserved[7];    // Reserved for future use
};
// Size: 16 bytes
```

## 🔄 Communication Flow

```
┌──────────────────┐                  ┌──────────────────┐
│   WEARABLE       │                  │  COMMUNICATION   │
│   ESP32 #1       │                  │  HUB ESP32 #2    │
└──────────────────┘                  └──────────────────┘
         │                                     │
         │  sensor_data_t (32 bytes)          │
         │────────────────────────────────────>│
         │        @ 10Hz (every 100ms)         │
         │                                     │
         │                                     │ Process data
         │                                     │ Run fall detection
         │                                     │
         │  fall_status_t (16 bytes)          │
         │<────────────────────────────────────│
         │      When status changes            │
         │                                     │
```

## 🖥️ OLED Display Layout

```
┌────────────────────────┐
│ === WEARABLE ===       │
│ Accel: 0.1 0.2 9.8     │
│ Gyro:  0.0 0.1 0.0     │
│ Temp:  25.3 C          │
│                        │
│ Status: MONITORING     │
│ Severity: 0%           │
│ [████░░░░░░░░░░░░░░]   │  ← Progress bar
│                        │
│ Sent:1234 Err:0        │
└────────────────────────┘
```

## ⚙️ Configuration

### Transmission Rate
Edit in `main_espnow.cpp` (line 230):
```cpp
if (now - lastSendMs >= 100) {  // 100ms = 10Hz
```

- **10 Hz (100ms)**: Default, good balance
- **20 Hz (50ms)**: High frequency for sensitive detection
- **5 Hz (200ms)**: Lower power consumption

### WiFi Channel
Both devices must use the same channel (line 43):
```cpp
const uint8_t WIFI_CHANNEL = 1;  // Must match hub
```

Channels 1, 6, 11 are typically least congested.

### MPU-6050 Ranges
Lines 123-125:
```cpp
mpu.setAccelerometerRange(MPU6050_RANGE_8_G);    // 2G, 4G, 8G, 16G
mpu.setGyroRange(MPU6050_RANGE_500_DEG);         // 250, 500, 1000, 2000 deg/s
mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);      // 5, 10, 21, 44, 94, 184, 260 Hz
```

## 🧪 Testing

### 1. Test MAC Address Discovery
```cmd
# Use get_mac_address.cpp
build.bat upload monitor
# Look for: "Array format: {0x24,0x0A,..."
```

### 2. Test Sensor Reading (without ESP-NOW)
```cmd
# Use original main.cpp
build.bat upload monitor
# Verify MPU-6050 and OLED work
```

### 3. Test ESP-NOW Transmission
```cmd
# Use main_espnow.cpp
build.bat upload monitor
# Look for: "[TX] Sensor data sent successfully"
```

### 4. Test Round-Trip Communication
```cmd
# With both ESP32s running
# Wearable should show: "[RX] Fall Status from..."
# Hub should show received sensor data
```

## 🐛 Troubleshooting

### "ESP-NOW Initialization failed"
- **Cause**: WiFi initialization issue
- **Fix**: Reset ESP32, reflash firmware

### "Failed to add peer"
- **Cause**: Incorrect MAC address format
- **Fix**: 
  1. Verify MAC address: `{0xXX,0xXX,0xXX,0xXX,0xXX,0xXX}`
  2. Check for typos
  3. Ensure 6 bytes, hexadecimal format

### "[TX] Send failed!"
- **Cause**: Peer not reachable or channel mismatch
- **Fix**:
  1. Verify both ESP32s on same WiFi channel
  2. Check distance (< 100m line of sight)
  3. Restart both devices

### "No reply" on display
- **Cause**: Hub not responding
- **Fix**:
  1. Verify hub is running
  2. Check hub has correct wearable MAC address
  3. Monitor hub serial output

### OLED shows "MPU6050 FAIL"
- **Cause**: Sensor not detected
- **Fix**:
  1. Check I2C wiring (SDA=GPIO21, SCL=GPIO22)
  2. Verify 3.3V or 5V power to MPU-6050
  3. Run I2C scanner to verify address (0x68)

### High error count (Err: number)
- **Cause**: ESP-NOW transmission failures
- **Fix**:
  1. Reduce distance between devices
  2. Check for WiFi interference
  3. Try different WiFi channel (1, 6, or 11)

## 📊 Performance Metrics

### Expected Performance

| Metric | Value |
|--------|-------|
| Latency | 10-20 ms |
| Packet size | 32 bytes (TX), 16 bytes (RX) |
| Transmission rate | 10 Hz (configurable) |
| Range | 100m+ (line of sight) |
| Success rate | >99% |
| Power consumption | ~80 mA (WiFi enabled) |

## 🔮 Future Enhancements

### Already Implemented
- ✅ Bidirectional communication
- ✅ Automatic acknowledgments
- ✅ Error counting
- ✅ Timestamp synchronization

### Planned Features
- [ ] Encryption (AES)
- [ ] Multiple peer support (mesh network)
- [ ] Adaptive transmission rate
- [ ] Battery level reporting
- [ ] Signal strength (RSSI) display

## 📝 Code Files

| File | Purpose |
|------|---------|
| `get_mac_address.cpp` | Get ESP32 MAC address |
| `main_espnow.cpp` | ESP-NOW enabled wearable code |
| `main.cpp` (original) | HAL-based wearable code (no wireless) |

## 🤝 Integration with Protocol

The ESP-NOW data structures are designed to be **compatible** with the protocol defined in `protocol/protocol.h`:

- `sensor_data_t` matches `PKT_SENSOR_DATA` (0x01)
- `fall_status_t` is simplified version of fall detection response

For full protocol support, wrap ESP-NOW payloads in protocol packets (START, LENGTH, TYPE, PAYLOAD, CRC, END).

## 📖 References

- [ESP-NOW Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- Original protocol: `protocol/README.md`

---

**Quick Start Command**:
```cmd
cd wearable-sensor-module
# Get MAC address
build.bat upload monitor
# Edit main_espnow.cpp with MAC addresses
# Upload ESP-NOW version
build.bat upload monitor
```

**Status**: Ready for testing  
**Last Updated**: October 2025
