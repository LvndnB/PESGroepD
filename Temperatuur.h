#ifndef TEMPERATUUR_H
#define TEMPERATUUR_H

#include "RGB.h"

class Temperatuur
{
public:
    Temperatuur(RGB* rgbtemp);
    void requestFromSensor();

    void temperatureNaarRGB(double temp);

private:
    double temperatuur;
    RGB* rgbtemp;


};


#endif
