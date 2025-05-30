#include "Ventilator.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "procflow.h"


Ventilator::Ventilator() {}

/**
 * @param speed: 0 < x < 100
 */
void Ventilator::sendToActuator(device_t device, int speed) {
    char msg[100];
    sprintf(msg, "vent=%d", speed);

    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}
