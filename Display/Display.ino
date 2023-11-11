#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "splash.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void initDisplay(){
  delay(1000); // KEEP THIS!
  // The OLED driver circuit needs a small amount of time to be ready after initial power.
  // https://learn.adafruit.com/monochrome-oled-breakouts/troubleshooting-2

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
}

void setup() {
  Serial.begin(9600);
  initDisplay();
}

void loop() {
  display.clearDisplay();

  // Display Text
  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // display.setCursor(0, 0);
  // display.println("Hello World!");

  // Display bitmap
  display.drawBitmap(0, 0, caleb_bmp, 128, 64, 1); // draw a bitmap

  Serial.printf("ARRAY SIZE = %d\n", sizeof(caleb_bmp));

  display.display();
  delay(1000);
}