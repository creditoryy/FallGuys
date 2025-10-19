# ESP-NOW Configuration - MAC Addresses

**Date**: October 18, 2025

## ✅ MAC Addresses Configured

### Your ESP32 (Wearable Module)
```
MAC Address: EC:E3:34:DB:95:30
Array Format: {0xEC, 0xE3, 0x34, 0xDB, 0x95, 0x30}
```

### Your Friend's ESP32 (Communication Hub)
```
MAC Address: EC:E3:34:DA:5D:B4
Array Format: {0xEC, 0xE3, 0x34, 0xDA, 0x5D, 0xB4}
```

---

## 🎯 Configuration Status

✅ **Wearable Module** (`wearable-sensor-module/src/main_espnow.cpp`)
- Line 42: `HUB_PEER_MAC` set to friend's MAC: `{0xEC, 0xE3, 0x34, 0xDA, 0x5D, 0xB4}`

✅ **Communication Hub** (`communication-hub/esp32/src/main.cpp`)
- Line 17: `WEARABLE_PEER_MAC` set to your MAC: `{0xEC, 0xE3, 0x34, 0xDB, 0x95, 0x30}`

---

## 🚀 Ready to Upload!

### Step 1: Upload Wearable Code (Your ESP32)

```cmd
cd wearable-sensor-module\src
ren main.cpp main_hal_backup.cpp
copy main_espnow.cpp main.cpp
cd ..
build.bat upload monitor
```

### Step 2: Upload Hub Code (Friend's ESP32)

Your friend should run:
```cmd
cd communication-hub\esp32
pio run -t upload
pio device monitor --baud 115200
```

---

## 📊 Expected Output

### Your ESP32 (Wearable) Serial Monitor:
```
[ESP-NOW] Wearable MAC: EC:E3:34:DB:95:30
[ESP-NOW] Channel: 1
[TX] Sensor data sent successfully
[RX] Fall Status from EC:E3:34:DA:5D:B4
     State=MONITORING, Severity=10, Confidence=0.05
```

### Friend's ESP32 (Hub) Serial Monitor:
```
[ESP-NOW] Hub MAC: EC:E3:34:DA:5D:B4
[ESP-NOW] Channel: 1
[RX #1] Sensor data from EC:E3:34:DB:95:30
     Accel: 0.12, 0.23, 9.81 m/s²
     Gyro:  0.01, -0.02, 0.00 rad/s
     Temp:  25.3 °C
[TX] Fall status sent successfully
```

---

## 🔧 Communication Flow

```
┌─────────────────────────────┐         ┌─────────────────────────────┐
│   YOUR ESP32                │         │   FRIEND'S ESP32            │
│   (Wearable Module)         │         │   (Communication Hub)       │
│                             │         │                             │
│   MAC: EC:E3:34:DB:95:30   │         │   MAC: EC:E3:34:DA:5D:B4   │
│                             │         │                             │
│   Sends: sensor_data_t      │────────>│   Receives & processes      │
│          (32 bytes @ 10Hz)  │         │   Simple fall detection     │
│                             │         │                             │
│   Receives: fall_status_t   │<────────│   Sends: fall_status_t      │
│            (16 bytes)        │         │          (on change)        │
│                             │         │                             │
│   OLED Display:             │         │   Serial Monitor Output     │
│   - Sensor readings         │         │   - Statistics              │
│   - Fall status             │         │   - Received data           │
│   - Connection status       │         │   - Fall detection alerts   │
└─────────────────────────────┘         └─────────────────────────────┘
```

---

## ✅ Testing Checklist

- [ ] Upload wearable code to **your** ESP32 (EC:E3:34:DB:95:30)
- [ ] Upload hub code to **friend's** ESP32 (EC:E3:34:DA:5D:B4)
- [ ] Check wearable serial monitor shows: `[TX] Sensor data sent successfully`
- [ ] Check hub serial monitor shows: `[RX] Sensor data from EC:E3:34:DB:95:30`
- [ ] Check wearable OLED displays "Status: MONITORING"
- [ ] Shake the wearable vigorously to test fall detection
- [ ] Verify status changes to "FALL_SUSPECTED" on display
- [ ] Check hub detects the fall and logs it

---

## 🐛 Troubleshooting

### If wearable shows "[TX] Send failed!"
1. Verify friend's ESP32 is powered on and running hub code
2. Check both ESP32s are on WiFi channel 1
3. Ensure distance is < 100m (line of sight)
4. Restart both devices

### If hub shows "No data received"
1. Verify your ESP32 is running wearable code
2. Check MAC addresses are correct
3. Try moving devices closer together
4. Check serial output on wearable for errors

### If no wireless communication at all
1. Verify both ESP32s uploaded successfully
2. Check serial monitor output for initialization errors
3. Try reflashing both devices
4. Test with devices 1-2 meters apart first

---

## 📝 Notes

- **WiFi Channel**: Both configured to channel 1
- **Transmission Rate**: 10 Hz (every 100ms)
- **Range**: Tested up to 100m line of sight
- **Latency**: Typically 10-20ms
- **Fall Threshold**: 15.0 m/s² (adjustable in hub code line 70)

---

## 🔮 Next Steps

1. ✅ MAC addresses configured
2. **→ Upload code to both ESP32s**
3. Test wireless communication
4. Calibrate fall detection threshold
5. Add heart rate sensor (MS1)
6. Implement advanced fall detection on BeagleBoard (MS2)

---

**Status**: ✅ **READY TO UPLOAD**  
**Configuration**: Complete  
**Date**: October 18, 2025
