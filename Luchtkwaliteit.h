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
    double temperatuur;
    int co2value;
    int luchtvochtigheid;
    Ventilator* ventilator;


};


#endif
