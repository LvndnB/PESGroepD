#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "SSID";
const char* password = "Password";
const int serverPort = 12345;
const int redledPin = D8;
const int greenledPin = D7;

WiFiServer server(serverPort);
int ledStatus = 0;

void setup() {
  Serial.begin(9600);
  pinMode(redledPin, OUTPUT);
  pinMode(greenledPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String message = client.readStringUntil('\n');
    message.trim();
    if (message = "knop ingedrukt") {
      if (ledStatus == 0) {
        ledStatus = 1;
      } else {
        ledStatus = 0;
      }
      digitalWrite(redledPin, ledStatus);
    }
    client.stop();
  }
}
