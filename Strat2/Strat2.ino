#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Display_Control.h"
#include "ServerInfo.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WebServer server(80);

String form = "<h1>Image Resizer</h1>"
              "<form action='/submit' method='post'>"
              "Message: <input type='text' name='message'><br>"
              "<input type='submit' value='Submit'>"
              "</form>"
              "<form id='upload-form'>"
              "Select Image: <input type='file' id='image-input' accept='image/*'><br>"
              "<input type='button' value='Submit' onclick='resizeAndDisplay()'>"
              "</form>"
              "<canvas id='resized-canvas' width='128' height='64' style='display:none;'></canvas>"
              "<img id='resized-image' style='display:none;'>"
              "<script>"
              "function resizeAndDisplay() {"
              "  var input = document.getElementById('image-input');"
              "  var canvas = document.getElementById('resized-canvas');"
              "  var ctx = canvas.getContext('2d');"
              "  var img = new Image();"
              "  img.onload = function() {"
              "    ctx.drawImage(img, 0, 0, 128, 64);"
              "    var resizedImage = document.getElementById('resized-image');"
              "    resizedImage.src = canvas.toDataURL('image/jpeg', 0.8);"
              "    resizedImage.style.display = 'block';"
              "  };"
              "  var file = input.files[0];"
              "  var reader = new FileReader();"
              "  reader.onload = function(e) {"
              "    img.src = e.target.result;"
              "  };"
              "  reader.readAsDataURL(file);"
              "}"
              "</script>";

String html = "<html><body>" + form + "</body></html>";

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
  Serial.println(message);
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

  char connectedString[100]; // array to store network data
  sprintf(connectedString, "Connected!\n%s\n%s:%d\n", 
    WiFi.localIP().toString().c_str(), getPublicIP().c_str(),TCP_PORT
  );
  displayText(connectedString);

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
