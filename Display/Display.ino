#include "Display_Control.h"
#include "splash.h"
#include "Server.h"

void setup() {
  Serial.begin(9600);
  initDisplay();
  initServer();
}

void loop() {
  // displayText("Test of function");
  // displayBitmap(caleb_bmp);

  getData();

  delay(1000);
}