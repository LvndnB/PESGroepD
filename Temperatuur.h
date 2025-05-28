#ifndef TEMPERATUUR_H
#define TEMPERATUUR_H

#include "RGB.h"
#include "SensorInterface.h"

class Temperatuur : public SensorInterface
{
private:


public:
    Luchtkwaliteit(RGB* rgbtemp);
    void requestFromSensor() override;

    void temperatureNaarRGB(double temp);

private:
    double temperatuur;
    RGB* rgbtemp;


};


#endif