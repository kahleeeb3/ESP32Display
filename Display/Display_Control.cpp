#include "Display_Control.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1); // define the display

void initDisplay(){
  delay(1000); // give OLED driver sime time
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  // clear and update display
  display.clearDisplay();
  display.display();
}

void displayText(const char c[]){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(c);
  display.display();
}

void displayBitmap(uint8_t *bitmap){
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap, 128, 64, 1); // draw a bitmap
  display.display();
}