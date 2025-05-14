#include <ESP8266WiFi.h>
#include "MAX30105.h"       // Voor communicatie met de MAX30102 sensor
#include "heartRate.h"      // Voor detectie van hartslagen via IR-signalen

MAX30105 particleSensor;

const byte RATE_SIZE = 10;
float rates[RATE_SIZE];       // Float array voor BPM-metingen
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;

int perCent, count = 0;

// Interval voor gemiddelde BPM-logging
unsigned long vorigePrintTijd = 0;
const unsigned long printInterval = 5000;  // 5 seconden
bool fingerDetected = false;
bool fingerWasPresent = true;

// WiFi-configuratie
const char* ssid = "Revalidatie";
const char* password = "Revalidatie";
const char* serverIP = "192.168.0.101";    // IP van Raspberry Pi
const int serverPort = 12345;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 kon niet gevonden worden. Check de kabels/power AUB.");
    while (1);
  }

  Serial.println("Plaats je vinger op de sensor met steady pressure.");

  // Sensorinstellingen
  byte ledBrightness = 25;
  byte sampleAverage = 4;
  byte ledMode = 2;
  int sampleRate = 400;
  int pulseWidth = 411;
  int adcRange = 2048;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  particleSensor.setPulseAmplitudeRed(0x0A);  // Laag niveau voor indicatie

  // WiFi-verbinding
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Verbonden met WiFi. IP-adres: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  long irValue = particleSensor.getIR();

  // Hartslagdetectie
  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) {
        beatAvg += rates[x];
      }
      beatAvg /= RATE_SIZE;
    }
  }

  count++;
  perCent = map(irValue, 20000, 100000, 0, 100);
  perCent = constrain(perCent, 0, 100);

  fingerDetected = (irValue >= 50000);

  // Elke 5 seconden uitvoeren
  if (millis() - vorigePrintTijd >= printInterval) {
    vorigePrintTijd = millis();

    if (fingerDetected && beatAvg > 0) {
      Serial.print("Gemiddelde BPM: ");
      Serial.print(beatAvg);
      Serial.print(" | Oxygen: ");
      Serial.print(perCent);
      Serial.println("%");

      if (client.connect(serverIP, serverPort)) {
        char buffer[50];
        sprintf(buffer, "HARTSLAGAVG=%d\n", beatAvg);
        client.print(buffer);
        client.stop();
      }

    } else if (!fingerDetected && fingerWasPresent) {
      Serial.println("Geen vinger op de sensor");

      if (client.connect(serverIP, serverPort)) {
        char buffer[50];
        sprintf(buffer, "HARTSLAGAVG=0\n");
        client.print(buffer);
        client.stop();
      }
    }

    fingerWasPresent = fingerDetected;
  }
}
