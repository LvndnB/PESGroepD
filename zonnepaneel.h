#ifndef ZONNEPANEEL_H
#define ZONNEPANEEL_H
#include "SensorInterface.h"
#include "RGB.h"

class Zonnepaneel : public SensorInterface {
public:
    Zonnepaneel(RGB* rgbLed);
    void requestFromSensor();  // hoofdlogica
    float berekenAmpere(float waarde);   // conversie raw → ampère
    void verwerkMetingen(float waarde);

private:
    RGB* rgbLed;
};

#endif
