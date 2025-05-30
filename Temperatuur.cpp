#include "Temperatuur.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>



Temperatuur::Temperatuur() {}


double Temperatuur::requestFromSensor() {
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(STM2, temp);

    if (rapport.rapport.error == 0) {
        char key[20];
        char value[40];
        sscanf(rapport.msg.get(), "%s[^=]=%s", key, value);
        if (strcmp(key, "temp") == 0){
              return std::atof(value); 
        }
        
    }
    return -200; // error

}


color Temperatuur::requestAsColor(double minimum, double maximum) {
    return temperatureNaarRGB(requestFromSensor(), minimum, maximum);
}


color Temperatuur::temperatureNaarRGB(double value_in, double minimum, double maximum) {
    if (value_in < minimum) value_in = minimum;
    if (value_in > maximum) value_in = maximum;

    // Temp schaal 18-24C naar 2700-6500K
    const int kelvin = 2700 + (24.0 - value_in) * 633;

    // Eenvoudige RGB-schatting
    // TODO: check if this formula holds width differed ranges. Prob not
    int r = kelvin < 6600 ? 255 : 255 - (kelvin - 6600) / 10;
    int g = kelvin < 6600 ? (kelvin - 2700) * 255 / 3900 : 255;
    int b = 5000 < kelvin ?  (kelvin - 5000) * 255 / 1500 : 0;

     color c(r,g,b);
     c.normalize();
     return c;
}
