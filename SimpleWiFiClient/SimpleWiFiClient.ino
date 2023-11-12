#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "ServerInfo.h"

WiFiServer server(TCP_PORT);  // start the server

String getPublicIP() {
  WiFiClient client;
  if (client.connect("api64.ipify.org", 80)) {
    client.println("GET /?format=text HTTP/1.0");
    client.println("Host: api64.ipify.org");
    client.println();
    while (client.connected() && !client.find("\r\n\r\n")) {
      // Wait for the response headers
    }
    // Read and parse the response
    String response = client.readStringUntil('\r');
    return response;
  }
  return "None";
}


void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected!");
  Serial.println("======================");
  Serial.print("Network: ");
  Serial.println(WiFi.SSID());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Private IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Public IP Address: ");
  Serial.println(getPublicIP());
  Serial.print("Port: ");
  Serial.println(TCP_PORT);
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    // Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String data = client.readStringUntil('\r');
        Serial.print("Received data: ");
        Serial.println(data);
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
