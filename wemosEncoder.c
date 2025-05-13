#include <ESP8266WiFi.h>

#define CLK D2  // S1 pin

int positie = 0;
int lastCLK;
unsigned long lastTime = 0;
unsigned long firstTime = 0;         // Eerste tijd in de periode
float snelheid = 0;                  // pulsen per seconde
float rpm = 0;                       // Huidige RPM
float avgRPM = 0;                    // Gemiddelde RPM over de laatste periode
const int positiesPerRotatie = 20;   // Aantal posities per volledige rotatie
unsigned long positieTeller = 0;     // Telt het aantal posities in de laatste periode
unsigned long lastRPMTime = 0;       // Tijdstip van laatste RPM berekening
const unsigned long periode = 5000;  // 5 seconden in milliseconden

int isMoving = 0;
int rpmreset = 0;
unsigned long laatsteBeweging = 0;

const char* ssid = "Revalidatie";
const char* password = "Revalidatie";
const char* serverIP = "192.168.0.101";  // IP van de Raspberry Pi
const int serverPort = 12345;

WiFiClient client;

void setup() {
  pinMode(CLK, INPUT_PULLUP);
  Serial.begin(9600);
  lastCLK = digitalRead(CLK);
  firstTime = millis();        // Stel de starttijd in voor de periode
  laatsteBeweging = millis();  // Tijd sinds de laatste beweging

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
}

void loop() {
  int currentCLK = digitalRead(CLK);

  // Detecteer alleen de randen van de CLK signaal (rising edge)
  if (currentCLK != lastCLK && currentCLK == HIGH) {  // Rising edge (van LOW naar HIGH)
    unsigned long currentTime = micros();             // Tijd in microseconden
    unsigned long timeDiff = currentTime - lastTime;

    // Snelheidsberekening (pulsen per seconde)
    if (timeDiff > 0) {
      snelheid = 1000000.0 / timeDiff;  // Posities per seconde
    }

    lastTime = currentTime;  // Update de laatste tijd

    // Bijwerken van de positie
    positie++;
    positieTeller++;  // Tel het aantal posities voor het gemiddelde RPM

    // Bereken RPM (posities per minuut gedeeld door 20 voor rotaties)
    rpm = (snelheid * 60) / positiesPerRotatie;

    // Bijwerken van de gemiddelde RPM
    unsigned long currentMillis = millis();
    unsigned long elapsedTime = currentMillis - firstTime;

    // Als we binnen de laatste periode zitten, rekent het gemiddelde uit
    if (elapsedTime <= periode) {
      avgRPM = (positieTeller * 60.0) / (positiesPerRotatie * (elapsedTime / 1000.0));  // Gemiddelde RPM over de laatste periode
    } else {
      // Na 10 seconden resetten
      firstTime = currentMillis;
      positieTeller = 0;
    }

    // Output naar Serial Monitor
    Serial.print("Positie: ");
    Serial.print(positie);
    Serial.print(" | Snelheid: ");
    Serial.print(snelheid);
    Serial.print(" pos/sec | Huidige RPM: ");
    Serial.print(rpm);
    Serial.print(" | Gemiddelde RPM: ");
    Serial.println(avgRPM);

    // Controleer of de encoder net is gestart met bewegen
    if (!isMoving) {
      isMoving = 1;
      if (client.connect(serverIP, serverPort)) {
        char buffer[50];
        sprintf(buffer, "ENCODER_STATUS=1\n");
        client.print(buffer);
        client.stop();
        Serial.println("Status=1");
      }
    }

    laatsteBeweging = millis();  // Reset de stilstandtimer
    rpmreset = 0;


    if (client.connect(serverIP, serverPort)) {  // Connect met Server: RPiA
      char buffer[50];                           // Buffer voor de geformatteerde string
      sprintf(buffer, "ENCODERRPM=%.2f\n", avgRPM);
      client.print(buffer);
      client.stop();
    }
  }

  if (isMoving && !rpmreset && (millis() - laatsteBeweging > 1000)) {
    if (client.connect(serverIP, serverPort)) {
      char buffer[50];
      sprintf(buffer, "ENCODERRPM=0\n");
      client.print(buffer);
      client.stop();
    }
    rpmreset = 1;
  }

  // Controleer of de encoder 5 seconden niet heeft bewogen
  if (isMoving && (millis() - laatsteBeweging > 5000)) {
    isMoving = 0;
    if (client.connect(serverIP, serverPort)) {
      char buffer[50];
      sprintf(buffer, "ENCODER_STATUS=0\n");
      client.print(buffer);
      client.stop();
      Serial.println("Status=0");
    }
    
  }

  

  

  lastCLK = currentCLK;  // Update de vorige CLK waarde
}
