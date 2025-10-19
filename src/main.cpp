#include <Arduino.h>

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Initialize the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println("ESP32 FallGuys Project Started!");
}

void loop() {
  // Blink the LED
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
  delay(1000);
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
