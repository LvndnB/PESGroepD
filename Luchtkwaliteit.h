#ifndef LUCHTKWALITEIT_H
#define LUCHTKWALITEIT_H

#include "Ventilator.h"
#include "SensorInterface.h"

class Luchtkwaliteit : public SensorInterface
{
private:


public:
    Luchtkwaliteit(Ventilator* ventilator);
    void requestFromSensor() override;
    void luchtkwaliteitNaarSpeed();

private:
    double temperatuur;
    int co2value;
    int luchtvochtigheid;
    Ventilator* ventilator;


};


#endif