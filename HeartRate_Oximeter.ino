#include "MAX30105.h" // Voor communicatie met de MAX30102 sensor
#include "heartRate.h" // Voor detectie van hartslagen via IR-signalen

MAX30105 particleSensor; // Sensor object

const byte RATE_SIZE = 4; 
byte rates[RATE_SIZE]; // Array voor 4 laatste BPM-metingen (rolling average)
byte rateSpot = 0; // Index voor bovenstaand array
long lastBeat = 0; // Tijdstip van laatste gedetecteerde hartslag
float beatsPerMinute; // Huidige BPM

int perCent, beatAvg, count, avgLoop, noFinger, avgIr;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 kon niet gevonden worden. Check de kabels/power AUB.");
    while (1);
  }
  Serial.println("Plaats je vinger op de sensor met steady pressure.");

//Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 25; //Options: 0=Off to 255=50mA=0xFF hexadecimal. 100=0x64; 50=0x32 25=0x19
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
}

void loop() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) {
        beatAvg += rates[x];
      }
      beatAvg /= RATE_SIZE;
    }
  }
// Controleert de IR-waarde om te bepalen of er een vinger op de sensor ligt.
  if (irValue < 50000) {
    Serial.print(" No finger?");
    Serial.println();
    noFinger++;
  } else {
    count++;
    avgLoop++;
    perCent = map(irValue, 20000, 100000, 0, 100); // schaal IR naar 0–100%
    perCent = constrain(perCent, 0, 100); // zorg dat het tussen 0 en 100 blijft

  }
  //print ongeveer 20x een value uit
  if (count == 25) {
    Serial.print("Oxygen=");
    Serial.print(perCent);
    Serial.print("%");
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.println();
    count = 0;  
  } 
  // print per 20x een average BPM  
  if (avgLoop == 500) {
    Serial.print("Avg BPM=");
    Serial.print(beatAvg);
    Serial.print(" Oxygen=");
    Serial.print(perCent);
    Serial.print("%");
    Serial.println();
    avgLoop = 0;
  }
}