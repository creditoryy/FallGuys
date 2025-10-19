#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// FallGuys Communication Protocol v1.0
// =============================================================================

// Protocol constants
#define PROTOCOL_START_BYTE     0xAA
#define PROTOCOL_END_BYTE       0x55
#define PROTOCOL_MAX_PAYLOAD    255

// Packet types - Wearable to Hub
#define PKT_SENSOR_DATA         0x01    // Periodic sensor data
#define PKT_FALL_DETECTED       0x02    // Fall detection alert
#define PKT_HEARTRATE           0x03    // Heart rate & vitals
#define PKT_STATUS_RESPONSE     0x13    // Response to status request
#define PKT_USER_RESPONSE       0x20    // User acknowledgment

// Packet types - Hub to Wearable
#define PKT_ACK                 0x10    // Acknowledgment
#define PKT_CONFIG              0x11    // Configuration update
#define PKT_STATUS_REQUEST      0x12    // Status request

// Configuration IDs (for PKT_CONFIG)
#define CFG_SAMPLING_RATE       0x01    // uint32_t (Hz)
#define CFG_FALL_THRESHOLD      0x02    // float (g)
#define CFG_ALERT_TIMEOUT       0x03    // uint32_t (seconds)
#define CFG_DISPLAY_BRIGHTNESS  0x04    // uint8_t (0-255)

// System states
#define STATE_IDLE              0x00
#define STATE_MONITORING        0x01
#define STATE_FALL_SUSPECTED    0x02
#define STATE_FALL_CONFIRMED    0x03
#define STATE_EMERGENCY_ALERT   0x04
#define STATE_ERROR             0xFF

// User response types
#define USER_NO_RESPONSE        0x00
#define USER_CONFIRMED_OK       0x01
#define USER_REQUESTED_HELP     0x02

// ACK types
#define ACK_SENSOR_DATA         0x01
#define ACK_FALL_DETECTED       0x02
#define ACK_HEARTRATE           0x03

// Heart rate status
#define HR_STATUS_OK            0x00
#define HR_STATUS_LOW           0x01
#define HR_STATUS_HIGH          0x02
#define HR_STATUS_IRREGULAR     0x03
#define HR_STATUS_ERROR         0x04

// =============================================================================
// Data Structures
// =============================================================================

// Packet structure (raw format)
typedef struct {
    uint8_t start;                          // 0xAA
    uint8_t length;                         // Payload length
    uint8_t type;                           // Packet type
    uint8_t payload[PROTOCOL_MAX_PAYLOAD];  // Data
    uint16_t crc;                           // CRC-16
    uint8_t end;                            // 0x55
} protocol_packet_t;

// SENSOR_DATA payload (32 bytes)
typedef struct {
    float accel_x;      // m/s²
    float accel_y;      // m/s²
    float accel_z;      // m/s²
    float gyro_x;       // rad/s
    float gyro_y;       // rad/s
    float gyro_z;       // rad/s
    float temperature;  // °C
    uint32_t timestamp; // milliseconds
} sensor_data_t;

// FALL_DETECTED payload (28 bytes)
typedef struct {
    uint8_t severity;       // 0-255 (0=low, 255=critical)
    float impact;           // Impact force (g)
    uint32_t duration;      // Fall duration (ms)
    float pre_impact_x;     // Acceleration at impact
    float pre_impact_y;
    float pre_impact_z;
    uint32_t timestamp;     // milliseconds
} fall_detected_t;

// HEARTRATE payload (16 bytes)
typedef struct {
    uint16_t bpm;           // Beats per minute (40-200)
    uint8_t spo2;           // Oxygen saturation % (70-100)
    uint8_t status;         // HR_STATUS_xxx
    uint8_t reserved[8];    // Reserved for future use
    uint32_t timestamp;     // milliseconds
} heartrate_t;

// ACK payload (8 bytes)
typedef struct {
    uint8_t ack_type;       // ACK_xxx
    uint8_t seq_num;        // Sequence number
    uint8_t reserved[6];    // Reserved
} ack_t;

// CONFIG payload (variable)
typedef struct {
    uint8_t config_id;      // CFG_xxx
    uint8_t length;         // Value length
    uint8_t value[253];     // Configuration value
} config_t;

// STATUS_RESPONSE payload (16 bytes)
typedef struct {
    uint8_t state;          // STATE_xxx
    uint8_t battery;        // Battery % (0-100)
    uint32_t uptime;        // Seconds since boot
    uint8_t errors;         // Error count
    uint8_t reserved[9];    // Reserved
} status_response_t;

// USER_RESPONSE payload (5 bytes)
typedef struct {
    uint8_t response;       // USER_xxx
    uint32_t timestamp;     // milliseconds
} user_response_t;

// =============================================================================
// Function Prototypes
// =============================================================================

/**
 * Calculate CRC-16/CCITT checksum
 * @param data: Data buffer
 * @param length: Data length
 * @return CRC-16 value
 */
uint16_t protocol_calculate_crc(const uint8_t* data, size_t length);

/**
 * Validate packet integrity
 * @param packet: Packet to validate
 * @return true if valid, false otherwise
 */
bool protocol_validate_packet(const protocol_packet_t* packet);

/**
 * Encode packet into buffer
 * @param buffer: Output buffer (must be at least length + 6 bytes)
 * @param type: Packet type
 * @param payload: Payload data
 * @param length: Payload length
 * @return Total packet size, or -1 on error
 */
int protocol_encode_packet(uint8_t* buffer, uint8_t type, 
                           const uint8_t* payload, uint8_t length);

/**
 * Decode packet from buffer
 * @param packet: Output packet structure
 * @param buffer: Input buffer
 * @param length: Buffer length
 * @return Number of bytes consumed, or -1 on error
 */
int protocol_decode_packet(protocol_packet_t* packet, 
                           const uint8_t* buffer, size_t length);

/**
 * Create SENSOR_DATA packet
 * @param buffer: Output buffer
 * @param data: Sensor data
 * @return Packet size, or -1 on error
 */
int protocol_create_sensor_data(uint8_t* buffer, const sensor_data_t* data);

/**
 * Create FALL_DETECTED packet
 * @param buffer: Output buffer
 * @param fall: Fall detection data
 * @return Packet size, or -1 on error
 */
int protocol_create_fall_detected(uint8_t* buffer, const fall_detected_t* fall);

/**
 * Create HEARTRATE packet
 * @param buffer: Output buffer
 * @param hr: Heart rate data
 * @return Packet size, or -1 on error
 */
int protocol_create_heartrate(uint8_t* buffer, const heartrate_t* hr);

/**
 * Create ACK packet
 * @param buffer: Output buffer
 * @param ack_type: Type of acknowledgment
 * @param seq_num: Sequence number
 * @return Packet size, or -1 on error
 */
int protocol_create_ack(uint8_t* buffer, uint8_t ack_type, uint8_t seq_num);

/**
 * Create STATUS_REQUEST packet
 * @param buffer: Output buffer
 * @return Packet size, or -1 on error
 */
int protocol_create_status_request(uint8_t* buffer);

/**
 * Create STATUS_RESPONSE packet
 * @param buffer: Output buffer
 * @param status: Status data
 * @return Packet size, or -1 on error
 */
int protocol_create_status_response(uint8_t* buffer, const status_response_t* status);

/**
 * Create USER_RESPONSE packet
 * @param buffer: Output buffer
 * @param response: User response
 * @return Packet size, or -1 on error
 */
int protocol_create_user_response(uint8_t* buffer, const user_response_t* response);

/**
 * Parse SENSOR_DATA packet
 * @param data: Output sensor data
 * @param packet: Input packet
 * @return true on success, false on error
 */
bool protocol_parse_sensor_data(sensor_data_t* data, const protocol_packet_t* packet);

/**
 * Parse FALL_DETECTED packet
 * @param fall: Output fall data
 * @param packet: Input packet
 * @return true on success, false on error
 */
bool protocol_parse_fall_detected(fall_detected_t* fall, const protocol_packet_t* packet);

/**
 * Parse HEARTRATE packet
 * @param hr: Output heart rate data
 * @param packet: Input packet
 * @return true on success, false on error
 */
bool protocol_parse_heartrate(heartrate_t* hr, const protocol_packet_t* packet);

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_H
