#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// GitHub Page = https://github.com/Tommrodrigues/homebridge-web-valve

// Script Type = Modulating relay

// D7 = Relay

/////////////////// CHANGE THESE VALUES //////////////////////
const char* ssid = "SSID"; //Name of your network
const char* password = "PASSWORD"; //Password for your network
const String relay = "HIGH"; //Relay type (`HIGH` or `LOW`)
const char* mdns = "valve"; //mDNS name
const int modulationOn = 10000; //Time (in ms) for relay to be ON when modulating
const int modulationOff = 20000; //Time (in ms) for relay to be OFF when modulating
//////////////////////////////////////////////////////////////

const int relayPin = 13;
int state = 0;
int modState = 0;

int relayOn, relayOff;

unsigned long lastMillis = millis();

WiFiServer server(80);

void setup() {
  if (relay.equals("LOW")) {
    relayOn = 0;
    relayOff = 1;
  } else {
    relayOn = 1;
    relayOff = 0;
  }

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayOff);

  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.println("Connecting to \"" + String(ssid) + "\"");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(String(++i) + " ");
  }
  Serial.println();
  Serial.println("Connected successfully");

  // Start the server
  server.begin();

  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(mdns)) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS address: " + String(mdns) + ".local");
}

//Main loop
void loop() {

  if (state) {
    if (!modState && millis() - lastMillis > modulationOff) {
      digitalWrite(relayPin, relayOn);
      modState = 1;
      lastMillis = millis();
    } else if (modState && millis() - lastMillis > modulationOn) {
      digitalWrite(relayPin, relayOff);
      modState = 0;
      lastMillis = millis();
    }
  } else {
    digitalWrite(relayPin, relayOff);
    modState = 0;
  }

  MDNS.update();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("New client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  // Match the request
  if (request.indexOf("/setState") != -1) {
    state = request.substring(14, 15).toInt();
  }

  if (request.indexOf("/status") != -1) {
    client.println("{\"currentState\": " + String(state) + "}");
  }

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
