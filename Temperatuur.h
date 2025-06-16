#ifndef TEMPERATUUR_H
#define TEMPERATUUR_H

#include "color.h"
#include "procflow.h"

class Temperatuur
{
public:
    Temperatuur();

    /**
     * request the raw temperature values
     */
    double requestFromSensor();

    /**
     * quality of live function.
     */
    color requestAsColor(double minimum, double maximum);

    /**
     * convert temperature to a color for display
     *
     * white for cold. red for warm
     */
    color temperatureNaarRGB(double value_in, double minimum, double maximum);

};


#endif
