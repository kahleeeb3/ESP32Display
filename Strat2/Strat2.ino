#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Display_Control.h"
#include "ServerInfo.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WebServer server(80);

String html = "<head>\n"
              "    <title>ESP32 Display Messenger</title>\n"
              "</head>\n"
              "<body>\n"
              "    <h1>Message Submission</h1>\n"
              "    <form action='/submit' method='post'>\n"
              "        Message: <input type='text' name='message'>\n"
              "        <input type='submit' value='Send'>\n"
              "    </form>\n"
              "    <h1>Image Resizer</h1>\n"
              "    <canvas id='resized-canvas' width='128' height='64' style='display:none;'></canvas>\n"
              "    <img id='resized-image' style='display:none;'>\n"
              "    <form id='upload-form'>\n"
              "        Select Image: <input type='file' id='image-input' accept='image/*'>\n"
              "        <input type='button' value='Update Image' onclick='resizeAndDisplay()'>\n"
              "        <input type='button' value='Send Image' onclick='sendImage()'>\n"
              "        <br>\n"
              "        Threshold: <input type='range' id='threshold-slider' min='0' max='255' value='115' oninput='updateThresholdValue()'>\n"
              "        <span id='threshold-value'>115</span>\n"
              "    </form>\n"
              "    <script>\n"
              "        function resizeAndDisplay() {\n"
              "            var input = document.getElementById('image-input');\n"
              "            var canvas = document.getElementById('resized-canvas');\n"
              "            var ctx = canvas.getContext('2d');\n"
              "            var img = new Image();\n"
              "            var thresholdSlider = document.getElementById('threshold-slider');\n"
              "            var thresholdValueSpan = document.getElementById('threshold-value');\n"
              "            img.onload = function() {\n"
              "                // Resize the image\n"
              "                ctx.drawImage(img, 0, 0, 128, 64);\n"
              "                // Convert the image to greyscale\n"
              "                var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);\n"
              "                var data = imageData.data;\n"
              "                for (var i = 0; i < data.length; i += 4) {\n"
              "                    var avg = (data[i] + data[i + 1] + data[i + 2]) / 3;\n"
              "                    data[i] = avg;\n"
              "                    data[i + 1] = avg;\n"
              "                    data[i + 2] = avg;\n"
              "                }\n"
              "                ctx.putImageData(imageData, 0, 0);\n"
              "                // Apply threshold\n"
              "                imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);\n"
              "                data = imageData.data;\n"
              "                var threshold = thresholdSlider.value;\n"
              "                thresholdValueSpan.innerText = threshold;\n"
              "                for (var i = 0; i < data.length; i += 4) {\n"
              "                    var brightness = (data[i] + data[i + 1] + data[i + 2]) / 3;\n"
              "                    var newValue = brightness > threshold ? 255 : 0;\n"
              "                    data[i] = newValue;\n"
              "                    data[i + 1] = newValue;\n"
              "                    data[i + 2] = newValue;\n"
              "                }\n"
              "                ctx.putImageData(imageData, 0, 0);\n"
              "                // Convert to monochrome\n"
              "                var thresholdedImage = document.getElementById('resized-image');\n"
              "                thresholdedImage.src = canvas.toDataURL('image/png');\n"
              "                thresholdedImage.style.display = 'block';\n"
              "            };\n"
              "            var file = input.files[0];\n"
              "            var reader = new FileReader();\n"
              "            reader.onload = function(e) {\n"
              "                img.src = e.target.result;\n"
              "            };\n"
              "            reader.readAsDataURL(file);\n"
              "        }\n"
              "        function updateThresholdValue() {\n"
              "            var thresholdSlider = document.getElementById('threshold-slider');\n"
              "            var thresholdValueSpan = document.getElementById('threshold-value');\n"
              "            thresholdValueSpan.innerText = thresholdSlider.value;\n"
              "        }\n"
              "        function sendImage() {\n"
              "            var canvas = document.getElementById('resized-canvas');\n"
              "            var ctx = canvas.getContext('2d');\n"
              "            var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height).data;\n"
              "            // Convert image data to a bitmap string\n"
              "            var bitmapString = '';\n"
              "            for (var i = 0; i < imageData.length; i += 4) {\n"
              "                var brightness = (imageData[i] + imageData[i + 1] + imageData[i + 2]) / 3;\n"
              "                bitmapString += brightness > 128 ? '1' : '0';\n"
              "            }\n"
              "            // Break into 8-bit chunks\n"
              "            var binaryChunks = [];\n"
              "            for (var i = 0; i < bitmapString.length; i += 8) {\n"
              "                binaryChunks.push(bitmapString.slice(i, i + 8));\n"
              "            }\n"
              "            // Convert chunks to strings\n"
              "            var binaryStrings = binaryChunks.map(chunk => chunk);\n"
              "            // Convert each section to a byte\n"
              "            var byteValues = binaryStrings.map(binaryString => parseInt(binaryString, 2));\n"
              "            // Log the byte data (you may want to send it to the server)\n"
              "            console.log('Byte Data:', byteValues);\n"
              "            // Store the byte data globally if needed\n"
              "            // window.byteData = byteValues;\n"
              "        }\n"
              "    </script>\n"
              "</body>\n";

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
  updatedHtml.replace(html, html + "<p>Submission: " + message + "</p>");
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
