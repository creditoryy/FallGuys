// OLED Display Hardware Abstraction Layer
// Provides interface for displaying text and graphics on OLED screen

#ifndef _OLED_H_
#define _OLED_H_

#include <stdbool.h>
#include <stdint.h>

// Display dimensions
#define OLED_WIDTH  128
#define OLED_HEIGHT 64

// Text sizes
typedef enum {
    OLED_TEXT_SIZE_SMALL = 1,   // 6x8 pixels per character
    OLED_TEXT_SIZE_MEDIUM = 2,  // 12x16 pixels per character
    OLED_TEXT_SIZE_LARGE = 3    // 18x24 pixels per character
} oled_text_size_t;

/**
 * Initialize the OLED display
 * @return true if successful, false otherwise
 */
bool oled_init(void);

/**
 * Clear the entire display
 */
void oled_clear(void);

/**
 * Update the display with buffered content
 * Must be called after drawing operations to see changes
 */
void oled_display(void);

/**
 * Set cursor position for text
 * @param x X coordinate (0 to OLED_WIDTH-1)
 * @param y Y coordinate (0 to OLED_HEIGHT-1)
 */
void oled_set_cursor(uint8_t x, uint8_t y);

/**
 * Set text size
 * @param size Text size (1, 2, or 3)
 */
void oled_set_text_size(oled_text_size_t size);

/**
 * Print a string at current cursor position
 * @param str String to print
 */
void oled_print(const char *str);

/**
 * Print a string with newline at current cursor position
 * @param str String to print
 */
void oled_println(const char *str);

/**
 * Print a formatted string (like printf)
 * @param format Format string
 * @param ... Variable arguments
 */
void oled_printf(const char *format, ...);

/**
 * Draw a horizontal line
 * @param x Starting X coordinate
 * @param y Y coordinate
 * @param width Length of line
 */
void oled_draw_hline(uint8_t x, uint8_t y, uint8_t width);

/**
 * Draw a vertical line
 * @param x X coordinate
 * @param y Starting Y coordinate
 * @param height Height of line
 */
void oled_draw_vline(uint8_t x, uint8_t y, uint8_t height);

/**
 * Draw a rectangle
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param width Width of rectangle
 * @param height Height of rectangle
 * @param fill true to fill, false for outline only
 */
void oled_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill);

/**
 * Cleanup and release OLED resources
 */
void oled_cleanup(void);

#endif // _OLED_H_
