#include "servo.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

Servo::Servo() {
    sendToActuator(STM3, 0); // Initialiseer deur als gesloten
}

void Servo::checkServoKnop(device_t device) {
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(device, deurKnop);

    if (rapport.rapport.error == 0) {
        char key[20];
        char value[40];
        sscanf(rapport.msg.get(), "%[^=]=%s", key, value);


        if (strcmp(key, "deurKnop") == 0){

            
              knopStatus = std::atoi(value);
        }
    }

    printf("done rx with return %d\r\n\r\n", rapport.rapport.error);
    for (int i = 0; i < rapport.rapport.recieved_bytes; i++) {
        printf("msg[%d]: %c\r\n",i, rapport.msg.get()[i]);
    }



}

void Servo::sendToActuator(device_t device, int deurStatus) {
    char msg[100];
    sprintf(msg, "deur=%d", deurStatus);

    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}


void Servo::update(device_t device) {
    checkServoKnop(device);


    if (knopStatus) {
        gettimeofday(&laatsteKnopDruk, NULL);

        if (!deurOpen) {
            sendToActuator(device, 1);  // Deur openen
            deurOpen = 1;
        }


    }

    knopStatus = 0;  // Reset knop status na verwerking

    if (deurOpen) {
        struct timeval huidigeTijd;
        gettimeofday(&huidigeTijd, NULL);
        double verschil = huidigeTijd.tv_sec - laatsteKnopDruk.tv_sec;


        if (verschil >= 3.0) {
            sendToActuator(device, 0);  // Deur sluiten
            deurOpen = 0;
        }
    
    }
}