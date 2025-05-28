#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


RGB::RGB() {}

void RGB::sendToActuator(std::string key, std::string value, device_t device) {
    char msg[100];
    sprintf(msg, "%s=%s", key, value);


    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}

void RGB::makeRGBMessage(int r, int g, int b){
    char hexColor[8];
    sprintf(hexColor, "#%02X%02X%02X", r, g, b);

    sendToActuator("rgb", hexColor, STM1);

}
