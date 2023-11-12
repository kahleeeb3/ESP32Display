#include "Server.h"

WiFiServer server(TCP_PORT);  // define server

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

void initServer(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("\nConnecting to WiFi...");
    displayText("Connecting to WiFi...");
  }
  
  char connectedString[100]; // array to store network data
  sprintf(connectedString, "Connected!\n%s\n%s\n%s\n%d", 
    WiFi.SSID(), WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str(), TCP_PORT
  );
  displayText(connectedString);
  
  Serial.println(connectedString);
  
  server.begin();
}

void getData(){
  WiFiClient client = server.available();
  if (client) {
    // Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String data = client.readStringUntil('\r');
        // Serial.print("Received data: ");
        Serial.println(data);
        displayText(data.c_str());
      }
    }
    client.stop();
    // Serial.println("Client disconnected");
  }
}