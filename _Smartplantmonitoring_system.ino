#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char *ssid = "Your_SSID";
const char *password = "Your_PASSWORD";

// Set your sensor pins
#define DHTPIN D4
#define DHTTYPE DHT11
#define PIRPIN D5
#define SOILMOISTUREPIN A0
#define PUMPPIN D6 // Define the pump pin

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Create an instance of the server on port 80
ESP8266WebServer server(80);

// Soil moisture threshold
#define SOIL_MOISTURE_THRESHOLD 450

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  delay(100);

  // Initialize the DHT sensor
  dht.begin();

  // Initialize PIR sensor pin
  pinMode(PIRPIN, INPUT);

  // Initialize pump pin
  pinMode(PUMPPIN, OUTPUT);
  digitalWrite(PUMPPIN, LOW); // Ensure pump is off initially

  // Set up the Access Point
  Serial.println("Setting up Access Point...");
  WiFi.softAP(ssid, password);

  // Print the IP address
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define the webpage to be served
  server.on("/", handleRoot);
  
  // Define the JSON endpoint for sensor data
  server.on("/data", handleData);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}

void handleRoot() {
  // HTML template with JavaScript for periodic AJAX requests
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>Sensor Data</title>";
  html += "<style>";
  html += "body { background-color: #1976d2; font-family: 'Arial', sans-serif; background-color: #f0f0f5; margin: 0; padding: 20px; display: flex; justify-content: center; align-items: center; height: 100vh; }";
  html += ".container { background-color: #ffffff; padding: 30px; border-radius: 12px; box-shadow: 0 0 20px rgba(0, 0, 0, 0.1); width: 400px; text-align: center; }";
  html += "h1 { color: #333; margin-bottom: 30px; font-size: 28px; }";
  html += ".sensor-box { padding: 15px; margin-bottom: 20px; border-radius: 10px; box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.05); font-size: 20px; }";
  html += ".temperature-box { background-color: #ffe6e6; }";
  html += ".humidity-box { background-color: #e6ffe6; }";
  html += ".pir-box { background-color: #e6f2ff; }";
  html += ".soil-box { background-color: #fff9e6; }";
  html += ".sensor-box p { margin: 10px 0; }";
  html += ".temperature-box p { color: #d32f2f; }";
  html += ".humidity-box p { color: #388e3c; }";
  html += ".pir-box p { color: #1976d2; }";
  html += ".soil-box p { color: #f57c00; }";
  html += ".sensor-box span { font-weight: bold; }";
  html += ".footer { text-align: center; margin-top: 20px; font-size: 16px; color: #777; }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class=\"container\">";
  html += "<h1>Sensor Data</h1>";
  html += "<div class=\"sensor-box temperature-box\">";
  html += "<p>Temperature: <span id=\"temperature\">Loading...</span> &deg;C</p>";
  html += "</div>";
  html += "<div class=\"sensor-box humidity-box\">";
  html += "<p>Humidity: <span id=\"humidity\">Loading...</span> %</p>";
  html += "</div>";
  html += "<div class=\"sensor-box pir-box\">";
  html += "<p>PIR Motion: <span id=\"pir\">Loading...</span></p>";
  html += "</div>";
  html += "<div class=\"sensor-box soil-box\">";
  html += "<p>Soil Moisture: <span id=\"soilMoisture\">Loading...</span></p>";
  html += "</div>";
  html += "<div class=\"footer\">";
  html += "<p>Data refreshes every 1 seconds</p>";
  html += "</div>";
  html += "</div>";
  html += "<script>";
  html += "function getData() {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/data', true);";
  html += "xhr.onreadystatechange = function() {";
  html += "if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "var data = JSON.parse(xhr.responseText);";
  html += "document.getElementById('temperature').innerHTML = data.temperature;";
  html += "document.getElementById('humidity').innerHTML = data.humidity;";
  html += "document.getElementById('pir').innerHTML = data.pir == 1 ? 'Motion Detected' : 'No Motion';";
  html += "document.getElementById('soilMoisture').innerHTML = data.soilMoisture;";
  html += "}";
  html += "};";
  html += "xhr.send();";
  html += "}";
  html += "setInterval(getData, 500);";
  html += "window.onload = getData;";
  html += "</script>";
  html += "</html>";
  html += "</body>";

  // Send the HTML response
  server.send(200, "text/html", html);
}

void handleData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int pirState = digitalRead(PIRPIN);
  int soilMoistureValue = analogRead(SOILMOISTUREPIN);

  // Check if any reads failed and exit early (to try again)
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    server.send(500, "application/json", "{\"error\":\"Failed to read from DHT sensor!\"}");
    return;
  }

  // Control the pump based on soil moisture level
  if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD) {
    digitalWrite(PUMPPIN, HIGH);
    Serial.println("Pump ON");
  } else {
    digitalWrite(PUMPPIN, LOW);
    Serial.println("Pump OFF");
  }

  // Create JSON object
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["temperature"] = t;
  jsonDoc["humidity"] = h;
  jsonDoc["pir"] = pirState;
  jsonDoc["soilMoisture"] = soilMoistureValue;

  // Serialize JSON object to string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send JSON response
  server.send(200, "application/json", jsonString);
}
