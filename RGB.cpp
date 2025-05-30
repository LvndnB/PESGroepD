#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


RGB::RGB(device_t device):device(device) {}

/**
 * @param value: min = 0; max= 100
 */
void RGB::SendFromColorspace(int value) {
    // TODO: implement this
}

void RGB::sendColorToActuator(color color) {
    color.normalize();

    char msg[100];
    sprintf(msg, "rgb=#%02X%02X%02X", color.r,color.g,color.b);

    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}
