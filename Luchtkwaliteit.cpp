#include "Luchtkwaliteit.h"
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "SocketClient.h"




Luchtkwaliteit::Luchtkwaliteit(Ventilator* ventilator) : ventilator(ventilator) {}


void Luchtkwaliteit::requestFromSensor() {
    int sensorCheck = 0;

    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, temp);

    if (rapport.rapport.error == 0) {
        char key[60];
        char value[60];
        sscanf(rapport.msg.get(), "%[^=]=%s", key, value);
        //printf("Received key: %s, value: %s\n", key, value);
        if (strcmp(key, "temp") == 0){
            temperatuur = atof(value);
            sensorCheck++;
            printf("temp (%d)\n", sensorCheck);

            SocketClient client;
            char message[100];
            sprintf(message, "TEMPLOG=%.1f", temperatuur);
            printf("temp:%s", message);
            client.send("192.168.0.101", 12345, message);

        }
        
        
    }
    usleep(30000);


    rx_request_response co2Request = bus.requestDataFromDevice(STM2, co2);

    if (co2Request.rapport.error == 0) {
        char key[60];
        char value[60];
        sscanf(co2Request.msg.get(), "%[^=]=%s", key, value);
        //printf("Received key: %s, value: %s\n", key, value);
        if (strcmp(key, "co2") == 0){
            co2value = atoi(value);
            sensorCheck++;
            printf("co2 (%d)\n", sensorCheck);

            SocketClient client;
            char message[100];
            sprintf(message, "CO2LOG=%d", co2value);
            printf(message);
            client.send("192.168.0.101", 12345, message);

        }
            
        
    }
    usleep(32000);

    rx_request_response luchtRequest = bus.requestDataFromDevice(STM2, lucht);

    if (luchtRequest.rapport.error == 0) {
        char key[60];
        char value[60];
        sscanf(luchtRequest.msg.get(), "%[^=]=%s", key, value);
        // printf("Received key: %s, value: %s\n", key, value);
        if (strcmp(key, "lucht") == 0){

            luchtvochtigheid = std::atof(value);
            sensorCheck++;
            printf("lucht (%d)\n", sensorCheck);

            
            SocketClient client;
            char message[100];
            sprintf(message, "LUCHTVOCHTIGHEIDLOG=%.1f", luchtvochtigheid); // SAD
            printf(message);
            client.send("192.168.0.101", 12345, message);
        }
    }

    // if (sensorCheck == 3){
        luchtkwaliteitNaarSpeed();
    // }
}

void Luchtkwaliteit::luchtkwaliteitNaarSpeed() {
    printf("Luchtkwaliteit: Temp: %.1f, CO2: %d, Luchtvochtigheid: %.1f\r\n", temperatuur, co2value, luchtvochtigheid);
    float score = 0.0;


    // Normaliseer en bepaal scores tussen 0 en 1
    float tempScore = (temperatuur < 25) ? 0.0f : 
                  (temperatuur > 28) ? 1.0f : 
                  (temperatuur - 25) / 2.0f;
    if (tempScore > 1) tempScore = 1;

    float co2Score = (float)(co2value - 400) / 200.0f; // CO2 waarde normaal is 400 ppm, en we willen dat boven 600 ppm een score van 1 geeft
    if (co2Score > 1) co2Score = 1;

    float humScore = 0;
    if (luchtvochtigheid < 30)
        humScore = (30 - luchtvochtigheid ) / 30.0;
    else if (luchtvochtigheid > 70)
        humScore = (luchtvochtigheid - 70) / 30.0;

    if (humScore > 1) humScore = 1;

    // Combineer gewogen scores
    score = tempScore * 0.3 + co2Score * 0.5 + humScore * 0.2;

    // Schaal naar 0–100
    int snelheid = (int)(score * 100);

    printf("Luchtkwaliteit score: %.2f\r\n", score);

    ventilator->sendToActuator(STM1, snelheid);


}


