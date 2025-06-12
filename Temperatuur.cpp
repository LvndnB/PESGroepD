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
        sscanf(rapport.msg.get(), "%[^=]=%s", key, value);
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
    // Clamp waarde binnen bereik
    if (value_in < minimum) value_in = minimum;
    if (value_in > maximum) value_in = maximum;

    double percentage = (value_in - minimum) / (maximum - minimum);
    printf("tempvalues: %f (min: %f, max %f, %% %f)\r\n", value_in, minimum, maximum, percentage);

    // Interpoleer Kelvin omgedraaid: laagste waarde is warm (rood), hoogste is koel (wit)
    const int kelvinMin = 2700; // warm roodachtig
    const int kelvinMax = 6500; // koel wit
    const int kelvin = kelvinMin + (1.0 - percentage) * (kelvinMax - kelvinMin); // omgekeerd!

    // Bestaande RGB-schatting
    int r = kelvin < 6600 ? 255 : std::max(0, 255 - (kelvin - 6600) / 10);
    int g = kelvin < 6600 ? (kelvin - kelvinMin) * 255 / (6600 - kelvinMin) : 255;
    int b = kelvin > 5000 ? std::min(255, (kelvin - 5000) * 255 / 1500) : 0;

    color c(r, g, b);
    c.normalize();
    return c;
}

