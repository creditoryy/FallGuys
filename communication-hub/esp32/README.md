# Communication Hub - ESP32 Firmware

## 📦 Two Versions Available

### 1. **Arduino IDE Version** ⭐ (Recommended for beginners)
**File**: `FallGuys_Hub_Receiver.ino`
- Single file, ready to use
- Open in Arduino IDE and upload
- MAC addresses pre-configured
- Enhanced formatted output
- **See**: `ARDUINO_IDE_SETUP.md` for complete guide

### 2. **PlatformIO Version** (Advanced users)
**File**: `src/main.cpp`
- Professional build system
- Better for development
- Requires PlatformIO installation

---

## ✅ Status: READY TO USE

Both versions are fully implemented and configured with your MAC addresses:
- **Hub MAC**: `EC:E3:34:DA:5D:B4`
- **Wearable MAC**: `EC:E3:34:DB:95:30`

---

## 🚀 Quick Start (Arduino IDE)

1. Install Arduino IDE
2. Add ESP32 board support
3. Open `FallGuys_Hub_Receiver.ino`
4. Click Upload
5. Open Serial Monitor (115200 baud)

**Full instructions**: See `ARDUINO_IDE_SETUP.md`

---

## 📁 Files in This Directory

| File | Purpose | For |
|------|---------|-----|
| `FallGuys_Hub_Receiver.ino` | Arduino IDE sketch | Arduino IDE users ⭐ |
| `ARDUINO_IDE_SETUP.md` | Complete setup guide | Arduino IDE users |
| `QUICK_START.txt` | Quick reference | Arduino IDE users |
| `src/main.cpp` | PlatformIO version | PlatformIO users |
| `platformio.ini` | Build configuration | PlatformIO users |

---

## 🎯 Features

- ✅ ESP-NOW wireless communication
- ✅ Receives sensor data from wearable
- ✅ Simple fall detection algorithm
- ✅ Sends fall status back to wearable
- ✅ Statistics reporting every 10 seconds
- ✅ Formatted console output
- ✅ MAC addresses pre-configured

---

## 📊 Expected Output

```
╔════════════════════════════════════════════════╗
║       FallGuys Communication Hub (ESP32)       ║
╚════════════════════════════════════════════════╝

✓ ESP-NOW initialized
✓ Hub Ready - Waiting for sensor data...

[RX #1] Sensor Data Received
From: EC:E3:34:DB:95:30
Accel:  X=0.12  Y=0.23  Z=9.81 m/s²
[TX] ✓ Fall status sent successfully
```

---

## 🔧 Development Notes

- **Platform**: ESP32 (any variant)
- **Framework**: Arduino
- **Communication**: ESP-NOW
- **WiFi Channel**: 1
- **Baud Rate**: 115200

Future integration with BeagleBoard will be added in MS2.

---

**For complete setup instructions, see**: `ARDUINO_IDE_SETUP.md`
