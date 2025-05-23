#include <ESP8266WiFi.h>

#define CLK D2  // S1 pin

int positie = 0;
int lastCLK;
unsigned long lastTime = 0;
unsigned long firstTime = 0;
float snelheid = 0;
float rpm = 0;
float avgRPM = 0;
const int positiesPerRotatie = 20;
unsigned long positieTeller = 0;
const unsigned long periode = 5000;

int isMoving = 0;
int rpmreset = 0;
unsigned long laatsteBeweging = 0;

const char* ssid = "Revalidatie";
const char* password = "Revalidatie";
const char* serverIP = "192.168.0.101";
const int serverPort = 12345;

WiFiClient client;

unsigned long lastSendTime = 0;

void setup() {
  pinMode(CLK, INPUT_PULLUP);
  Serial.begin(9600);
  lastCLK = digitalRead(CLK);
  firstTime = millis();
  laatsteBeweging = millis();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
}

void loop() {
  int currentCLK = digitalRead(CLK);

  if (currentCLK != lastCLK && currentCLK == HIGH) {
    unsigned long currentTime = micros();
    unsigned long timeDiff = currentTime - lastTime;

    if (timeDiff > 0) {
      snelheid = 1000000.0 / timeDiff;
    }

    lastTime = currentTime;
    positie++;
    positieTeller++;
    rpm = (snelheid * 60) / positiesPerRotatie;

    unsigned long currentMillis = millis();
    unsigned long elapsedTime = currentMillis - firstTime;

    if (elapsedTime <= periode) {
      avgRPM = (positieTeller * 60.0) / (positiesPerRotatie * (elapsedTime / 1000.0));
    } else {
      firstTime = currentMillis;
      positieTeller = 0;
    }

    Serial.print(" pos/sec | Huidige RPM: ");
    Serial.print(rpm);
    Serial.print(" | Gemiddelde RPM: ");
    Serial.println(avgRPM);

    if (!isMoving) {
      isMoving = 1;
    }

    laatsteBeweging = millis();
    rpmreset = 0;
  }

  // Elke seconde versturen
  if (millis() - lastSendTime >= 1000) {
    lastSendTime = millis();

    // Als er geen beweging is gedurende 1s, stuur eenmalig 0 en zet avgRPM op 0
    if (millis() - laatsteBeweging > 1000) {
      if (!rpmreset) {
        avgRPM = 0;  // Zet avgRPM op 0 bij stilstand
        if (client.connect(serverIP, serverPort)) {
          char buffer[50];
          sprintf(buffer, "ENCODERRPM=0\n");
          client.print(buffer);
          client.stop();
        }
        rpmreset = 1;  // Zorg dat we dit maar één keer doen
      }
    } else {
      // Beweging actief → normale verzending
      if (client.connect(serverIP, serverPort)) {
        char buffer[50];
        sprintf(buffer, "ENCODERRPM=%.2f\n", avgRPM);
        client.print(buffer);
        client.stop();
      }
    }
  }

  lastCLK = currentCLK;
}
