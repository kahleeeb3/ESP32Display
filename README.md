# ESP32 Display Simple Display Server

## Equipment
- [Adafruit ESP32 Feather V2 - 8MB Flash + 2 MB PSRAM - STEMMA QT](https://www.adafruit.com/product/5400)
- [Monochrome 1.3" 128x64 OLED graphic display - STEMMA QT / Qwiic](https://www.adafruit.com/product/938)
- [Lithium Ion Polymer Battery - 3.7v 1200mAh](https://www.adafruit.com/product/258)

## Resources
- [SSD1306-OLED-Emulator](https://github.com/sam-peach/SSD1306-OLED-Emulator)
- [Adafruit_CAD_Parts](https://github.com/adafruit/Adafruit_CAD_Parts)

## Design
This will show the design process for the 3D shell. The shell will be created In SolidWorks. The thought is that it will have a Desktop PC like structure.
<div align="center">
    <img src="Images/Design Part 1.png" alt="drawing" width="30%"/>
    <div> Figure 1: Starting Design Concept </div>
</div>

## Server Info
Create the following `Server.py` file
```python
ESP32_IP = "YOUR LOCAL IP HERE"     # Replace with the ESP32's Private IP address
# ESP32_IP = "YOUR PUBLIC IP HERE"  # Replace with the ESP32's Public IP address
ESP32_PORT = 000                    # Replace with the port you would like to use
```
Create the following `ServerInfo.h` file
```cpp
#ifndef SERVERINFO
#define SERVERINFO

#define WIFI_SSID      "YOUR SSID HERE"     // Replace with your network SSID
#define WIFI_PASSWORD  "YOUR PASSWORD HERE" // Replace with your network password
#define TCP_PORT        000                 // replace with the port you intend to use

#endif
```