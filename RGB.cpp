#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


RGB::RGB() {}

void RGB::sendToActuator(int r, int g, int b, device_t device) {
    char hexColor[8];
    sprintf(hexColor, "#%02X%02X%02X", r, g, b);
	// Hier zou de code komen om de RGB-waarden naar de hardware te sturen
	// Bijvoorbeeld: bus->send(hexColor);
	// Voor nu is het een placeholder functie


    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, hexColor, strlen(hexColor));
}

