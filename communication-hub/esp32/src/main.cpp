/*
 * FallGuys - Communication Hub ESP32 (ESP-NOW Receiver)
 * 
 * This code runs on the Communication Hub ESP32 #2
 * It receives sensor data from the Wearable Module via ESP-NOW
 * and sends back fall detection status
 * 
 * INSTRUCTIONS:
 * 1. Get this ESP32's MAC address using get_mac_address.cpp
 * 2. Put this MAC in the wearable's main_espnow.cpp (HUB_PEER_MAC)
 * 3. Put the wearable's MAC in WEARABLE_PEER_MAC below
 * 4. Upload this code to Communication Hub ESP32
 * 
 * NOTE: This is a placeholder for MS2. Currently just echoes data back.
 * Future: This will interface with BeagleBoard for fall detection algorithm.
 */

#include <Arduino.h>
#include <WiFi.h>
extern "C" {
  #include <esp_now.h>
  #include <esp_wifi.h>
  #include <esp_wifi_types.h>
}

// ===== Configuration =====
// Wearable Module's MAC address (your ESP32)
uint8_t WEARABLE_PEER_MAC[6] = {0xEC, 0xE3, 0x34, 0xDB, 0x95, 0x30};  // Your ESP32

const uint8_t WIFI_CHANNEL = 1;  // Must match wearable

// ===== Data Structures =====

// Sensor data received FROM wearable
typedef struct __attribute__((packed)) {
  float accel_x;
  float accel_y;
  float accel_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
  float temperature;
  uint32_t timestamp;
} sensor_data_t;

// Fall status sent TO wearable
typedef struct __attribute__((packed)) {
  uint8_t state;          // 0=IDLE, 1=MONITORING, 2=FALL_SUSPECTED, 3=FALL_CONFIRMED
  uint8_t fall_severity;  // 0-255
  float fall_confidence;  // 0.0-1.0
  uint32_t timestamp;
  uint8_t reserved[7];
} fall_status_t;

static_assert(sizeof(sensor_data_t) == 32, "sensor_data_t must be 32 bytes");
static_assert(sizeof(fall_status_t) == 16, "fall_status_t must be 16 bytes");

// ===== State Variables =====
unsigned long lastReceiveMs = 0;
unsigned long receiveCount = 0;
unsigned long sendCount = 0;
sensor_data_t latestSensorData{};

// Simple fall detection state (placeholder for MS2)
uint8_t currentState = 1;  // 1 = MONITORING
float fallMagnitude = 0.0f;

// ===== Fall Detection Algorithm (Placeholder) =====
// This is a SIMPLE placeholder. Real algorithm will be on BeagleBoard.

void simpleFallDetection(const sensor_data_t &data) {
  // Calculate acceleration magnitude
  float accelMag = sqrt(
    data.accel_x * data.accel_x +
    data.accel_y * data.accel_y +
    data.accel_z * data.accel_z
  );
  
  fallMagnitude = accelMag;
  
  // Simple threshold detection (placeholder)
  const float FALL_THRESHOLD = 15.0;  // m/s² (~1.5g)
  const float NORMAL_GRAVITY = 9.81;   // m/s²
  
  if (accelMag > FALL_THRESHOLD) {
    // Sudden acceleration detected
    if (currentState == 1) {  // Was monitoring
      currentState = 2;  // FALL_SUSPECTED
      Serial.println("[FALL] Suspected fall detected!");
    }
  } else if (accelMag < (NORMAL_GRAVITY + 2.0) && currentState == 2) {
    // Acceleration returned to normal
    currentState = 1;  // Back to MONITORING
    Serial.println("[FALL] False alarm, back to monitoring");
  }
  
  // Auto-reset after 5 seconds
  static unsigned long lastStateChange = 0;
  if (currentState == 2 && (millis() - lastStateChange > 5000)) {
    currentState = 1;
    lastStateChange = millis();
  }
}

// ===== ESP-NOW Callbacks =====

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("[TX] Fall status sent successfully");
  } else {
    Serial.println("[TX] Send failed!");
  }
}

void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len >= (int)sizeof(sensor_data_t)) {
    memcpy(&latestSensorData, data, sizeof(sensor_data_t));
    lastReceiveMs = millis();
    receiveCount++;
    
    Serial.printf("[RX #%lu] Sensor data from %02X:%02X:%02X:%02X:%02X:%02X\n",
      receiveCount,
      info->src_addr[0], info->src_addr[1], info->src_addr[2],
      info->src_addr[3], info->src_addr[4], info->src_addr[5]);
    
    Serial.printf("     Accel: %.2f, %.2f, %.2f m/s²\n",
      latestSensorData.accel_x,
      latestSensorData.accel_y,
      latestSensorData.accel_z);
    
    Serial.printf("     Gyro:  %.2f, %.2f, %.2f rad/s\n",
      latestSensorData.gyro_x,
      latestSensorData.gyro_y,
      latestSensorData.gyro_z);
    
    Serial.printf("     Temp:  %.1f °C\n", latestSensorData.temperature);
    
    // Run fall detection algorithm
    simpleFallDetection(latestSensorData);
    
    // Send fall status back to wearable
    fall_status_t status;
    status.state = currentState;
    status.fall_severity = (uint8_t)(constrain(fallMagnitude / 20.0 * 255, 0, 255));
    status.fall_confidence = constrain(fallMagnitude / 20.0, 0.0, 1.0);
    status.timestamp = millis();
    memset(status.reserved, 0, sizeof(status.reserved));
    
    esp_err_t result = esp_now_send(WEARABLE_PEER_MAC, (const uint8_t*)&status, sizeof(status));
    if (result == ESP_OK) {
      sendCount++;
    }
  }
}

// ===== ESP-NOW Initialization =====

void initESPNow() {
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true);
  
  // Set channel
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ESP-NOW] Initialization failed!");
    while (1) delay(1000);
  }
  
  // Register callbacks
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);
  
  // Add peer (Wearable Module)
  esp_now_peer_info_t peerInfo{};
  memcpy(peerInfo.peer_addr, WEARABLE_PEER_MAC, 6);
  peerInfo.channel = WIFI_CHANNEL;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ESP-NOW] Failed to add peer!");
    while (1) delay(1000);
  }
  
  // Print MAC address
  Serial.print("[ESP-NOW] Hub MAC: ");
  Serial.println(WiFi.macAddress());
  
  uint8_t ch;
  wifi_second_chan_t sc;
  esp_wifi_get_channel(&ch, &sc);
  Serial.printf("[ESP-NOW] Channel: %u\n", ch);
}

// ===== Setup =====

void setup() {
  Serial.begin(115200);
  delay(200);
  
  Serial.println("\n\n========================================");
  Serial.println("FallGuys - Communication Hub (ESP32)");
  Serial.println("========================================\n");
  
  // Initialize ESP-NOW
  initESPNow();
  
  Serial.println("\n=== Hub Ready - Waiting for sensor data ===\n");
}

// ===== Main Loop =====

void loop() {
  unsigned long now = millis();
  
  // Print statistics every 5 seconds
  static unsigned long lastStatsMs = 0;
  if (now - lastStatsMs >= 5000) {
    Serial.println("\n--- Statistics ---");
    Serial.printf("Received: %lu packets\n", receiveCount);
    Serial.printf("Sent:     %lu packets\n", sendCount);
    Serial.printf("State:    %s\n", 
      currentState == 0 ? "IDLE" :
      currentState == 1 ? "MONITORING" :
      currentState == 2 ? "FALL_SUSPECTED" :
      currentState == 3 ? "FALL_CONFIRMED" : "UNKNOWN");
    Serial.printf("Fall Mag: %.2f m/s²\n", fallMagnitude);
    
    if (now - lastReceiveMs > 2000) {
      Serial.println("WARNING: No data received for 2+ seconds");
    }
    
    Serial.println();
    lastStatsMs = now;
  }
  
  delay(100);
}
