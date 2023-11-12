#ifndef DISPLAY_CONTROL
#define DISPLAY_CONTROL

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void initDisplay();
void displayText(const char c[]);
void displayBitmap(uint8_t *bitmap);

#endif