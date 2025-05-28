#ifndef LUCHTKWALITEIT_H
#define LUCHTKWALITEIT_H

#include "RGB.h"
#include "SensorInterface.h"

class Luchtkwaliteit : public SensorInterface
{
private:


public:
    Luchtkwaliteit(RGB* rgbtemp);
    void requestFromSensor();
    void requestFromSensor2();
    void temperatureNaarRGB(double temp);

private:
    double temperatuur;
    int co2value;
    int luchtvochtigheid;
    RGB* rgbtemp;


};


#endif