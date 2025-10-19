/*
 * ESP32 MAC Address Finder
 * Upload this sketch to get your ESP32's MAC address for ESP-NOW
 * 
 * Instructions:
 * 1. Rename main.cpp to main.cpp.bak
 * 2. Rename this file to main.cpp
 * 3. Upload and check Serial Monitor
 * 4. Copy the MAC address shown
 * 5. Restore original main.cpp
 */

#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("ESP32 MAC Address Finder");
  Serial.println("========================================\n");
  
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  delay(100);
  
  // Get MAC address
  String macAddress = WiFi.macAddress();
  uint8_t mac[6];
  WiFi.macAddress(mac);
  
  Serial.println("Your ESP32 MAC Address:");
  Serial.println("------------------------");
  Serial.print("String format: ");
  Serial.println(macAddress);
  Serial.print("Array format:  {0x");
  for(int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if(i < 5) Serial.print(",0x");
  }
  Serial.println("}");
  
  Serial.println("\n========================================");
  Serial.println("Copy the array format above for ESP-NOW");
  Serial.println("========================================\n");
}

void loop() {
  delay(5000);
  Serial.println("MAC Address: " + WiFi.macAddress());
}
