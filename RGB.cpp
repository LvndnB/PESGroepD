#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#inlcude "procflow.h"

RGB::RGB() {}


void RGB::requestTemperature() {
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, temp);

    if (rapport.rapport.error == 0) {
        temperatureNaarRGB(std::atof(rapport.msg.get()));
    }

}

void RGB::temperatureNaarRGB(double temperature) {
    if (temperature < 18.0) temperature = 18.0;
    if (temperature > 24.0) temperature = 24.0;

    // Temp schaal 18–24°C naar 2700–6500K
    int kelvin = 2700 + (24.0 - temperature) * 633;

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
    
	void stuurRGB(int r, int g, int b, int STM = 0); // STM is een placeholder voor juiste stm implementatie
}

void RGB::stuurRGB(int r, int g, int b, int STM) {
    char hexColor[8];
    sprintf(hexColor, "#%02X%02X%02X", r, g, b);
	// Hier zou de code komen om de RGB-waarden naar de hardware te sturen
	// Bijvoorbeeld: bus->send(hexColor);
	// Voor nu is het een placeholder functie
}

