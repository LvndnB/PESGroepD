#include "Luchtkwaliteit.h"
#include <cstdio>
#include <cstdlib>


Luchtkwaliteit::Luchtkwaliteit(RGB* rgbtemp) : rgbtemp(rgbtemp) {}


void Luchtkwaliteit::requestFromSensor() {
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, temp);

    if (rapport.rapport.error == 0) {
<<<<<<< HEAD
        temperatureNaarRGB(std::atof(rapport.msg.get())); // Hier moet nog parsing voor key=value
    }

}

void Luchtkwaliteit::requestFromSensor2() {
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, co2);

    if (rapport.rapport.error == 0) {
        //temperatureNaarRGB(std::atof(rapport.msg.get()));
=======
        temperatureNaarRGB(std::atof(rapport.msg.get()));
>>>>>>> a7fdca2cacfe10120421fd22347cd5c20142f8f3
    }

}


void Luchtkwaliteit::temperatureNaarRGB(double temperature) {
    if (temperature < 18.0) temperature = 18.0;
    if (temperature > 24.0) temperature = 24.0;

    // Temp schaal 18�24�C naar 2700�6500K
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
    
	rgbtemp->sendToActuator(r, g, b, STM1);
}