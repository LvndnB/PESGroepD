#include <ESP8266WiFi.h>

const char* ssid = "SSID";
const char* password = "Password";
const int serverPort = 12345;
const int redledPin = D8;

WiFiServer server(serverPort);
char key[50];
int value = 0;


void setup() {
  Serial.begin(9600);
  pinMode(redledPin, OUTPUT);
  pinMode(greenledPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  server.begin();
}

// Functie voor besturen LED
void bestuurLED(int pin, int status) {
  digitalWrite(pin, status);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String input = client.readStringUntil('\n');
    input.trim();
    if (sscanf(input.c_str(), "%49[^ = ] = %d", key, &value) == 2) {  // String parsing voor key = value
      if ((strcmp(key, "LED") == 0) && (value == 1 || value == 0)) {  // Aansturing van besturenLED functie
        bestuurLED(redledPin, value);
      }
    }
  }
  client.stop();
}
