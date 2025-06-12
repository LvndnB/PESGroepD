#ifndef LUCHTKWALITEIT_H
#define LUCHTKWALITEIT_H

#include "Ventilator.h"

class Luchtkwaliteit
{
private:


public:
    Luchtkwaliteit(Ventilator* ventilator);
    void requestFromSensor();
    void luchtkwaliteitNaarSpeed();

private:
    float temperatuur;
    int co2value;
    float luchtvochtigheid;
    Ventilator* ventilator;


};


#endif
