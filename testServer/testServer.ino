#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Redmi Note 12";
const char* password = "wb845q4fmpura7x";

WebServer server(80); // Port 80

void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP32!");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
