#include <ESP8266WiFi.h>              // This includes the libraries to connect to wifi and start a server

const char* wifiSSID = "BYU-WiFi";    // In order for this to work, you MUST specify the SSID for your wifi
const char* wifiPSK  = "";            // And the preshared key (wifi password)

WiFiServer server(80);                // This sets which port our server will listen on

int ledPin = D5;                      // Sets which GPIO pin we will use for the LED
String ledState = "off";              // Initialize a variable for tracking the state of the LED

void setup() {                        // Perform this part once when first powered on
  Serial.begin(115200);               // Open a serial connection (for debugging)
  delay(10);                          // Wait 10 milliseconds (1/100th of a second)
  Serial.println("Initializing ..."); // DEBUG CODE

  pinMode(ledPin, OUTPUT);            // Set the LED pin to Output
  digitalWrite(ledPin, LOW);          // Set the pin to LOW (Off)

  // ** Connect to WiFi network - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  Serial.print("Connecting to ");           // Display debugging connection info
  Serial.println(wifiSSID);                 // Print the configured SSID to the Serial Monitor

  WiFi.begin(wifiSSID, wifiPSK);            // Use the provided SSID and PSK to connect

  while (WiFi.status() != WL_CONNECTED) {   // If not connected to wifi
    delay(500);                             // Pause
    Serial.print(".");                      // Print a dot each loop while trying to connect
  }
  Serial.println("");
  Serial.println("WiFi connected");         // Print "connected" message to the Serial Monitor

  server.begin();                           // Start the web server
  Serial.println("Server started");

  Serial.print("Control the LED Flashlight at: ");          // Print the connected IP address to the Serial Monitor
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
} // ** End Adapted Code - This is the end of the code that was adapted from www.esp8266learning.com

void loop() { // ** Create a web server - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  WiFiClient client = server.available(); // Create a new client object for available connections
  if (client) {                           // If a client is connected, wait until it sends some data
    while (!client.available()) {         // If the client hasn't sent info, wait for it
      delay(10);
    }

    String request = client.readStringUntil('\r');  // read the first line of the request
    Serial.println(request);              // DEBUG CODE - Echo the request to the Serial Monitor for debug
    client.flush();                       // Wait until the buffers are clear

    if (request.indexOf("/led=on") != -1) {   // If the request is for the page "/led=on"
      digitalWrite(ledPin, HIGH);             //   then set the ledPin to high (turn on)
      ledState = "on";                        //   and track the state as "on"
    }
    if (request.indexOf("/led=off") != -1) {  // If the request is for the page "/led=off"
      digitalWrite(ledPin, LOW);              //   then set the ledPin to low (turn off)
      ledState = "off";                       //   and track the state as "off"
    }
    // ** End Adapted Code - This is the end of the code that was adapted from www.esp8266learning.com

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");

    // The concept and code of the sliding button on this page came from https://www.w3schools.com/howto/howto_css_switch.asp
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>");
    client.println("html {text-align: center;}");
    client.println(".switch { position: relative; display: inline-block; width: 60px; height: 34px;}");
    client.println(".switch input {display:none;}");
    client.println(".slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; -webkit-transition: .4s; transition: .4s;}");
    client.println(".slider:before {position: absolute; content: \"\"; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; -webkit-transition: .4s;transition: .4s;}");
    client.println("input:checked + .slider {background-color: #2196F3;}");
    client.println("input:focus + .slider {box-shadow: 0 0 1px #2196F3;}");
    client.println("input:checked + .slider:before {-webkit-transform: translateX(26px); -ms-transform: translateX(26px); transform: translateX(26px);}");
    client.println(".slider.round { border-radius: 34px;}");
    client.println(".slider.round:before { border-radius: 50%;}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");

    client.println("<h2>The Flashlight is currently ");
    client.println(ledState);
    client.println("</h2>");

    // This section modifies the page HTML based on the current state of the LED
    client.println("<label class='switch'>");
    client.print("  <input type=\"checkbox\" ");
    if (ledState == "on") {
      client.print("checked ");
    }
    client.print("onchange=\"window.location.href='/led=");
    if (ledState == "on") {
      client.print("off");
    } else {
      client.print("on");
    }
    client.println("'\">");
    client.println("  <span class=\"slider round\"></span>");
    client.println("</label>");
    client.println("</body>");
    client.println("</html>");
  }
  delay(100); // This introduces a little pause in each cycle. Probably helps save some power.
}

