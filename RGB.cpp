#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


RGB::RGB() {}

void RGB::sendToActuator(int r, int g, int b, device_t device) {
    char hexColor[12];
    sprintf(hexColor, "rgb=#%02X%02X%02X", r, g, b);




    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, hexColor, strlen(hexColor));
}

