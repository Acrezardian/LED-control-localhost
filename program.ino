#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Nama Wifi";
const char* password = "Password Wifi";

ESP8266WebServer server(80);

const int ledPin = D1; // Pin D4 untuk LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Atur pin D1 sebagai output
  digitalWrite(ledPin, LOW); // Pastikan LED dimatikan saat startup

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>LED Control</h1>";
  html += "<p>Click to control the LED:</p>";
  html += "<button onclick=\"toggleLed(true)\">Turn On</button>";
  html += "<button onclick=\"toggleLed(false)\">Turn Off</button>";

  html += "<script>";
  html += "function toggleLed(state) {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', state ? '/on' : '/off', true);";
  html += "xhr.send();";
  html += "}";
  html += "</script>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH); // Menyalakan LED
  server.send(200, "text/plain", "LED is on");
}

void handleLedOff() {
  digitalWrite(ledPin, LOW); // Mematikan LED
  server.send(200, "text/plain", "LED is off");
}
