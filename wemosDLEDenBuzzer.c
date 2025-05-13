#include <ESP8266WiFi.h>

const char* ssid = "Revalidatie";
const char* password = "Revalidatie";
const int serverPort = 12345;
const int roodledPin = D8;
const int groenledPin = D7;
const int buzzerPin = D2;

WiFiServer server(serverPort);
char key[50];
int value = 0;


void setup() {
  Serial.begin(9600);
  pinMode(roodledPin, OUTPUT);
  pinMode(groenledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  server.begin();
  Serial.println(WiFi.localIP());
}


void aansturingGroen(int status) {
  if (status == 1) {
    digitalWrite(groenledPin, 1);
    digitalWrite(roodledPin, 0);
  } else {
    digitalWrite(groenledPin, 0);
  }
}

void aansturingRood(int status) {
  if (status == 1) {
    digitalWrite(roodledPin, 1);
    digitalWrite(groenledPin, 0);
  } else {
    digitalWrite(roodledPin, 0);
  }
}

void aansturingBuzzer(int status) {
  if (status == 1) {
    tone(buzzerPin, 500);  // Start toon op 1000 Hz
  } else {
    noTone(buzzerPin);  // Stop de toon
  }
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String input = client.readStringUntil('\n');
    input.trim();


    if (sscanf(input.c_str(), "%49[^ = ] = %d", key, &value) == 2) {  // String parsing voor key = value
      Serial.println(key);
      //value = 0;

      if ((strcmp(key, "DUALLEDROOD") == 0)) {
        aansturingRood(value);
      } else if (strcmp(key, "DUALLEDGROEN") == 0) {
        aansturingGroen(value);
      } else if (strcmp(key, "BUZZER") == 0) {
        aansturingBuzzer(value);
      }
    }
  }
  client.stop();
}
