# Communication Hub

**Platform**: ESP32 + BeagleBoard  
**Development OS**: Linux  
**Build System**: Makefile + CMake

## 📋 Overview

The communication hub is the processing and networking component of the FallGuys fall detection system. It receives sensor data from the wearable module, processes it on the BeagleBoard, and coordinates emergency responses.

## 🏗️ Architecture

The hub consists of two interconnected components:

### 1. ESP32 Communication Controller
- Receives data from wearable sensor module
- Interfaces with BeagleBoard via UART/SPI
- Manages real-time communication
- Handles protocol encoding/decoding

### 2. BeagleBoard Main Processor
- Processes sensor data and runs detection algorithms
- Manages GPS module for location tracking
- Handles network connectivity (WiFi/Cellular)
- Coordinates emergency service/caretaker contact
- Logs data for analysis

## 📁 Directory Structure

```
communication-hub/
├── README.md                  # This file
│
├── esp32/                     # ESP32 firmware
│   ├── platformio.ini        # PlatformIO configuration
│   ├── Makefile              # Linux build automation
│   ├── src/
│   │   ├── main.cpp          # ESP32 main application
│   │   ├── protocol.cpp      # Protocol handler
│   │   └── uart_comm.cpp     # UART/SPI communication
│   └── include/
│       ├── protocol.h
│       └── uart_comm.h
│
└── beagleboard/              # BeagleBoard software
    ├── Makefile
    ├── src/
    │   ├── main.c            # Main application
    │   ├── data_processor.c  # Sensor data processing
    │   ├── fall_detector.c   # Fall detection algorithm
    │   ├── gps_handler.c     # GPS location services
    │   ├── network_manager.c # Network connectivity
    │   └── emergency.c       # Emergency contact system
    ├── include/
    │   ├── data_processor.h
    │   ├── fall_detector.h
    │   ├── gps_handler.h
    │   ├── network_manager.h
    │   └── emergency.h
    └── config/
        └── config.json       # Configuration file
```

## 🔧 Hardware Components

### ESP32 Module
| Component | Interface | Description |
|-----------|-----------|-------------|
| ESP32 | - | Communication controller |
| UART/SPI | GPIO | Interface to wearable module |
| UART | GPIO | Interface to BeagleBoard |
| Status LEDs | GPIO | System status indicators |

### BeagleBoard Setup
| Component | Interface | Description |
|-----------|-----------|-------------|
| BeagleBoard Black/AI | - | Main processor |
| GPS Module | UART | Location tracking |
| WiFi/4G Module | USB/PCIe | Network connectivity |
| Storage | SD Card | Data logging |

## 🚀 Quick Start

### Prerequisites (Linux)
```bash
# Install development tools
sudo apt-get update
sudo apt-get install build-essential cmake git

# Install PlatformIO (for ESP32)
pip install platformio

# Install BeagleBoard SDK
# (Instructions vary by BeagleBoard model)
```

### Building ESP32 Firmware

```bash
cd esp32
make build
```

### Uploading ESP32 Firmware

```bash
make upload
make monitor
```

### Building BeagleBoard Software

```bash
cd beagleboard
make
```

### Running on BeagleBoard

```bash
sudo ./build/fallguys-hub
```

## 📡 Communication Protocol

The hub uses a custom protocol to communicate with the wearable module:

### Protocol Specification
- **Transport**: UART (115200 baud) or SPI
- **Format**: Binary packet-based
- **Error Detection**: CRC16
- **Packet Structure**:
  ```
  [START] [LENGTH] [TYPE] [PAYLOAD] [CRC] [END]
  ```

See `../../protocol/README.md` for detailed protocol specification.

### Packet Types

| Type ID | Name | Direction | Description |
|---------|------|-----------|-------------|
| 0x01 | SENSOR_DATA | Wearable → Hub | Sensor readings |
| 0x02 | FALL_DETECTED | Wearable → Hub | Fall detection alert |
| 0x03 | HEARTRATE | Wearable → Hub | Heart rate data |
| 0x10 | ACK | Hub → Wearable | Acknowledgment |
| 0x11 | CONFIG | Hub → Wearable | Configuration update |
| 0x12 | STATUS_REQUEST | Hub → Wearable | Request status |

## 🧠 Fall Detection Algorithm

The BeagleBoard processes sensor data using a multi-stage algorithm:

### Stage 1: Motion Analysis
- Monitors acceleration magnitude
- Detects sudden changes (threshold: 2.5g)
- Analyzes motion pattern

### Stage 2: Orientation Check
- Evaluates gyroscope data
- Confirms fall trajectory
- Distinguishes from normal movement

### Stage 3: Impact Detection
- Detects impact with ground
- Measures impact force

### Stage 4: Vital Verification
- Checks heart rate stability
- Monitors consciousness indicators
- Filters false alarms

### Stage 5: Post-Fall Assessment
- Monitors user position after fall
- Checks for movement/recovery
- Waits for user response

## 🚨 Emergency Response System

When a fall is confirmed:

1. **User Alert** - Send signal to wearable to display "Are you OK?" prompt
2. **Countdown** - Wait 30 seconds for user response
3. **Vital Check** - Analyze heart rate and consciousness
4. **Decision**:
   - If user confirms OK → Cancel alert
   - If vitals stable but no response → Contact caretaker
   - If vitals unstable or critical → Call emergency services
5. **Location** - Send GPS coordinates
6. **Continuous Monitoring** - Keep monitoring until help arrives

### Contact Priority
1. User confirmation (highest priority)
2. Caretaker notification (medium priority)
3. Emergency services (critical situations only)

## 🗺️ GPS Location Services

### GPS Module
- Module: NEO-6M or similar
- Interface: UART
- Update rate: 1 Hz
- Accuracy: ~2.5m

### Location Data
```json
{
  "latitude": 49.2827,
  "longitude": -123.1207,
  "altitude": 100,
  "accuracy": 2.5,
  "timestamp": "2025-10-18T20:00:00Z"
}
```

## 🌐 Network Connectivity

### WiFi
- Emergency contact via WiFi when available
- Lower latency, preferred method
- SMS/email notifications

### Cellular (Fallback)
- 4G/LTE module for remote locations
- SMS emergency alerts
- HTTP/MQTT for cloud logging

### Cloud Services (Future)
- Data logging and analysis
- Remote monitoring dashboard
- Historical data for pattern analysis

## ⚙️ Configuration

Edit `beagleboard/config/config.json`:

```json
{
  "emergency": {
    "phone": "+1-234-567-8900",
    "email": "emergency@example.com",
    "countdown": 30
  },
  "caretaker": {
    "name": "John Doe",
    "phone": "+1-234-567-8901",
    "email": "caretaker@example.com"
  },
  "thresholds": {
    "fall_acceleration": 2.5,
    "impact_force": 3.0,
    "heartrate_min": 40,
    "heartrate_max": 150
  },
  "gps": {
    "enabled": true,
    "uart_port": "/dev/ttyS1",
    "baud_rate": 9600
  }
}
```

## 🧪 Testing

### Unit Tests
```bash
cd beagleboard
make test
```

### Integration Tests
```bash
cd ../../testing/integration-tests
./run_tests.sh
```

### Simulated Fall Tests
```bash
cd ../../testing/fall-detection-tests
./simulate_fall.py
```

## 📊 Development Status

### MS1 Checklist
- [ ] ESP32 communication firmware
- [ ] UART/SPI protocol implementation
- [ ] BeagleBoard base software
- [ ] GPS module integration
- [ ] Initial communication with wearable

### MS2 Checklist
- [ ] Fall detection algorithm implementation
- [ ] Emergency contact system
- [ ] Network connectivity (WiFi/Cellular)
- [ ] Full system integration
- [ ] End-to-end testing

## 🔍 Troubleshooting

### ESP32 Not Receiving Data
1. Check UART connections
2. Verify baud rate matches wearable module
3. Test with loopback (TX→RX)

### BeagleBoard Communication Issues
1. Check UART device: `ls /dev/ttyS*`
2. Verify permissions: `sudo usermod -a -G dialout $USER`
3. Test with minicom: `minicom -D /dev/ttyS1`

### GPS Not Getting Fix
1. Check antenna connection
2. Ensure clear view of sky
3. Wait 30-60 seconds for cold start
4. Verify UART communication

## 📖 Additional Documentation

- **Protocol Specification**: See `../../protocol/README.md`
- **System Architecture**: See `../../docs/design/`
- **GPS Setup Guide**: See `../../docs/design/gps_setup.md`
- **Network Configuration**: See `../../docs/design/network_config.md`

## 🤝 Contributing

Development workflow:
1. Develop on Linux system or BeagleBoard
2. Use Makefile for building
3. Test with wearable simulator first
4. Integration test with actual wearable module
5. Document changes in this README

---

**Last Updated**: October 2025  
**Status**: Planning Phase
