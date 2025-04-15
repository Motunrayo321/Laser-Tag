#include <WiFi.h>

const char* ssid     = "UNILAG-DESIGN-STUDIO";
const char* password = "Design@24.";

// const char* ssid     = "HOMIE";
// const char* password = "BigBang419";

// const char* ssid     = "Redmi Note 13";
// const char* password = "homie123";

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 2;
const int output27 = 27;

WiFiServer server(80);

void handleClientRequest(WiFiClient client) {
  
}


void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\">");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<title>Game Kill Tracker</title>");
            client.println("<style>");
            client.println("body {");
            client.println("  font-family: Arial, sans-serif;");
            client.println("  text-align: center;");
            client.println("  margin-top: 50px;");
            client.println("}");
            client.println(".hidden {");
            client.println("  display: none;");
            client.println("}");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>Game Kill Tracker</h1>");
            client.println("<div id=\"register\">");
            client.println("<h2>Register Player</h2>");
            client.println("<input type=\"text\" id=\"playerName\" placeholder=\"Enter your name\" />");
            client.println("<button onclick=\"registerPlayer()\">Register</button>");
            client.println("</div>");
            client.println("<div id=\"game\" class=\"hidden\">");
            client.println("<h2>Welcome, <span id=\"displayName\"></span>!</h2>");
            client.println("<p>Your Kill Count: <span id=\"killCount\">0</span></p>");
            client.println("</div>");
            client.println("<script>");
            client.println("// Register the player");
            client.println("function registerPlayer() {");
            client.println("  const playerName = document.getElementById(\"playerName\").value;");
            client.println("  if (playerName) {");
            client.println("    document.getElementById(\"displayName\").textContent = playerName;");
            client.println("    document.getElementById(\"register\").classList.add(\"hidden\");");
            client.println("    document.getElementById(\"game\").classList.remove(\"hidden\");");
            client.println("    fetch(`/register?name=${encodeURIComponent(playerName)}`)");
            client.println("      .then(response => response.text())");
            client.println("      .then(data => {");
            client.println("        console.log(\"Response from ESP32:\", data);");
            client.println("      })");
            client.println("      .catch(err => console.error(\"Error:\", err));");
            client.println("  } else {");
            client.println("    alert(\"Please enter your name!\");");
            client.println("  }");
            client.println("}");
            client.println("// Fetch the kill count");
            client.println("function fetchKillCount() {");
            client.println("  fetch(\"/killcount\")");
            client.println("    .then(response => response.text())");
            client.println("    .then(data => {");
            client.println("      document.getElementById(\"killCount\").textContent = data;");
            client.println("    });");
            client.println("}");
            client.println("// Poll kill count every second");
            client.println("setInterval(fetchKillCount, 1000);");
            client.println("</script>");
            client.println("</body>");
            client.println("</html>");

            // End HTTP response
            client.println();
            client.stop();
            
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
