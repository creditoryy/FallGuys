/*
 * FallGuys - Wearable Sensor Module with ESP-NOW
 * 
 * Hardware:
 * - ESP32-WROOM-32D
 * - MPU-6050 (I2C: SDA=21, SCL=22, Address=0x68)
 * - SSD1306 OLED (I2C: Address=0x3C, requires 5V power)
 * 
 * Communication:
 * - ESP-NOW to send sensor data to Communication Hub
 * - Receives processed fall detection status back
 * 
 * INSTRUCTIONS:
 * 1. First run get_mac_address.cpp to get your MAC
 * 2. Update HUB_PEER_MAC below with Communication Hub's MAC
 * 3. Upload this code
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
extern "C" {
  #include <esp_now.h>
  #include <esp_wifi.h>
  #include <esp_wifi_types.h>
}

// Version compatibility for ESP32 Arduino Core
// Detect ESP-IDF version for callback compatibility
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
  #define ESP_NOW_RECV_CB_NEW  // New callback signature (v3.x+)
#endif

// ===== OLED Configuration =====
#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_ADDR   0x3C
#define OLED_SDA    21
#define OLED_SCL    22

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// ===== MPU-6050 Configuration =====
Adafruit_MPU6050 mpu;

// ===== ESP-NOW Configuration =====
// Communication Hub's MAC address (your friend's ESP32)
uint8_t HUB_PEER_MAC[6] = {0xEC, 0xE3, 0x34, 0xDA, 0x5D, 0xB4};  // Friend's ESP32

const uint8_t WIFI_CHANNEL = 1;  // Must match hub

// ===== Data Structures (matching protocol.h) =====

// RAW sensor data sent TO hub
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

// Processed data received FROM hub
typedef struct __attribute__((packed)) {
  uint8_t state;          // System state (0=IDLE, 1=MONITORING, 2=FALL_SUSPECTED, 3=FALL_CONFIRMED)
  uint8_t fall_severity;  // 0-255
  float fall_confidence;  // 0.0-1.0
  uint32_t timestamp;     // milliseconds
  uint8_t reserved[6];    // Padding to 16 bytes (1+1+4+4+6=16)
} fall_status_t;

static_assert(sizeof(sensor_data_t) == 32, "sensor_data_t must be 32 bytes");
static_assert(sizeof(fall_status_t) == 16, "fall_status_t must be 16 bytes");

// ===== State Variables =====
volatile bool haveReply = false;
volatile fall_status_t lastFallStatus{};
unsigned long lastReplyMs = 0;
unsigned long lastSendMs = 0;
unsigned long sensorReadCount = 0;
unsigned long sendErrorCount = 0;

// State names for display
const char* STATE_NAMES[] = {
  "IDLE",
  "MONITORING", 
  "FALL SUSPECTED",
  "FALL CONFIRMED",
  "EMERGENCY"
};

// ===== ESP-NOW Callbacks =====

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("[TX] Sensor data sent successfully");
  } else {
    Serial.println("[TX] Send failed!");
    sendErrorCount++;
  }
}

// Version-compatible receive callback
#ifdef ESP_NOW_RECV_CB_NEW
void onDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
#else
void onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  // Create a dummy info structure for compatibility
  struct { const uint8_t *src_addr; } info_compat;
  info_compat.src_addr = mac;
  const auto *info = &info_compat;
#endif
  if (len >= (int)sizeof(fall_status_t)) {
    memcpy((void*)&lastFallStatus, data, sizeof(fall_status_t));
    haveReply = true;
    lastReplyMs = millis();
    
    uint8_t state_val = lastFallStatus.state;  // Copy volatile to local
    Serial.printf("[RX] Fall Status from %02X:%02X:%02X:%02X:%02X:%02X\n",
      info->src_addr[0], info->src_addr[1], info->src_addr[2],
      info->src_addr[3], info->src_addr[4], info->src_addr[5]);
    Serial.printf("     State=%s, Severity=%d, Confidence=%.2f\n",
      STATE_NAMES[min(state_val, (uint8_t)4)],
      lastFallStatus.fall_severity,
      lastFallStatus.fall_confidence);
  }
}

// ===== ESP-NOW Initialization =====

void initESPNow() {
  // CRITICAL: Start WiFi in STA mode FIRST
  WiFi.mode(WIFI_STA);
  WiFi.begin();  // Start WiFi subsystem
  delay(100);
  WiFi.disconnect(false);
  
  Serial.println("\n=== ESP-NOW Initialization ===");
  Serial.print("Wearable MAC: ");
  Serial.println(WiFi.macAddress());
  
  // NOW set the channel (after WiFi is started)
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  
  // Verify channel
  uint8_t ch;
  wifi_second_chan_t sc;
  esp_wifi_get_channel(&ch, &sc);
  Serial.printf("[INFO] Channel before ESP-NOW init: %u\n", ch);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] ESP-NOW initialization failed!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("ESP-NOW INIT FAIL");
    display.display();
    while (1) delay(1000);
  }
  Serial.println("[OK] ESP-NOW initialized");
  
  // Verify channel again
  esp_wifi_get_channel(&ch, &sc);
  Serial.printf("[OK] Final WiFi Channel: %u\n", ch);
  
  if (ch != WIFI_CHANNEL) {
    Serial.printf("[WARNING] Channel mismatch! Expected %u, got %u\n", WIFI_CHANNEL, ch);
    Serial.println("[WARNING] Communication may fail!");
  }
  
  // Register callbacks
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("[OK] Callbacks registered");
  
  // Add peer (Communication Hub)
  esp_now_peer_info_t peerInfo{};
  memcpy(peerInfo.peer_addr, HUB_PEER_MAC, 6);
  peerInfo.channel = WIFI_CHANNEL;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ERROR] Failed to add peer!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("PEER ADD FAIL");
    display.display();
    while (1) delay(1000);
  }
  
  Serial.print("[OK] Peer added: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", HUB_PEER_MAC[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  Serial.println("=== Ready to Send ===\n");
}

// ===== OLED Helper Functions =====

void drawProgressBar(int x, int y, int w, int h, int percent) {
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  int fillWidth = (w - 4) * constrain(percent, 0, 100) / 100;
  display.fillRect(x + 2, y + 2, fillWidth, h - 4, SSD1306_WHITE);
}

// ===== Setup =====

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n\n=== FallGuys Wearable Sensor Module ===");
  
  // Initialize I2C
  Wire.begin(OLED_SDA, OLED_SCL);
  
  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("[OLED] Initialization failed!");
    while (1) delay(1000);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("FallGuys Wearable");
  display.println("Initializing...");
  display.display();
  delay(500);
  
  // Initialize MPU-6050
  if (!mpu.begin()) {
    Serial.println("[MPU6050] Initialization failed!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("MPU6050 FAIL");
    display.println("Check wiring:");
    display.println("SDA -> GPIO21");
    display.println("SCL -> GPIO22");
    display.display();
    while (1) delay(1000);
  }
  
  // Configure MPU-6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  Serial.println("[MPU6050] Initialized successfully");
  
  // Initialize ESP-NOW
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Initializing");
  display.println("ESP-NOW...");
  display.display();
  
  initESPNow();
  
  Serial.println("[ESP-NOW] Initialized successfully");
  
  // Ready screen
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("System Ready!");
  display.println();
  display.print("MAC: ");
  display.println(WiFi.macAddress());
  display.display();
  delay(2000);
  
  Serial.println("\n=== System Ready ===\n");
}

// ===== Main Loop =====

void loop() {
  unsigned long now = millis();
  
  // Read sensors
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);
  
  sensorReadCount++;
  
  // Prepare sensor data packet
  sensor_data_t packet;
  packet.accel_x = accel.acceleration.x;
  packet.accel_y = accel.acceleration.y;
  packet.accel_z = accel.acceleration.z;
  packet.gyro_x = gyro.gyro.x;
  packet.gyro_y = gyro.gyro.y;
  packet.gyro_z = gyro.gyro.z;
  packet.temperature = temp.temperature;
  packet.timestamp = now;
  
  // Send data via ESP-NOW (every 100ms = 10Hz)
  if (now - lastSendMs >= 100) {
    esp_err_t result = esp_now_send(HUB_PEER_MAC, (const uint8_t*)&packet, sizeof(packet));
    
    if (result != ESP_OK) {
      Serial.printf("[TX] Send error: %d ", (int)result);
      if (result == 12396) {
        Serial.println("(ESP_ERR_ESPNOW_NOT_FOUND - Peer not found!)");
      } else if (result == 12389) {
        Serial.println("(ESP_ERR_ESPNOW_NOT_INIT - ESP-NOW not initialized!)");
      } else if (result == 12394) {
        Serial.println("(ESP_ERR_ESPNOW_ARG - Invalid argument!)");
      } else {
        Serial.println("(Unknown error)");
      }
      sendErrorCount++;
    }
    
    lastSendMs = now;
  }
  
  // Update OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  // Header
  display.println("=== WEARABLE ===");
  
  // Sensor readings
  display.setTextSize(1);
  display.printf("Accel: %.1f %.1f %.1f\n", packet.accel_x, packet.accel_y, packet.accel_z);
  display.printf("Gyro:  %.1f %.1f %.1f\n", packet.gyro_x, packet.gyro_y, packet.gyro_z);
  display.printf("Temp:  %.1f C\n", packet.temperature);
  
  display.println();
  
  // Fall detection status
  if (haveReply) {
    unsigned long replyAge = now - lastReplyMs;
    
    if (replyAge < 5000) {  // Show if reply is fresh (< 5 seconds old)
      uint8_t state_val = lastFallStatus.state;  // Copy volatile to local
      display.print("Status: ");
      display.println(STATE_NAMES[min(state_val, (uint8_t)4)]);
      
      if (lastFallStatus.state >= 2) {  // FALL_SUSPECTED or higher
        display.printf("Severity: %d%%\n", (lastFallStatus.fall_severity * 100) / 255);
        drawProgressBar(0, 52, 128, 8, (lastFallStatus.fall_severity * 100) / 255);
      }
    } else {
      display.println("Status: No reply");
    }
  } else {
    display.println("Status: Waiting...");
  }
  
  // Statistics at bottom
  display.setCursor(0, 56);
  display.printf("Sent:%lu Err:%lu", sensorReadCount, sendErrorCount);
  
  display.display();
  
  // Small delay for ~10Hz loop
  delay(50);
}
