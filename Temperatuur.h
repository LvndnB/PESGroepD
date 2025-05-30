#ifndef TEMPERATUUR_H
#define TEMPERATUUR_H

#include "color.h"
#include "procflow.h"

class Temperatuur
{
public:
    Temperatuur();
    double requestFromSensor();
    color requestAsColor(double minimum, double maximum);
    color temperatureNaarRGB(double value_in, double minimum, double maximum);

};


#endif
