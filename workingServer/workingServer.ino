#include <WiFi.h>
#include <WebServer.h>

// WiFi Credentials
const char* ssid     = "UNILAG-DESIGN-STUDIO";
const char* password = "Design@24.";

// Web server on port 80
WebServer server(80);

// Variables
String playerName = ""; // Store the player's name
int killCount = 0;
bool killRegistered = false;

// HTML for the webpage
String webpage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Game Kill Tracker</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin-top: 50px;
    }
    .hidden {
      display: none;
    }
  </style>
</head>
<body>
  <h1>Game Kill Tracker</h1>
  <div id="register">
    <h2>Register Player</h2>
    <input type="text" id="playerName" placeholder="Enter your name" />
    <button onclick="registerPlayer()">Register</button>
  </div>
  <div id="game" class="hidden">
    <h2>Welcome, <span id="displayName"></span>!</h2>
    <p>Your Kill Count: <span id="killCount">0</span></p>
  </div>
  <script>
    // Register the player
    function registerPlayer() {
      const playerName = document.getElementById("playerName").value;

      if (playerName) {
        document.getElementById("displayName").textContent = playerName;
        document.getElementById("register").classList.add("hidden");
        document.getElementById("game").classList.remove("hidden");

        // Send the player's name to the ESP32
        fetch(`/register?name=${encodeURIComponent(playerName)}`)
          .then(response => response.text())
          .then(data => {
            console.log("Response from ESP32:", data);
          })
          .catch(err => console.error("Error:", err));
      } else {
        alert("Please enter your name!");
      }
    }

    // Fetch the kill count
    function fetchKillCount() {
      fetch("/killcount")
        .then(response => response.text())
        .then(data => {
          document.getElementById("killCount").textContent = data;
        });
    }

    // Poll kill count every second
    setInterval(fetchKillCount, 1000);
  </script>
</body>
</html>
)rawliteral";

// Handle the main page
void handleRoot() {
  server.send(200, "text/html", webpage);
}

// Handle the kill count
void handleKillCount() {
  server.send(200, "text/plain", String(killCount));
}

// Handle player registration
void handleRegister() {
  if (server.hasArg("name")) {
    playerName = server.arg("name"); // Get the player's name
    Serial.println("Player registered: " + playerName);
    server.send(200, "text/plain", "Player registered successfully!");
  } else {
    server.send(400, "text/plain", "Name not provided.");
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

  // Set up web server
  server.on("/", handleRoot);
  server.on("/killcount", handleKillCount);
  server.on("/register", handleRegister); // New endpoint for player registration
  server.begin();
  Serial.println("HTTP server started");

  // Set up a button for simulating kills
  pinMode(2, INPUT_PULLUP); // Button connected to GPIO 2
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(2) == LOW && !killRegistered) {
    killCount++;
    killRegistered = true;
    delay(200); // Debounce
  }

  // Reset the flag when the button is released
  if (digitalRead(2) == HIGH) {
    killRegistered = false;
  }

  // Handle client requests
  server.handleClient();
}
