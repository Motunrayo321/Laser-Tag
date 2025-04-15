#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

// const char* ssid     = "iPhone";
// const char* password = "3902608018";

// const char* ssid     = "UNILAG-DESIGN-STUDIO";
// const char* password = "Design@24.";

const char* ssid = "Redmi Note 12";
const char* password = "wb845q4fmpura7x";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Game variables
String playerName = "Not Registered";
String playerTeam = "Red";
int playerScore = 0;
int playersLeft = 10;
int timeLeft = 300;  
int playerHealth = 100;
String leadingTeam = "Red";

// HTML Webpage stored as a string
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>IR Shooting Game</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        body { font-family: Arial, sans-serif; text-align: center; background-color: #222; color: white; }
        .container { width: 80%; margin: auto; padding: 20px; }
        .input-box { padding: 10px; margin: 10px; font-size: 16px; }
        .button { padding: 10px 20px; font-size: 16px; background-color: #28a745; color: white; border: none; cursor: pointer; }
        .button:hover { background-color: #218838; }
        .stats { margin-top: 20px; text-align: left; display: inline-block; }
        .bar { width: 100px; height: 20px; background-color: red; }
    </style>
    
  </head>

  <body>
    <div class="container">
        <h1>IR Shooting Game</h1>
        <div>
            <input type="text" id="name" class="input-box" placeholder="Enter Your Name">
            <button class="button" onclick="registerPlayer()">Join Game</button>
        </div>

        <div class="stats">
            <p>Player: <span id="playerName">Not Registered</span></p>
            <p>Team: <span id="team">N/A</span></p>
            <p>Score: <span id="score">0</span></p>
            <p>Health: <span id="health">100</span></p>
            <div class="bar" id="healthBar"></div>
        </div>
        <div class="stats">
            <p>Players Left: <span id="playersLeft">10</span></p>
            <p>Time Left: <span id="timeLeft">300</span> sec</p>
            <p>Leading Team: <span id="leadingTeam">N/A</span></p>
        </div>
    </div>

    <script>
        let ws;

        function connectWebSocket() {
            ws = new WebSocket("ws://" + window.location.host + "/ws");
            ws.onmessage = function(event) {
                let data = JSON.parse(event.data);
                document.getElementById("score").innerText = data.score;
                document.getElementById("healthBar").style.width = data.health + "px";
                document.getElementById("team").innerText = data.team;
                document.getElementById("playersLeft").innerText = data.playersLeft;
                document.getElementById("timeLeft").innerText = data.timeLeft;
                document.getElementById("leadingTeam").innerText = data.leadingTeam;
            };
        }

        function registerPlayer() {
            let name = document.getElementById("name").value;
            fetch("/register?name=" + name).then(response => response.text()).then(data => {
                document.getElementById("playerName").innerText = name;
                ws.send(JSON.stringify({ name: name }));
            });
        }

        window.onload = connectWebSocket;
    </script>
    
  </body>
</html>
)rawliteral";

// Function to send game updates to all connected WebSocket clients
void sendGameUpdate() {
  StaticJsonDocument<256> doc;
  doc["score"] = playerScore;
  doc["health"] = playerHealth;
  doc["team"] = playerTeam;
  doc["playersLeft"] = playersLeft;
  doc["timeLeft"] = timeLeft;
  doc["leadingTeam"] = leadingTeam;
  String jsonString;
  serializeJson(doc, jsonString);
  ws.textAll(jsonString);
}

// WebSocket Event Handler
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, data);
    if (doc.containsKey("name")) {
        playerName = doc["name"].as<String>();
    }
    sendGameUpdate();
  }
}

void setup() {
  Serial.begin(115200);

  // Set up WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("ESP32 WebSocket Server Started");

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", webpage);
  });

  server.on("/register", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("name")) {
      playerName = request->getParam("name")->value();
    }
    request->send(200, "text/plain", "Player registered: " + playerName);
    Serial.println("Player Registered!\n");
    Serial.print("Player Name: ");
    Serial.println(playerName);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();

  // Simulated game logic (Replace with actual IR sensor logic)
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) { // Update every 5 seconds
    lastUpdate = millis();

    // Simulate getting hit and losing health
    playerHealth -= 10;
    if (playerHealth <= 0) {
      playersLeft--;
      playerHealth = 100;  // Respawn logic (optional)
    }

    // Simulate scoring
    playerScore += 5;

    // Simulate leading team change
    leadingTeam = (playerScore % 2 == 0) ? "Red" : "Blue";

    // Send update to all clients
    sendGameUpdate();
  }
}
