# Smart-plant-monitoring-system-
Project Report: Smart Plant
Monitoring System
1. Introduction
The aim of this project is to develop an automated system for monitoring environmental
conditions and controlling a water pump based on soil moisture levels using the ESP8266
microcontroller. The system is designed to help in the efficient management of plant care
continuously monitoring temperature, humidity, motion, and soil moisture. These readings are
transmitted to a web interface, which displays the data in real-time and allows users to monitor
conditions remotely. Additionally, the system automates irrigation by activating a water pump
when the soil moisture falls below a defined threshol
2. Objectives
Primary Objectives:
- Real-time Monitoring: Continuously monitor environmental conditions such as temperature,
humidity, motion, and soil moisture to ensure optimal plant care.
- Automated Control: Automate the control of a water pump based on soil moisture readings
to ensure plants are watered adequately.
- Web Interface: Provide a web interface for users to view real-time data and monitor the
status of the water pump.
Secondary Objectives:
- Ease of Use: Ensure the system is user-friendly, easy to set up, and accessible via any
Wi-Fi-enabled device.
- Expandability: Design the system in a way that allows for easy addition of more sensors and
functionalities in the future.
3. Components
Hardware
ESP8266 WiFi Module
- Description: A low-cost microcontroller with built-in Wi-Fi, widely used for IoT applications due
to its ease of programming and strong community support.
- Function: Acts as the central controller for the system, handling sensor data acquisition, web
server hosting, and pump control.
DHT11 Sensor
- Description: A basic, low-cost digital sensor for measuring temperature and humidity.
- Function: Provides temperature and humidity readings to monitor environmental conditions
around the plants.
PIR Motion Sensor
- Description: A sensor that detects infrared radiation to sense motion.
- Function: Detects movement within its range, providing an additional security feature or
triggering events based on motion around the plant area.
Soil Moisture Sensor
- Description: An analog sensor that measures the volumetric content of water in the soil.
- Function: Measures soil moisture levels to determine when the water pump should be
activated to irrigate the soil.
Water Pump
- Description: A small, controllable pump that can be used for irrigation purposes.
- Function: Activates to water the soil when the soil moisture sensor indicates that the moisture
level is below the set threshold.
Power Supply
- Description: Provides the necessary voltage and current to power the ESP8266 and
connected sensors.
- Function: Ensures all components receive stable power for reliable operation.
Breadboard and Jumper Wires
- Description: Tools for prototyping circuits and making connections between components
without soldering.
- Function: Allow easy and flexible wiring of the components during the development phas
Software
Arduino IDE
- Description: An open-source platform used for writing and uploading code to
microcontrollers.
- Function: Provides the environment to develop and upload the firmware for the ESP826
Libraries
1. ESP8266WiFi: Manages the Wi-Fi connectivity of the ESP8266.
2. ESP8266WebServer: Creates a web server on the ESP8266 to serve web pages and
handle HTTP requests.
3. DHT: Reads data from the DHT11 temperature and humidity sensor.
4. ArduinoJson: Handles the creation and parsing of JSON objects for data communication.
4. Circuit Diagram
The circuit diagram visually represents the connections between the ESP8266 and the sensors. It
is crucial for understanding how to wire the components correctly.
Connections:
- DHT11 Sensor: Connect the VCC to 3.3V, GND to GND, and Data pin to GPIO D4.
- PIR Sensor: Connect the VCC to 5V (or 3.3V depending on the model), GND to GND, and
Output pin to GPIO D5.
- Soil Moisture Sensor: Connect the VCC to 5V, GND to GND, and Analog Output pin to A0.
- Water Pump: Connect through a relay module to GPIO D6 for safe and reliable switching.
5. Software Implementation
WiFi Setup
The ESP8266 is set up as an access point, creating its own Wi-Fi network for devices to connect
to:
cpp
const char *ssid = "Your_SSID";
const char *password = "Your_PASSWORD";
WiFi.softAP(ssid, password);
- Rationale: Creating an access point allows users to connect directly to the ESP8266 without
needing an external Wi-Fi network.
Server Initialization
The web server handles HTTP requests and serves web pages:
cpp
ESP8266WebServer server(80);
server.on("/", handleRoot);
server.on("/data", handleData);
server.begin();
- Function: Initializes the server, defines the endpoints, and starts the server to listen for incomi
requests.
Sensor Reading and Control
The code reads data from the sensors and controls the water pump based on soil moisture
levels.
DHT11 Sensor:
cpp
float h = dht.readHumidity(
float t = dht.readTemperature(
- Function: Reads and stores humidity and temperature values.
PIR Sensor:
cpp
int pirState = digitalRead(PIRPIN);
- Function: Reads the state of the PIR sensor to detect motion.
Soil Moisture Sensor:
cpp
int soilMoistureValue = analogRead(SOILMOISTUREPIN);
if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD) {
digitalWrite(PUMPPIN, HIGH);
} else {
digitalWrite(PUMPPIN, LOW);
}
- Function: Reads soil moisture levels and activates/deactivates the water pump accordingly.
Web Interface
The web interface displays sensor data and updates dynamically using AJAX:
html
<div class="container">
<h1>Sensor Data</h1>
<div class="sensor-box temperature-box">
<p>Temperature: <span id="temperature">Loading...</span> &deg;C</p>
</div>
<div class="sensor-box humidity-box">
<p>Humidity: <span id="humidity">Loading...</span> %</p>
</div>
<div class="sensor-box pir-box">
<p>PIR Motion: <span id="pir">Loading...</span></p>
</div>
<div class="sensor-box soil-box">
<p>Soil Moisture: <span id="soilMoisture">Loading...</span></p>
</div>
</div>
<script>
function getData() {
var xhr = new XMLHttpRequest();
xhr.open('GET', '/data', true);
xhr.onreadystatechange = function() {
if (xhr.readyState == 4 && xhr.status == 200) {
var data = JSON.parse(xhr.responseText);
document.getElementById('temperature').innerHTML = data.temperature;
document.getElementById('humidity').innerHTML = data.humidity;
document.getElementById('pir').innerHTML = data.pir == 1 ? 'Motion Detected' : 'No Motion';
document.getElementById('soilMoisture').innerHTML = data.soilMoisture;
}
};
xhr.send();
}
setInterval(getData, 1000);
window.onload = getData;
</script>
- Function: Sends periodic requests to the server to fetch the latest sensor data and updates the
web page elements.
JSON Data Endpoint
The `/data` endpoint reads sensor values, creates a JSON object, and sends it as a response:
cpp
void handleData() {
StaticJsonDocument<200> jsonDoc;
jsonDoc["temperature"] = t;
jsonDoc["humidity"] = h;
jsonDoc["pir"] = pirState;
jsonDoc["soilMoisture"] = soilMoistureValue;
String jsonString;
serializeJson(jsonDoc, jsonString);
server.send(200, "application/json", jsonString);
}
- Function: Compiles sensor data into a JSON object and serves it to the client.
6. Testing and Results
Live Demo
To demonstrate the system's functionality:
1. Connect: Connect to the ESP8266 access point using a mobile device or laptop.
2. Access: Open the provided IP address in a web browser.
3. Monitor: Observe real-time sensor data updates on the web interface.
4. Automate: Demonstrate the automatic control of the water pump based on soil moisture levels.
Screenshots
Include screenshots of the web interface showing real-time sensor data and pump status.
[Insert screenshots here]
7. Conclusion
This project successfully developed a Smart Plant Monitoring System using the ESP8266
microcontroller for real-time monitoring and control of environmental conditions. The
user-friendly web interface effectively displays sensor data, and the water pump is automatical
controlled based on soil moisture levels.
Summary
- Real-time Monitoring: Accurate and continuous monitoring of temperature, humidity, motion, and
soil moisture.
- Automated Control: Efficient control of the water pump to maintain optimal soil conditio
- Web Interface: Intuitive and accessible interface for real-time data visualization.
Future Work
- Additional Sensors: Integrate sensors for light, CO2, and other environmental parameters.
- Data Logging
: Implement data logging features to track historical data and analyze trends.
- Mobile App: Develop a mobile application for easier access and control.
- Advanced Irrigation Control: Implement more advanced irrigation algorithms based on
plant-specific needs and environmental condition
