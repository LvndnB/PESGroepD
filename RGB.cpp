#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


RGB::RGB() {}

/**
 * @param value: min = 0; max= 100
 */
void RGB::SendFromColorspace(int value) {
    // TODO: implement this
}

/**
 * @param r; min 0 max 255
 * @param g; min 0 max 255
 * @param b; min 0 max 255
 */
void RGB::sendToActuator(int r, int g, int b, device_t device) {
    r = (r < 0)? 0: r;
    g = (g < 0)? 0: g;
    b = (b < 0)? 0: b;

    r = (r > 255)? 255: r;
    g = (g > 255)? 255: g;
    b = (b > 255)? 255: b;

    char msg[100];
    sprintf(msg, "rgb=#%02X%02X%02X", r,g,b);


    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}
