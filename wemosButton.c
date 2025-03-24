#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "SSID";
const char* password = "Password";
const char* serverIP = "192.168.1.166";  // IP van de Raspberry Pi
const int serverPort = 12345;
const int buttonPin = D8;


WiFiClient client;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    if (client.connect(serverIP, serverPort)) {
      client.print("knop ingedrukt\n");
      client.stop();
    }
    delay(250);
  }
}
