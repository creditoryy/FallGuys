// OLED Display Hardware Abstraction Layer - ESP32 Implementation
#include "hal/oled.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <stdarg.h>

// Display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Static display object
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
static bool is_initialized = false;

bool oled_init(void)
{
    Serial.println("OLED - Initializing");
    
    if (is_initialized) {
        Serial.println("OLED - Already initialized");
        return false;
    }

    // Initialize display
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("OLED - SSD1306 allocation failed");
        return false;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();

    is_initialized = true;
    Serial.println("OLED - Initialized successfully");
    return true;
}

void oled_clear(void)
{
    if (!is_initialized) return;
    display.clearDisplay();
}

void oled_display(void)
{
    if (!is_initialized) return;
    display.display();
}

void oled_set_cursor(uint8_t x, uint8_t y)
{
    if (!is_initialized) return;
    display.setCursor(x, y);
}

void oled_set_text_size(oled_text_size_t size)
{
    if (!is_initialized) return;
    display.setTextSize(size);
}

void oled_print(const char *str)
{
    if (!is_initialized || str == NULL) return;
    display.print(str);
}

void oled_println(const char *str)
{
    if (!is_initialized || str == NULL) return;
    display.println(str);
}

void oled_printf(const char *format, ...)
{
    if (!is_initialized || format == NULL) return;
    
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    display.print(buffer);
}

void oled_draw_hline(uint8_t x, uint8_t y, uint8_t width)
{
    if (!is_initialized) return;
    display.drawFastHLine(x, y, width, SSD1306_WHITE);
}

void oled_draw_vline(uint8_t x, uint8_t y, uint8_t height)
{
    if (!is_initialized) return;
    display.drawFastVLine(x, y, height, SSD1306_WHITE);
}

void oled_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill)
{
    if (!is_initialized) return;
    
    if (fill) {
        display.fillRect(x, y, width, height, SSD1306_WHITE);
    } else {
        display.drawRect(x, y, width, height, SSD1306_WHITE);
    }
}

void oled_cleanup(void)
{
    Serial.println("OLED - Cleanup");
    if (is_initialized) {
        display.clearDisplay();
        display.display();
    }
    is_initialized = false;
}
