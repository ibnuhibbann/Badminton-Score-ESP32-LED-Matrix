#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 5

// Your WiFi credentials
const char* ssid = "score";
const char* password = "score54321";

// Create an instance of the MD_Parola class
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Create a WebServer instance
WebServer server(80);

// Global variables for scores
int player1Score = 0;
int player2Score = 0;

void setup() {
  Serial.begin(115200);

  // Initialize the display
  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();

  // Menghubungkan ke WiFi
  // connectToWiFi();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Display.displayScroll("Connecting", PA_LEFT, PA_SCROLL_LEFT, 45);
    while (!Display.displayAnimate()) {
      // Menunggu hingga animasi selesai
    }
  }

  Serial.println("Connected to WiFi");

  // Print the IP address
  String ipAddress = WiFi.localIP().toString();
  Serial.print("IP Address: ");
  Serial.println(ipAddress);
  // Display.displayScroll(ipAddress.c_str(), PA_LEFT, PA_SCROLL_LEFT, 150);   // Display the IP address on the LED matrix with scrolling effect
  // while (!Display.displayAnimate()) {
  //   // Wait for the animation to complete
  // }

  // Menampilkan alamat IP di matriks LED dengan efek scrolling dua kali
  for (int i = 0; i < 2; i++) {
    Display.displayScroll(ipAddress.c_str(), PA_LEFT, PA_SCROLL_LEFT, 100);
    while (!Display.displayAnimate()) {
      // Menunggu hingga animasi selesai
    }
  }

  // Display the initial score on LED matrix
  updateDisplay();  // Show initial score (0-0)

  // Serve the web page
  server.on("/", HTTP_GET, handleRoot);
  server.on("/updateScore", HTTP_GET, handleUpdateScore);
  server.on("/resetScores", HTTP_GET, handleResetScores);
  server.on("/getScores", HTTP_GET, handleGetScores);
  server.begin();
}

void loop() {
  // Memeriksa status koneksi WiFi dan mencoba menghubungkan kembali jika terputus
  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
  }

  server.handleClient(); // Handle incoming client requests
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Display.displayScroll("Disconnected", PA_LEFT, PA_SCROLL_LEFT, 45);
    while (!Display.displayAnimate()) {
      // Menunggu hingga animasi selesai
    }
  }

  Serial.println("Connected to WiFi");

  // Print the IP address
  String ipAddress = WiFi.localIP().toString();
  Serial.print("IP Address: ");
  Serial.println(ipAddress);

  // Menampilkan alamat IP di matriks LED dengan efek scrolling dua kali
  for (int i = 0; i < 2; i++) {
    Display.displayScroll(ipAddress.c_str(), PA_LEFT, PA_SCROLL_LEFT, 100);
    while (!Display.displayAnimate()) {
      // Menunggu hingga animasi selesai
    }
  }

  // Display the initial score on LED matrix
  updateDisplay();  // Show initial score (0-0)

}

void reconnectWiFi() {
  WiFi.disconnect();
  connectToWiFi();
  Serial.println("Reconnected to WiFi");
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang=\"en\"><head>"
                "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                "<title>Badminton Scoreboard</title><style>"
                "body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f9; margin: 0; padding: 20px; }"
                "h1 { color: #333; margin-bottom: 10px; }"
                "h2 { margin-top: 2px; margin-bottom: 10px; }"
                "small { display: block; margin-bottom: 20px; color: #777; }"
                ".score-container { display: flex; justify-content: center; gap: 20px; margin-bottom: 20px; flex-wrap: wrap; }"
                ".score-box { font-size: 24px; width: 200px; padding: 20px; background-color: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }"
                ".buttons-container { margin-top: 10px; }"
                "button { font-size: 18px; padding: 10px 15px; margin: 5px; cursor: pointer; border: 1px solid #ccc; border-radius: 5px; background-color: #fff; width: 80px; }"
                "button:hover { background-color: #f0f0f0; }"
                ".reset-scores{  background-color: #ec3b3b; color: #f0f0f0; font-weight: bold; width: 150px; }"
                ".reset-scores:hover { background-color: #b81212; color: #f0f0f0; }"
                ".score-players{ font-size: 75px; }"
                ".switch-positions { background-color: #4CAF50; color: white; width: 150px; }"
                ".switch-positions:hover { background-color: #45a049; }"
                "@media (max-width: 600px) { .score-container { flex-direction: column; } .score-box { width: 100%; max-width: 285px; margin-bottom: 3px; margin: 0 auto; } }"
                "</style></head><body>"
                "<h1>Badminton Scoreboard</h1><small>Project By Ibnu Hibban</small>"
                "<div class=\"score-container\" id=\"score-container\"><div class=\"score-box\" id=\"player1-box\" data-position=\"1\">"
                "<h2>Team 1</h2>"
                "<small style=\"font-size: medium; color: red; font-weight: bold; margin-top: -4px;\">Left</small>"
                "<div class=\"score-players\" id=\"score-player1\">Loading...</div>"
                "<div class=\"buttons-container\">"
                "<button onclick=\"updateScore('player1', -1)\">-</button>"
                "<button onclick=\"updateScore('player1', 1)\">+</button>"
                "</div></div><div class=\"score-box\" id=\"player2-box\" data-position=\"2\">"
                "<h2>Team 2</h2>"
                "<small style=\"font-size: medium; color: red; font-weight: bold; margin-top: -4px;\">Right</small>"
                "<div class=\"score-players\" id=\"score-player2\">Loading...</div>"
                "<div class=\"buttons-container\">"
                "<button onclick=\"updateScore('player2', -1)\">-</button>"
                "<button onclick=\"updateScore('player2', 1)\">+</button>"
                "</div></div></div>"
                "<button class=\"reset-scores\" onclick=\"confirmResetScores()\">Reset Scores</button>"
                "<button class=\"switch-positions\" onclick=\"switchPositions()\">Tukar Posisi</button>"
                "<script>"
                "  let isSwitched = false;"
                "  let originalOrder = [];"

                "function updateScore(player, delta) {"
                "  var xhttp = new XMLHttpRequest();"
                "  xhttp.open('GET', '/updateScore?player=' + player + '&delta=' + delta, true);"
                "  xhttp.send();"
                "  fetchScores();"
                "}"
                "function confirmResetScores() {"
                "  if (confirm('Apakah anda yakin mereset score?')) {"
                "    resetScores();"
                "  }"
                "}"
                "function resetScores() {"
                "  var xhttp = new XMLHttpRequest();"
                "  xhttp.open('GET', '/resetScores', true);"
                "  xhttp.send();"
                "  fetchScores();"
                "}"
                "function fetchScores() {"
                "  var xhttp = new XMLHttpRequest();"
                "  xhttp.onreadystatechange = function() {"
                "    if (this.readyState == 4 && this.status == 200) {"
                "      const scores = JSON.parse(this.responseText);"
                "      document.getElementById('score-player1').innerText = scores.player1;"
                "      document.getElementById('score-player2').innerText = scores.player2;"
                "    }"
                "  };"
                "  xhttp.open('GET', '/getScores', true);"
                "  xhttp.send();"
                "}"
                "function switchPositions() {"
                "  const container = document.getElementById('score-container');"
                "    const player1Box = document.getElementById('player1-box');"
                "    const player2Box = document.getElementById('player2-box');"
                "    if (!isSwitched) {"
                "        if (originalOrder.length === 0) {"
                "            originalOrder.push(container.children[0].id);"
                "            originalOrder.push(container.children[1].id);"
                "        }"
                "        container.insertBefore(player2Box, player1Box);"
                "    } else {"
                "       if (originalOrder.length > 0) {"
                "            const first = document.getElementById(originalOrder[0]);"
                "            const second = document.getElementById(originalOrder[1]);"
                "            container.insertBefore(first, second);"
                "        }"
                "   }"
                "    isSwitched = !isSwitched;"
                "}"
                "fetchScores();"
                "</script></body></html>";
  server.send(200, "text/html", html);
}

void handleUpdateScore() {
  if (server.hasArg("player") && server.hasArg("delta")) {
    String player = server.arg("player");
    int delta = server.arg("delta").toInt();
    
    if (player == "player1") {
      player1Score += delta;
      if (player1Score < 0) player1Score = 0; // Ensure score doesn't go negative
    } else if (player == "player2") {
      player2Score += delta;
      if (player2Score < 0) player2Score = 0; // Ensure score doesn't go negative
    }

    // Update LED display to match the new score
    updateDisplay();
  }
  server.send(200, "text/plain", "Score updated");
}

void handleResetScores() {
  player1Score = 0;
  player2Score = 0;
  updateDisplay(); // Update display immediately after reset
  server.send(200, "text/plain", "Scores reset");
}

void handleGetScores() {
  String scores = "{\"player1\":" + String(player1Score) + ",\"player2\":" + String(player2Score) + "}";
  server.send(200, "application/json", scores);
}

void updateDisplay() {
  String scoreString = String(player1Score) + "-" + String(player2Score);
  Display.setTextAlignment(PA_CENTER);
  Display.print(scoreString);
}
