#ifndef WIFISERVER
#define WIFISERVER

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "ServerInfo.h"
#include "Display_Control.h"

void initServer();
void getData();

#endif