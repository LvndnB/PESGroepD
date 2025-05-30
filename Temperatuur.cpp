#include "Temperatuur.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>


Temperatuur::Temperatuur(RGB* rgbtemp) : rgbtemp(rgbtemp) {}


void Temperatuur::requestFromSensor() {
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, temp);

    if (rapport.rapport.error == 0) {
        char key[20];
        char value[40];
        sscanf(rapport.msg.get(), "%s[^=]=%s", key, value);
        if (strcmp(key, "temp") == 0){
              temperatuur = std::atof(value); 
              temperatureNaarRGB(temperatuur);
        }
        
    }

}



void Temperatuur::temperatureNaarRGB(double temperatuur) {
    if (temperatuur < 18.0) temperatuur = 18.0;
    if (temperatuur > 24.0) temperatuur = 24.0;

    // Temp schaal 18-24C naar 2700-6500K
    const int kelvin = 2700 + (24.0 - temperatuur) * 633;

    // Eenvoudige RGB-schatting
    int r = kelvin < 6600 ? 255 : 255 - (kelvin - 6600) / 10;
    int g = kelvin < 6600 ? (kelvin - 2700) * 255 / 3900 : 255;
    int b = kelvin < 5000 ? 0 : (kelvin - 5000) * 255 / 1500;

    // Zorg dat rgb binnen limiet zit
    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;
    
	rgbtemp->sendToActuator(r, g, b, STM1);
}
