#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

// WiFi Credentials
// const char* ssid     = "iPhone";
// const char* password = "3902608018";

// const char* ssid     = "UNILAG-DESIGN-STUDIO";
// const char* password = "Design@24.";

const char* ssid = "Redmi Note 12";
const char* password = "wb845q4fmpura7x";


// Web server and WebSocket
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Player Variables
String playerName = "";
String playerTeam = "";
int playerPoints = 0;
int playerHealth = 100;
bool damageRegistered = false;

// Sensor Pins
#define BUTTON_PIN 2        // Button for increasing points
#define DAMAGE_SENSOR_PIN 5 // Sensor for detecting damage

// HTML file content (replace this with the content of your file)
// HTML for the webpage
String webpage = R"rawliteral(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IR Combat Arena</title>
    <script src="https://unpkg.com/lucide@latest"></script>
    <style>
        /* Base styles */
        :root {
            --bg-gradient-from: #1a1a2e;
            --bg-gradient-to: #1a1a1a;
            --card-bg: rgba(31, 31, 35, 0.5);
            --border-color: rgba(75, 75, 75, 0.7);
            --input-bg: rgba(55, 55, 55, 0.7);
            --health-bar-bg: rgba(55, 55, 55, 0.7);
            --red: #f87171;
            --blue: #60a5fa;
            --green: #4ade80;
            --yellow: #fbbf24;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
            background: linear-gradient(135deg, var(--bg-gradient-from), var(--bg-gradient-to));
            color: white;
            min-height: 100vh;
            padding: 1.5rem;
            line-height: 1.5;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
        }

        h1 {
            font-size: 2.5rem;
            text-align: center;
            margin-bottom: 2rem;
            background: linear-gradient(to right, var(--blue), var(--red));
            -webkit-background-clip: text;
            background-clip: text;
            color: transparent;
            font-weight: bold;
        }

        /* Grid layouts */
        .grid-2 {
            display: grid;
            grid-template-columns: 1fr;
            gap: 1.5rem;
            margin-bottom: 1.5rem;
        }

        .grid-3 {
            display: grid;
            grid-template-columns: 1fr;
            gap: 1.5rem;
        }

        @media (min-width: 768px) {
            .grid-2 {
                grid-template-columns: repeat(2, 1fr);
            }
            
            .grid-3 {
                grid-template-columns: repeat(3, 1fr);
            }
        }

        /* Card styles */
        .card {
            background: var(--card-bg);
            border-radius: 0.5rem;
            border: 1px solid var(--border-color);
            backdrop-filter: blur(8px);
            padding: 1.5rem;
        }

        .card h2 {
            font-size: 1.25rem;
            font-weight: 600;
            margin-bottom: 1rem;
            display: flex;
            align-items: center;
            gap: 0.5rem;
        }

        /* Form elements */
        .form-group {
            margin-bottom: 1rem;
        }

        .form-group label {
            display: block;
            font-size: 0.875rem;
            font-weight: 500;
            margin-bottom: 0.25rem;
        }

        input, select {
            width: 100%;
            padding: 0.5rem 0.75rem;
            background: var(--input-bg);
            border: none;
            border-radius: 0.25rem;
            color: white;
            font-size: 1rem;
        }

        input:focus, select:focus {
            outline: 2px solid var(--blue);
        }

        .points {
            font-size: 1.5rem;
            font-weight: bold;
        }

        /* Health bar */
        .health-container {
            margin-top: 1rem;
        }

        .health-header {
            display: flex;
            justify-content: space-between;
            margin-bottom: 0.25rem;
        }

        .health-bar {
            width: 100%;
            height: 1rem;
            background: var(--health-bar-bg);
            border-radius: 9999px;
            overflow: hidden;
        }

        .health-fill {
            width: 100%;
            height: 100%;
            background: linear-gradient(to right, var(--red), var(--green));
            border-radius: 9999px;
            transition: width 0.3s ease;
        }

        /* Team scores */
        .team-scores .score-row {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 0.5rem;
        }

        .leading-team {
            margin-top: 1rem;
            padding-top: 1rem;
            border-top: 1px solid var(--border-color);
        }

        .leading-team .label {
            font-size: 0.875rem;
        }

        .leading-team .value {
            font-size: 1.125rem;
            font-weight: bold;
        }

        /* Player stats */
        .player-stats .stat-row {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 1rem;
        }

        /* Game status */
        .status-indicator {
            display: flex;
            align-items: center;
            gap: 0.5rem;
        }

        .status-dot {
            width: 0.75rem;
            height: 0.75rem;
            background: var(--green);
            border-radius: 50%;
            animation: pulse 2s infinite;
        }

        .time-remaining {
            margin-top: 1rem;
        }

        .time-remaining .label {
            font-size: 0.875rem;
            color: #9ca3af;
        }

        .time-remaining .value {
            font-size: 1.5rem;
            font-weight: bold;
        }

        /* Utility classes */
        .text-red { color: var(--red); }
        .text-blue { color: var(--blue); }
        .text-green { color: var(--green); }
        .text-yellow { color: var(--yellow); }

        /* Animations */
        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.5; }
            100% { opacity: 1; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>IR Combat Arena</h1>

        <!-- Player Info Section -->
        <div class="grid-2">
            <div class="card">
                <h2>
                    <i data-lucide="shield"></i>
                    Player Information
                </h2>
                <div class="content">
                    <div class="form-group">
                        <label>Player Name</label>
                        <input type="text" id="playerName" placeholder="Enter your name">
                    </div>
                    <div class="form-group">
                        <label>Team</label>
                        <select id="playerTeam">
                            <option value="red">Red Team</option>
                            <option value="blue">Blue Team</option>
                        </select>
                    </div>
                    <div class="form-group">
                        <label>Points</label>
                        <div class="points" id="playerPoints">0</div>
                    </div>
                </div>
            </div>

            <div class="card">
                <h2>
                    <i data-lucide="heart" class="text-red"></i>
                    Health Status
                </h2>
                <div class="content">
                    <div class="health-container">
                        <div class="health-header">
                            <span>Health</span>
                            <span id="healthValue">100%</span>
                        </div>
                        <div class="health-bar">
                            <div class="health-fill" id="healthFill"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Game Stats -->
        <div class="grid-3">
            <!-- Team Scores -->
            <div class="card">
                <h2>
                    <i data-lucide="trophy" class="text-yellow"></i>
                    Team Scores
                </h2>
                <div class="content">
                    <div class="team-scores">
                        <div class="score-row">
                            <span class="text-red">Red Team</span>
                            <span class="score" id="redScore">150</span>
                        </div>
                        <div class="score-row">
                            <span class="text-blue">Blue Team</span>
                            <span class="score" id="blueScore">120</span>
                        </div>
                        <div class="leading-team">
                            <div class="label">Leading Team</div>
                            <div class="value text-yellow" id="leadingTeam">Red Team</div>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Players Status -->
            <div class="card">
                <h2>
                    <i data-lucide="users"></i>
                    Players
                </h2>
                <div class="content">
                    <div class="player-stats">
                        <div class="stat-row">
                            <span>Total Players</span>
                            <span id="totalPlayers">12</span>
                        </div>
                        <div class="stat-row">
                            <span>Players Alive</span>
                            <span class="text-green" id="playersAlive">8</span>
                        </div>
                        <div class="stat-row">
                            <span>Players Eliminated</span>
                            <span class="text-red" id="playersEliminated">4</span>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Game Status -->
            <div class="card">
                <h2>
                    <i data-lucide="swords"></i>
                    Game Status
                </h2>
                <div class="content">
                    <div class="game-status">
                        <div class="status-indicator">
                            <div class="status-dot"></div>
                            <span>Game in Progress</span>
                        </div>
                        <div class="time-remaining">
                            <div class="label">Time Remaining</div>
                            <div class="value" id="timeRemaining">10:00</div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <script>
        const socket = new WebSocket("ws://your-webpage-url.com/ws");

        socket.onmessage = (event) => {
            const data = JSON.parse(event.data);
            updateHealth(data.health);
            updatePoints(data.points);
        };

        // Initialize Lucide icons
        lucide.createIcons();

        // Game state
        const gameState = {
            player: {
                name: '',
                team: 'red',
                points: 0,
                health: 100
            },
            game: {
                totalPlayers: 12,
                playersAlive: 8,
                redTeamScore: 150,
                blueTeamScore: 120,
                timeRemaining: 600 // 10 minutes in seconds
            }
        };

        // DOM Elements
        const playerNameInput = document.getElementById('playerName');
        const playerTeamSelect = document.getElementById('playerTeam');
        const playerPointsElement = document.getElementById('playerPoints');
        const healthValueElement = document.getElementById('healthValue');
        const healthFillElement = document.getElementById('healthFill');
        const redScoreElement = document.getElementById('redScore');
        const blueScoreElement = document.getElementById('blueScore');
        const leadingTeamElement = document.getElementById('leadingTeam');
        const totalPlayersElement = document.getElementById('totalPlayers');
        const playersAliveElement = document.getElementById('playersAlive');
        const playersEliminatedElement = document.getElementById('playersEliminated');
        const timeRemainingElement = document.getElementById('timeRemaining');

        // Event Listeners
        playerNameInput.addEventListener('input', (e) => {
            gameState.player.name = e.target.value;
        });

        playerTeamSelect.addEventListener('change', (e) => {
            gameState.player.team = e.target.value;
        });

        // Update UI functions
        function updateHealth(health) {
            gameState.player.health = health;
            healthValueElement.textContent = `${health}%`;
            healthFillElement.style.width = `${health}%`;
        }

        function updatePoints(points) {
            gameState.player.points = points;
            playerPointsElement.textContent = points;
        }

        function updateTeamScores(redScore, blueScore) {
            gameState.game.redTeamScore = redScore;
            gameState.game.blueTeamScore = blueScore;
            redScoreElement.textContent = redScore;
            blueScoreElement.textContent = blueScore;
            
            const leadingTeam = redScore > blueScore ? 'Red Team' : 'Blue Team';
            leadingTeamElement.textContent = leadingTeam;
        }

        function updatePlayerCounts(total, alive) {
            gameState.game.totalPlayers = total;
            gameState.game.playersAlive = alive;
            totalPlayersElement.textContent = total;
            playersAliveElement.textContent = alive;
            playersEliminatedElement.textContent = total - alive;
        }

        function formatTime(seconds) {
            const minutes = Math.floor(seconds / 60);
            const remainingSeconds = seconds % 60;
            return `${minutes}:${remainingSeconds.toString().padStart(2, '0')}`;
        }

        function updateTimeRemaining(seconds) {
            gameState.game.timeRemaining = seconds;
            timeRemainingElement.textContent = formatTime(seconds);
        }

        // Game timer
        let timerInterval = setInterval(() => {
            if (gameState.game.timeRemaining > 0) {
                updateTimeRemaining(gameState.game.timeRemaining - 1);
            } else {
                clearInterval(timerInterval);
                // Handle game over
            }
        }, 1000);

        // Initialize UI
        updateHealth(gameState.player.health);
        updatePoints(gameState.player.points);
        updateTeamScores(gameState.game.redTeamScore, gameState.game.blueTeamScore);
        updatePlayerCounts(gameState.game.totalPlayers, gameState.game.playersAlive);
        updateTimeRemaining(gameState.game.timeRemaining);

        // Example of how to simulate game events (for testing)
        function simulateHit() {
            const newHealth = Math.max(0, gameState.player.health - 20);
            updateHealth(newHealth);
        }

        function simulatePointGain() {
            updatePoints(gameState.player.points + 10);
        }

        // Uncomment to test the UI updates
        // setInterval(simulateHit, 3000);
        // setInterval(simulatePointGain, 5000);
    </script>
</body>
</html>
)rawliteral"; // Replace with actual HTML file content;

// Function to send data via WebSocket
void sendPlayerData() {
    String jsonData = "{\"points\":" + String(playerPoints) + ",\"health\":" + String(playerHealth) + "}";
    webSocket.broadcastTXT(jsonData);
}

// Handle the main page
void handleRoot() {
    server.send(200, "text/html", webpage);
}

// Handle player registration
void handleRegister() {
    if (server.hasArg("name") && server.hasArg("team")) {
        playerName = server.arg("name");
        playerTeam = server.arg("team");

        Serial.println("Player Registered!");
        Serial.println("Name: " + playerName);
        Serial.println("Team: " + playerTeam);

        server.send(200, "text/plain", "Player registered successfully!");
    } else {
        server.send(400, "text/plain", "Missing name or team.");
    }
}

// WebSocket event handling
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        Serial.printf("Received: %s\n", payload);
        String msg = (char*)payload;

        if (msg == "reset") {
            playerPoints = 0;
            playerHealth = 100;
        }

        sendPlayerData();
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
    server.on("/register", handleRegister);
    server.begin();
    Serial.println("HTTP server started");

    // Set up WebSocket
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // Set up sensor pins
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(DAMAGE_SENSOR_PIN, INPUT);
}

void loop() {
    server.handleClient();
    webSocket.loop();

    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();

        // Update player points when button is pressed
        // if (digitalRead(BUTTON_PIN) == LOW) {
          
        // }
        delay(1000);
          playerPoints++;
          // Serial.println("Button Pressed: Points Increased");
          sendPlayerData();

        // Update player health if damage sensor is triggered
        if (digitalRead(DAMAGE_SENSOR_PIN) == HIGH && !damageRegistered) {
            playerHealth -= 10;
            if (playerHealth < 0) playerHealth = 0;
            damageRegistered = true;
            Serial.println("Damage Taken: Health Decreased");
            sendPlayerData();
        }

        // Reset damage flag when sensor is not triggered
        if (digitalRead(DAMAGE_SENSOR_PIN) == LOW) {
            damageRegistered = false;
        }
    }
}



