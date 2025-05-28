#include "Ventilator.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


Ventilator::Ventilator() {}

void Ventilator::sendToActuator(std::string key, std::string value, device_t device) {
    char msg[100];
    sprintf(msg, "%s=%s", key, value);


    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}

void Ventilator::makeSpeedMessage(int speed) {
    char speedstring[4];
    sprintf(speedstring, "%d", speed);

    sendToActuator("ventilator", speedstring, STM1);
}

