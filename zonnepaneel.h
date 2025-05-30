#ifndef ZONNEPANEEL_H
#define ZONNEPANEEL_H
#include "RGB.h"

class Zonnepaneel {
public:
    Zonnepaneel(RGB* rgbLed);
    /**
     * hoofdlogica
     */
    void requestFromSensor();

    /**
     * conversie raw → ampère
     */
    float berekenAmpere(float waarde);
    void verwerkMetingen(float waarde);

private:
    RGB* rgbLed; // why does this contain  RGB
};

#endif
