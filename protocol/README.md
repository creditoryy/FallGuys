# Communication Protocol

**Version**: 1.0 (Draft)  
**Last Updated**: October 2025

## 📋 Overview

This document defines the communication protocol between the **Wearable Sensor Module** (ESP32 #1) and the **Communication Hub** (ESP32 #2 + BeagleBoard).

## 🔌 Physical Layer

### Option 1: UART (Recommended)
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1
- **Flow Control**: None
- **Wiring**:
  - Wearable TX → Hub RX
  - Wearable RX → Hub TX
  - GND → GND

### Option 2: SPI (High-Speed Alternative)
- **Clock Speed**: 1 MHz
- **Mode**: Mode 0 (CPOL=0, CPHA=0)
- **Bit Order**: MSB First
- **Wiring**:
  - MOSI, MISO, CLK, CS, GND

## 📦 Packet Format

All communication uses binary packets with the following structure:

```
┌────────┬────────┬────────┬─────────────┬────────┬────────┐
│ START  │ LENGTH │  TYPE  │   PAYLOAD   │  CRC16 │  END   │
├────────┼────────┼────────┼─────────────┼────────┼────────┤
│ 0xAA   │ 1 byte │ 1 byte │  0-255 bytes│ 2 bytes│ 0x55   │
└────────┴────────┴────────┴─────────────┴────────┴────────┘
```

### Field Descriptions

| Field | Size | Description |
|-------|------|-------------|
| START | 1 byte | Fixed: 0xAA (start marker) |
| LENGTH | 1 byte | Payload length (0-255) |
| TYPE | 1 byte | Packet type identifier |
| PAYLOAD | 0-255 bytes | Data payload (format depends on TYPE) |
| CRC16 | 2 bytes | CRC-16/CCITT checksum (LENGTH + TYPE + PAYLOAD) |
| END | 1 byte | Fixed: 0x55 (end marker) |

### CRC-16 Calculation

```c
uint16_t calculate_crc16(const uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc = crc << 1;
        }
    }
    return crc;
}
```

## 📨 Packet Types

### 0x01 - SENSOR_DATA (Wearable → Hub)

Periodic sensor data transmission.

**Payload Format** (32 bytes):
```
┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬──────────┬───────────┐
│ Accel X │ Accel Y │ Accel Z │ Gyro X  │ Gyro Y  │ Gyro Z  │   Temp   │ Timestamp │
├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼──────────┼───────────┤
│ 4 bytes │ 4 bytes │ 4 bytes │ 4 bytes │ 4 bytes │ 4 bytes │  4 bytes │  4 bytes  │
│ float   │ float   │ float   │ float   │ float   │ float   │  float   │  uint32_t │
└─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴──────────┴───────────┘
```

**Units**:
- Acceleration: m/s² (meters per second squared)
- Gyroscope: rad/s (radians per second)
- Temperature: °C (Celsius)
- Timestamp: milliseconds since boot

**Example**:
```c
typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float temperature;
    uint32_t timestamp;
} sensor_data_t;
```

**Transmission Rate**: 10 Hz (every 100ms)

---

### 0x02 - FALL_DETECTED (Wearable → Hub)

Immediate alert when fall is detected by wearable module.

**Payload Format** (20 bytes):
```
┌───────────┬──────────┬──────────┬─────────────┬───────────┐
│ Severity  │ Impact   │ Duration │ Pre-Impact  │ Timestamp │
├───────────┼──────────┼──────────┼─────────────┼───────────┤
│  1 byte   │ 4 bytes  │ 4 bytes  │   8 bytes   │  4 bytes  │
│  uint8_t  │  float   │ uint32_t │ accel data  │ uint32_t  │
└───────────┴──────────┴──────────┴─────────────┴───────────┘
```

**Fields**:
- **Severity**: 0-255 (0=low, 255=critical)
- **Impact**: Impact force in g (gravitational units)
- **Duration**: Fall duration in milliseconds
- **Pre-Impact**: Acceleration vector at impact (3x float: x, y, z)
- **Timestamp**: milliseconds since boot

**Priority**: HIGH - Requires immediate acknowledgment

---

### 0x03 - HEARTRATE (Wearable → Hub)

Heart rate and vital signs data.

**Payload Format** (16 bytes):
```
┌───────────┬────────┬──────────┬────────────┬───────────┐
│    BPM    │  SpO2  │  Status  │   Flags    │ Timestamp │
├───────────┼────────┼──────────┼────────────┼───────────┤
│  2 bytes  │ 1 byte │  1 byte  │  8 bytes   │  4 bytes  │
│  uint16_t │ uint8_t│  uint8_t │  reserved  │ uint32_t  │
└───────────┴────────┴──────────┴────────────┴───────────┘
```

**Fields**:
- **BPM**: Beats per minute (40-200)
- **SpO2**: Oxygen saturation percentage (70-100%)
- **Status**: 0=OK, 1=Low, 2=High, 3=Irregular, 4=Error
- **Flags**: Reserved for future use
- **Timestamp**: milliseconds since boot

**Transmission Rate**: 1 Hz (every 1 second)

---

### 0x10 - ACK (Hub → Wearable)

Acknowledgment packet.

**Payload Format** (8 bytes):
```
┌───────────┬───────────┬────────────┐
│  Ack Type │  Seq Num  │  Reserved  │
├───────────┼───────────┼────────────┤
│  1 byte   │  1 byte   │  6 bytes   │
│  uint8_t  │  uint8_t  │     -      │
└───────────┴───────────┴────────────┘
```

**Ack Types**:
- 0x01: SENSOR_DATA received
- 0x02: FALL_DETECTED received and processing
- 0x03: HEARTRATE received

---

### 0x11 - CONFIG (Hub → Wearable)

Configuration update from hub to wearable.

**Payload Format** (Variable):
```
┌───────────┬───────────┬─────────────┐
│ Config ID │   Length  │    Value    │
├───────────┼───────────┼─────────────┤
│  1 byte   │  1 byte   │  N bytes    │
│  uint8_t  │  uint8_t  │     -       │
└───────────┴───────────┴─────────────┘
```

**Config IDs**:
- 0x01: Sampling rate (4 bytes, uint32_t, Hz)
- 0x02: Fall threshold (4 bytes, float, g)
- 0x03: Alert timeout (4 bytes, uint32_t, seconds)
- 0x04: Display brightness (1 byte, uint8_t, 0-255)

---

### 0x12 - STATUS_REQUEST (Hub → Wearable)

Request current status from wearable.

**Payload Format**: Empty (0 bytes)

**Expected Response**: STATUS_RESPONSE (0x13)

---

### 0x13 - STATUS_RESPONSE (Wearable → Hub)

Response to status request.

**Payload Format** (16 bytes):
```
┌───────────┬──────────┬────────────┬──────────┬───────────┐
│   State   │ Battery  │  Uptime    │  Errors  │  Flags    │
├───────────┼──────────┼────────────┼──────────┼───────────┤
│  1 byte   │  1 byte  │  4 bytes   │  1 byte  │  9 bytes  │
│  uint8_t  │  uint8_t │  uint32_t  │  uint8_t │  reserved │
└───────────┴──────────┴────────────┴──────────┴───────────┘
```

**States**:
- 0x00: IDLE
- 0x01: MONITORING
- 0x02: FALL_SUSPECTED
- 0x03: FALL_CONFIRMED
- 0x04: EMERGENCY_ALERT
- 0xFF: ERROR

**Battery**: Percentage (0-100%)
**Uptime**: Seconds since boot
**Errors**: Error count since boot

---

### 0x20 - USER_RESPONSE (Wearable → Hub)

User acknowledgment from OLED display.

**Payload Format** (4 bytes):
```
┌───────────┬───────────┐
│ Response  │ Timestamp │
├───────────┼───────────┤
│  1 byte   │  4 bytes  │
│  uint8_t  │  uint32_t │
└───────────┴───────────┘
```

**Response Types**:
- 0x00: No response (timeout)
- 0x01: User confirmed OK
- 0x02: User requested help

---

## 🔄 Communication Flow

### Normal Operation

```
Wearable                    Hub
   |                         |
   |--SENSOR_DATA (0x01)---> |
   | <------ACK (0x10)------ |
   |                         |
   |--HEARTRATE (0x03)-----> |
   | <------ACK (0x10)------ |
   |                         |
```

### Fall Detection Sequence

```
Wearable                    Hub                   BeagleBoard
   |                         |                         |
   | FALL DETECTED!          |                         |
   |--FALL_DETECTED (0x02)-->|                         |
   | <------ACK (0x10)------ |                         |
   |                         |--Process & Analyze----->|
   | Display "Are you OK?"   |                         |
   |                         |                         |
   | User presses button     |                         |
   |--USER_RESPONSE (0x20)-->|                         |
   | <------ACK (0x10)------ |                         |
   |                         |--Decision Made--------->|
   |                         | <--Emergency/Cancel---- |
   |                         |                         |
```

## 📡 Protocol State Machine

### Wearable States

```
                      ┌──────┐
                 ┌────│ IDLE │────┐
                 │    └──────┘    │
                 │                 │
            Power On          Sensor Init
                 │                 │
                 ▼                 ▼
            ┌──────────┐     ┌───────────┐
            │   INIT   │────>│MONITORING │
            └──────────┘     └───────────┘
                                    │
                              Fall Detected
                                    │
                                    ▼
                            ┌──────────────┐
                            │FALL_SUSPECTED│
                            └──────────────┘
                                    │
                          Confirmed or Timeout
                                    │
                                    ▼
                            ┌──────────────┐
                            │FALL_CONFIRMED│
                            └──────────────┘
                                    │
                              User Response
                                    │
                   ┌────────────────┴────────────────┐
                   ▼                                  ▼
            ┌──────────┐                      ┌──────────────┐
            │ RESOLVED │                      │EMERGENCY_ALERT│
            └──────────┘                      └──────────────┘
                   │                                  │
                   └──────────────┬───────────────────┘
                                  ▼
                            ┌──────────┐
                            │MONITORING│
                            └──────────┘
```

## 🛠️ Implementation Reference

### C Header File

See `protocol.h` for complete implementation:

```c
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

// Protocol constants
#define PROTOCOL_START_BYTE     0xAA
#define PROTOCOL_END_BYTE       0x55
#define PROTOCOL_MAX_PAYLOAD    255

// Packet types
#define PKT_SENSOR_DATA         0x01
#define PKT_FALL_DETECTED       0x02
#define PKT_HEARTRATE           0x03
#define PKT_ACK                 0x10
#define PKT_CONFIG              0x11
#define PKT_STATUS_REQUEST      0x12
#define PKT_STATUS_RESPONSE     0x13
#define PKT_USER_RESPONSE       0x20

// Packet structure
typedef struct {
    uint8_t start;
    uint8_t length;
    uint8_t type;
    uint8_t payload[PROTOCOL_MAX_PAYLOAD];
    uint16_t crc;
    uint8_t end;
} protocol_packet_t;

// Function prototypes
uint16_t protocol_calculate_crc(const uint8_t* data, size_t length);
bool protocol_validate_packet(const protocol_packet_t* packet);
int protocol_encode_packet(uint8_t* buffer, uint8_t type, 
                           const uint8_t* payload, uint8_t length);
int protocol_decode_packet(protocol_packet_t* packet, 
                           const uint8_t* buffer, size_t length);

#endif // PROTOCOL_H
```

## 🧪 Testing

### Loopback Test
Connect TX to RX on same device and verify packet transmission/reception.

### CRC Validation
Test with intentionally corrupted packets to verify error detection.

### Timing Test
Measure latency from wearable transmission to hub acknowledgment.

**Target Latency**: < 50ms for FALL_DETECTED packets

## 📊 Performance Requirements

| Metric | Requirement | Notes |
|--------|-------------|-------|
| Latency | < 50ms | For fall detection alerts |
| Packet Loss | < 0.1% | Over 24-hour operation |
| Throughput | > 1000 packets/sec | Worst case: 10 Hz × 3 packet types |
| CRC Error Rate | 0% | Must detect all corrupted packets |

## 🔮 Future Enhancements

- **Encryption**: AES-128 for sensitive data
- **Compression**: For high-frequency sensor data
- **Multi-device**: Support for multiple wearables
- **Wireless**: Bluetooth Low Energy option

---

**Version History**:
- v1.0 (2025-10-18): Initial draft specification

**Status**: Draft - Subject to change during MS1 development
