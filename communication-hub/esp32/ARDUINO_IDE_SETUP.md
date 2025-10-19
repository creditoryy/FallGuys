# Arduino IDE Setup Guide for Communication Hub

**For Your Friend - Communication Hub ESP32**

## 📋 What This Is

This is the **receiver** code that runs on your friend's ESP32. It receives sensor data from your wearable module via ESP-NOW and sends back fall detection status.

---

## 🔧 Arduino IDE Installation

### Step 1: Install Arduino IDE

1. Download from: https://www.arduino.cc/en/software
2. Install the latest version (2.x or 1.8.19+)
3. Open Arduino IDE

### Step 2: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to: **File → Preferences**
3. In "Additional Board Manager URLs", paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to: **Tools → Board → Boards Manager...**
6. Search for "ESP32"
7. Install "**esp32 by Espressif Systems**" (latest version)
8. Wait for installation to complete

---

## 📂 File Setup

### Step 1: Get the Code File

The file is located at:
```
FallGuys/communication-hub/esp32/FallGuys_Hub_Receiver.ino
```

### Step 2: Open in Arduino IDE

1. Open Arduino IDE
2. **File → Open**
3. Navigate to `FallGuys_Hub_Receiver.ino`
4. Click **Open**

OR simply:
1. Double-click `FallGuys_Hub_Receiver.ino`
2. It should open in Arduino IDE automatically

---

## ⚙️ Configure Arduino IDE

### Board Settings

1. **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
   (Or select your specific ESP32 board if different)

2. **Tools → Upload Speed → 921600**

3. **Tools → CPU Frequency → 240MHz (WiFi/BT)**

4. **Tools → Flash Size → 4MB (32Mb)**

5. **Tools → Partition Scheme → Default 4MB with spiffs**

6. **Tools → Core Debug Level → None** (or Info for debugging)

### Port Selection

1. Connect your ESP32 to computer via USB
2. **Tools → Port → Select your COM port**
   - Windows: Usually COM3, COM4, COM5, etc.
   - The port with your ESP32 will appear when it's connected

---

## 📤 Upload the Code

### Step 1: Verify Code

1. Click the **✓ (Verify)** button in Arduino IDE
2. Wait for compilation
3. Check for "Done compiling" message

### Step 2: Upload

1. Click the **→ (Upload)** button
2. Wait for upload to complete
3. You should see:
   ```
   Writing at 0x00010000... (100%)
   Wrote XXXXX bytes
   Hash of data verified.
   Leaving...
   Hard resetting via RTS pin...
   ```

### Troubleshooting Upload Issues

**If upload fails:**

1. **Hold BOOT button** on ESP32 during upload
2. Try a different USB cable (must be data cable, not charge-only)
3. Try a different USB port
4. Reduce upload speed: **Tools → Upload Speed → 115200**

---

## 🖥️ Open Serial Monitor

### Step 1: Open Monitor

1. After upload completes, click **Tools → Serial Monitor**
2. OR press **Ctrl+Shift+M**
3. Set baud rate to: **115200**
4. Set line ending to: **Newline** or **Both NL & CR**

### Step 2: Reset ESP32

Press the **RST** (reset) button on your ESP32

### Step 3: Expected Output

You should see:
```
╔════════════════════════════════════════════════╗
║                                                ║
║       FallGuys Communication Hub (ESP32)       ║
║           ESP-NOW Receiver Module              ║
║                                                ║
╚════════════════════════════════════════════════╝

Initializing ESP-NOW...
✓ ESP-NOW initialized
✓ Peer added successfully

─────────────────────────────────────
Configuration:
─────────────────────────────────────
Hub MAC Address:      EC:E3:34:DA:5D:B4
Wearable MAC Address: EC:E3:34:DB:95:30
WiFi Channel:         1
─────────────────────────────────────

✓ Hub Ready - Waiting for sensor data...
```

---

## 📡 Testing Communication

### When Wearable Sends Data

You'll see output like:
```
─────────────────────────────────────
[RX #1] Sensor Data Received
─────────────────────────────────────
From: EC:E3:34:DB:95:30
Accel:  X=0.12  Y=0.23  Z=9.81 m/s²
Gyro:   X=0.01  Y=-0.02  Z=0.00 rad/s
Temp:   25.3 °C
Magnitude: 9.85 m/s²
[TX] ✓ Fall status sent successfully
```

### When Fall is Detected

You'll see:
```
╔════════════════════════════════════╗
║  ⚠️  FALL SUSPECTED!  ⚠️           ║
╚════════════════════════════════════╝
Acceleration magnitude: 18.50 m/s²
```

### Statistics Report (Every 10 seconds)

```
╔════════════════════════════════════╗
║         STATISTICS REPORT          ║
╚════════════════════════════════════╝
Packets Received: 123
Packets Sent:     123
Current State:    MONITORING
Fall Magnitude:   9.85 m/s²
Uptime:           120 seconds
════════════════════════════════════
```

---

## 🐛 Troubleshooting

### Issue: "ESP-NOW initialization failed!"

**Solution:**
1. Restart ESP32 (press RST button)
2. Re-upload the code
3. Check ESP32 is genuine (some clones have WiFi issues)

### Issue: "Failed to add peer!"

**Solution:**
1. Check wearable MAC address is correct in code (line 31)
2. Restart ESP32
3. Re-upload code

### Issue: No data received

**Checklist:**
- ✅ Wearable ESP32 is running and uploaded with correct code
- ✅ Both ESP32s are powered on
- ✅ MAC addresses are correct in both codes
- ✅ Both on WiFi channel 1
- ✅ Distance < 100 meters
- ✅ Serial monitor shows "✓ Hub Ready"

**Steps:**
1. Check wearable serial monitor shows: `[TX] Sensor data sent successfully`
2. Verify wearable MAC in code matches actual: `EC:E3:34:DB:95:30`
3. Restart both ESP32s
4. Try moving devices closer (1-2 meters apart)

### Issue: Upload fails - "Permission denied"

**Solution:**
1. Close Serial Monitor (must be closed during upload)
2. Disconnect and reconnect USB
3. Select correct COM port again
4. Try upload again

### Issue: Can't find COM port

**Solution:**
1. Install CH340 or CP2102 USB driver:
   - CH340: https://bit.ly/44WdzVF
   - CP2102: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. Disconnect and reconnect USB
3. Check Device Manager (Windows) for new COM port

---

## ⚙️ Configuration Options

### Adjust Fall Detection Sensitivity

In the code, find line 70:
```cpp
const float FALL_THRESHOLD = 15.0;  // m/s²
```

**Adjust sensitivity:**
- **More sensitive** (detect smaller movements): `12.0` - `14.0`
- **Default**: `15.0`
- **Less sensitive** (only large falls): `18.0` - `20.0`

### Change WiFi Channel

If interference issues, change line 32:
```cpp
const uint8_t WIFI_CHANNEL = 1;  // Try 6 or 11
```

**Note:** Must match wearable channel!

### Adjust Statistics Update Rate

Find line 217:
```cpp
if (now - lastStatsMs >= 10000) {  // 10000 ms = 10 seconds
```

Change `10000` to:
- `5000` for 5 seconds
- `30000` for 30 seconds

---

## 📊 Understanding the Output

### Acceleration Values
- **X, Y, Z**: Direction of acceleration (m/s²)
- **Magnitude**: Total acceleration vector
- **Normal**: ~9.81 m/s² (gravity)
- **Fall**: >15 m/s² (sudden movement)

### Gyroscope Values
- **X, Y, Z**: Rotation rates (rad/s)
- **Positive/Negative**: Direction of rotation
- **Normal**: Near 0 rad/s (stationary)

### Temperature
- From MPU-6050 sensor
- Typical range: 20-30°C (room temperature)

### States
- **IDLE**: System idle (not used)
- **MONITORING**: Normal operation
- **FALL SUSPECTED**: Possible fall detected
- **FALL CONFIRMED**: Confirmed fall (future feature)

---

## 📞 Help & Support

### Quick Checks
1. Serial monitor at **115200 baud**
2. ESP32 shows "Hub Ready" message
3. Wearable shows successful sends
4. Both devices within 10 meters to start

### Debug Mode
Change line 4 in code to enable verbose logging:
```cpp
// Tools → Core Debug Level → Info
```

### Test Communication
1. Run both ESP32s
2. Watch serial monitors on both
3. Shake wearable vigorously
4. Hub should show fall detection

---

## ✅ Final Checklist

Before testing:
- [ ] Arduino IDE installed
- [ ] ESP32 board support installed
- [ ] Code uploaded successfully
- [ ] Serial monitor shows "Hub Ready"
- [ ] Wearable code uploaded and running
- [ ] Both ESP32s showing their MAC addresses
- [ ] MAC addresses correct in both codes

---

## 📝 File Information

**Filename**: `FallGuys_Hub_Receiver.ino`  
**Location**: `communication-hub/esp32/`  
**Board**: ESP32 (any variant)  
**Baud Rate**: 115200  
**Upload Speed**: 921600  

**Your MAC**: `EC:E3:34:DA:5D:B4`  
**Wearable MAC**: `EC:E3:34:DB:95:30`  

---

**Ready to go!** Just follow the steps above and you'll have the communication hub running in minutes! 🚀
