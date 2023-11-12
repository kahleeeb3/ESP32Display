#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Display_Control.h"
#include "ServerInfo.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WebServer server(80);

String form = "<h1>Hello from ESP32!</h1>"
              "<form action='/submit' method='post'>"
              "Message: <input type='text' name='message'><br>"
              "<input type='submit' value='Submit'>"
              "</form>";

String html = "<html><body>" + form + "<p id='submission'></p></body></html>";

void handleRoot() {
  server.send(200, "text/html", html);
}

void handleFormSubmit() {
  String message = server.arg("message");

  // Update the HTML content to include the user's submission
  String updatedHtml = html;
  updatedHtml.replace(form, form + "<p>Submission: " + message + "</p>");
  server.send(200, "text/html", updatedHtml);

  // Display the message on the external display
  displayText(message.c_str());
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void initServer() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  displayText(WiFi.localIP().toString().c_str());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleFormSubmit);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup(void) {
  Serial.begin(9600);
  initDisplay();
  initServer();
}

void loop(void) {
  server.handleClient();
  delay(2);
}
