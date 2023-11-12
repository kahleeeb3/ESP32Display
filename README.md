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

## New Idea
The old idea was to have python handle everything but I think it would be more user friendly to create a
HTML server and have user communicate over that.

1. Try to connect to internet with provided SSID, Password
    - If user presses button before network connects, create your own web-server
    - `Scan` button allows them to search available network and select one
    - `Password` can be provided by the user
    - Attempt to connect again.
    - If success kill own network

2. Once connection success, allow user to submit some text
    - send text over socket
