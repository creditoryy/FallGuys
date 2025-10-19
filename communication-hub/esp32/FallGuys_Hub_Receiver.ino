/*
 * FallGuys - Communication Hub ESP32 (ESP-NOW Receiver)
 * FOR ARDUINO IDE
 * 
 * This code receives sensor data from the Wearable Module via ESP-NOW
 * and sends back fall detection status
 * 
 * HARDWARE:
 * - ESP32 (any variant)
 * - USB cable for programming
 * 
 * ARDUINO IDE SETUP:
 * 1. Install ESP32 board support:
 *    File → Preferences → Additional Board Manager URLs:
 *    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * 
 * 2. Tools → Board → ESP32 Arduino → Select your board (e.g., "ESP32 Dev Module")
 * 
 * 3. Tools → Upload Speed → 921600
 * 
 * 4. Tools → Port → Select your COM port
 * 
 * INSTALLATION:
 * 1. Save this file as: FallGuys_Hub_Receiver.ino
 * 2. Open in Arduino IDE
 * 3. Upload to your ESP32
 * 4. Open Serial Monitor (115200 baud)
 * 
 * YOUR ESP32 MAC ADDRESS: EC:E3:34:DA:5D:B4
 * WEARABLE MAC ADDRESS:   EC:E3:34:DB:95:30
 */

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// Version compatibility for ESP32 Arduino Core
// Detect ESP-IDF version for callback compatibility
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
  #define ESP_NOW_RECV_CB_NEW  // New callback signature (v3.x+)
#endif

// ===== Configuration =====
// Wearable Module's MAC address (already configured)
uint8_t WEARABLE_PEER_MAC[6] = {0xEC, 0xE3, 0x34, 0xDB, 0x95, 0x30};

const uint8_t WIFI_CHANNEL = 1;  // Must match wearable

// ===== Data Structures =====

// Sensor data received FROM wearable (32 bytes)
typedef struct __attribute__((packed)) {
  float accel_x;      // m/s²
  float accel_y;
  float accel_z;
  float gyro_x;       // rad/s
  float gyro_y;
  float gyro_z;
  float temperature;  // °C
  uint32_t timestamp; // milliseconds
} sensor_data_t;

// Fall status sent TO wearable (16 bytes)
typedef struct __attribute__((packed)) {
  uint8_t state;          // 0=IDLE, 1=MONITORING, 2=FALL_SUSPECTED, 3=FALL_CONFIRMED
  uint8_t fall_severity;  // 0-255
  float fall_confidence;  // 0.0-1.0
  uint32_t timestamp;     // milliseconds
  uint8_t reserved[7];    // Reserved for future use
} fall_status_t;

// ===== State Variables =====
unsigned long lastReceiveMs = 0;
unsigned long receiveCount = 0;
unsigned long sendCount = 0;
sensor_data_t latestSensorData = {};

// Simple fall detection state (placeholder for MS2)
uint8_t currentState = 1;  // 1 = MONITORING
float fallMagnitude = 0.0f;
unsigned long lastStateChange = 0;

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
  const float FALL_THRESHOLD = 15.0;  // m/s² (~1.5g sudden acceleration)
  const float NORMAL_GRAVITY = 9.81;   // m/s²
  
  if (accelMag > FALL_THRESHOLD) {
    // Sudden acceleration detected
    if (currentState == 1) {  // Was monitoring
      currentState = 2;  // FALL_SUSPECTED
      lastStateChange = millis();
      Serial.println("\n╔════════════════════════════════════╗");
      Serial.println("║  ⚠️  FALL SUSPECTED!  ⚠️           ║");
      Serial.println("╚════════════════════════════════════╝");
      Serial.printf("Acceleration magnitude: %.2f m/s²\n\n", accelMag);
    }
  } else if (accelMag < (NORMAL_GRAVITY + 2.0) && currentState == 2) {
    // Acceleration returned to normal
    currentState = 1;  // Back to MONITORING
    Serial.println("[FALL] False alarm - back to monitoring\n");
  }
  
  // Auto-reset after 5 seconds
  if (currentState == 2 && (millis() - lastStateChange > 5000)) {
    currentState = 1;
    Serial.println("[FALL] Timeout - back to monitoring\n");
  }
}

// ===== ESP-NOW Callbacks =====

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("[TX] ✓ Fall status sent successfully");
  } else {
    Serial.println("[TX] ✗ Send failed!");
  }
}

// Callback when data is received
// This version is compatible with ESP32 Arduino Core v3.x (ESP-IDF 5.x+)
#ifdef ESP_NOW_RECV_CB_NEW
void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  // Extract MAC address from recv_info structure
  const uint8_t *mac = recv_info->src_addr;
#else
// For older ESP32 Arduino Core v2.x
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
#endif
  if (len >= (int)sizeof(sensor_data_t)) {
    memcpy(&latestSensorData, incomingData, sizeof(sensor_data_t));
    lastReceiveMs = millis();
    receiveCount++;
    
    // Print header with separator
    Serial.println("\n─────────────────────────────────────");
    Serial.printf("[RX #%lu] Sensor Data Received\n", receiveCount);
    Serial.println("─────────────────────────────────────");
    
    // Print MAC address
    Serial.print("From: ");
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", mac[i]);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
    
    // Print sensor data
    Serial.printf("Accel:  X=%.2f  Y=%.2f  Z=%.2f m/s²\n",
      latestSensorData.accel_x,
      latestSensorData.accel_y,
      latestSensorData.accel_z);
    
    Serial.printf("Gyro:   X=%.2f  Y=%.2f  Z=%.2f rad/s\n",
      latestSensorData.gyro_x,
      latestSensorData.gyro_y,
      latestSensorData.gyro_z);
    
    Serial.printf("Temp:   %.1f °C\n", latestSensorData.temperature);
    
    // Calculate and display total acceleration
    float totalAccel = sqrt(
      latestSensorData.accel_x * latestSensorData.accel_x +
      latestSensorData.accel_y * latestSensorData.accel_y +
      latestSensorData.accel_z * latestSensorData.accel_z
    );
    Serial.printf("Magnitude: %.2f m/s²\n", totalAccel);
    
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
    } else {
      Serial.printf("[TX] Error sending: %d\n", result);
    }
  }
}

// ===== ESP-NOW Initialization =====

void initESPNow() {
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // Set channel
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW initialization failed!");
    while (1) delay(1000);
  }
  
  Serial.println("✓ ESP-NOW initialized");
  
  // Register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Add peer (Wearable Module)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, WEARABLE_PEER_MAC, 6);
  peerInfo.channel = WIFI_CHANNEL;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer!");
    while (1) delay(1000);
  }
  
  Serial.println("✓ Peer added successfully");
}

// ===== Setup =====

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(1000);
  
  // Print banner
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════════════╗");
  Serial.println("║                                                ║");
  Serial.println("║       FallGuys Communication Hub (ESP32)       ║");
  Serial.println("║           ESP-NOW Receiver Module              ║");
  Serial.println("║                                                ║");
  Serial.println("╚════════════════════════════════════════════════╝");
  Serial.println();
  
  // Initialize ESP-NOW
  Serial.println("Initializing ESP-NOW...");
  initESPNow();
  
  // Print configuration
  Serial.println("\n─────────────────────────────────────");
  Serial.println("Configuration:");
  Serial.println("─────────────────────────────────────");
  Serial.print("Hub MAC Address:      ");
  Serial.println(WiFi.macAddress());
  Serial.print("Wearable MAC Address: EC:E3:34:DB:95:30");
  Serial.println();
  
  uint8_t ch;
  wifi_second_chan_t sc;
  esp_wifi_get_channel(&ch, &sc);
  Serial.printf("WiFi Channel:         %u\n", ch);
  Serial.println("─────────────────────────────────────");
  
  Serial.println("\n✓ Hub Ready - Waiting for sensor data...\n");
}

// ===== Main Loop =====

void loop() {
  unsigned long now = millis();
  
  // Print statistics every 10 seconds
  static unsigned long lastStatsMs = 0;
  if (now - lastStatsMs >= 10000) {
    Serial.println("\n╔════════════════════════════════════╗");
    Serial.println("║         STATISTICS REPORT          ║");
    Serial.println("╚════════════════════════════════════╝");
    Serial.printf("Packets Received: %lu\n", receiveCount);
    Serial.printf("Packets Sent:     %lu\n", sendCount);
    Serial.printf("Current State:    %s\n", 
      currentState == 0 ? "IDLE" :
      currentState == 1 ? "MONITORING" :
      currentState == 2 ? "⚠️  FALL SUSPECTED" :
      currentState == 3 ? "🚨 FALL CONFIRMED" : "UNKNOWN");
    Serial.printf("Fall Magnitude:   %.2f m/s²\n", fallMagnitude);
    Serial.printf("Uptime:           %lu seconds\n", now / 1000);
    
    // Warning if no data received
    if (receiveCount == 0) {
      Serial.println("\n⚠️  WARNING: No data received yet!");
      Serial.println("   Check wearable module is running");
    } else if (now - lastReceiveMs > 5000) {
      Serial.printf("\n⚠️  WARNING: No data for %lu seconds\n", (now - lastReceiveMs) / 1000);
      Serial.println("   Wearable may be disconnected");
    }
    
    Serial.println("════════════════════════════════════\n");
    lastStatsMs = now;
  }
  
  delay(100);
}
