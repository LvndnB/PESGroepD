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
<<<<<<< HEAD
    void requestFromSensor2();
=======
>>>>>>> a7fdca2cacfe10120421fd22347cd5c20142f8f3
    void temperatureNaarRGB(double temp);

private:
    double temperatuur;
<<<<<<< HEAD
    int co2value;
=======
    int co2;
>>>>>>> a7fdca2cacfe10120421fd22347cd5c20142f8f3
    int luchtvochtigheid;
    RGB* rgbtemp;


};


#endif