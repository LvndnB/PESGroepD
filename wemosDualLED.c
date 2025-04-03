#include <ESP8266WiFi.h>

const char* ssid = "Revalidatie";
const char* password = "Revalidatie";
const int serverPort = 12345;
const int roodledPin = D8;
const int groenledPin = D7;

WiFiServer server(serverPort);
char key[50];
int value = 0;


void setup() {
  Serial.begin(9600);
  pinMode(roodledPin, OUTPUT);
  pinMode(groenledPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  server.begin();
}

// Functie voor besturen LED
void bestuurLED(int pin, int status) {
  digitalWrite(pin, !status);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String input = client.readStringUntil('\n');
    input.trim();
    if (sscanf(input.c_str(), "%49[^ = ] = %d", key, &value) == 2) {  // String parsing voor key = value
      if (value == 1 || value == 0) {                                 // Aansturing van besturenLED functie
        if (strcmp(key, "DUALLEDROOD") == 0) {
          bestuurLED(roodledPin, value);
        }
        if (strcmp(key, "DUALLEDGROEN") == 0) {
          bestuurLED(groenledPin, value);
        }
      }
    }
  }
  client.stop();
}
