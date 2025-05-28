#include "Luchtkwaliteit.h"
#include <cstdio>
#include <cstdlib>


Luchtkwaliteit::Luchtkwaliteit(Ventilator* ventilator) : ventilator(ventilator) {}


void Luchtkwaliteit::requestFromSensor() {
    int sensorCheck = 0;

    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, temp);

    if (rapport.rapport.error == 0) {
        std::string key;
        std::string value;
        sscanf(rapport.msg.get(), "%s[^=]=%s", key, value);
        if (strcmp(key, "temp") == 0){
            temperatuur = std::atof(value);
            sensorCheck++;
        }
        
    }

    rx_request_response rapport = bus.requestDataFromDevice(STM2, co2);

    if (rapport.rapport.error == 0) {
        std::string key;
        std::string value;
        sscanf(rapport.msg.get(), "%s[^=]=%s", key, value);
        if (strcmp(key, "co2") == 0){
            co2value = std::atoi(value);
            sensorCheck++;
        }
        
    }

    rx_request_response rapport = bus.requestDataFromDevice(STM2, lucht);

    if (rapport.rapport.error == 0) {
        std::string key;
        std::string value;
        sscanf(rapport.msg.get(), "%s[^=]=%s", key, value);
        if (strcmp(key, "lucht") == 0){
            luchtvochtigheid = std::atoi(value);
            sensorCheck++;
        }
        
    }

    if (sensorCheck == 3){
        luchtkwaliteitNaarSpeed();
    }

}

void Luchtkwaliteit::luchtkwaliteitNaarSpeed(){
    float score = 0.0;

    // Normaliseer en bepaal scores tussen 0 en 1
    float tempScore = (temperatuur > 25) ? (temperatuur - 25) / 10.0 : 0;
    if (tempScore > 1) tempScore = 1;

    float co2Score = (float)co2value / 1200.0;  //  max 1200 ppm
    if (co2Score > 1) co2Score = 1;

    float humScore = 0;
    if (luchtvochtingheid < 30)
        humScore = (30 - luchtvochtingheid) / 30.0;
    else if (luchtvochtingheid > 70)
        humScore = (luchtvochtingheid - 70) / 30.0;

    if (humScore > 1) humScore = 1;

    // Combineer gewogen scores
    score = tempScore * 0.3 + co2Score * 0.5 + humScore * 0.2;

    // Schaal naar 0–100
    int snelheid = (int)(score * 100);

    ventilator->makeSpeedMessage(snelheid);


}


