#include "Ventilator.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


Ventilator::Ventilator() {}

void Ventilator::sendToActuator(int speed, device_t device) {
    char speedstring[8];
    sprintf(speedstring, "%d", speed);

    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, speedstring, strlen(speedstring));
}

