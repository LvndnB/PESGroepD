#ifndef ZONNEPANEEL_H
#define ZONNEPANEEL_H
#include "RGB.h"

class Zonnepaneel {
public:
    Zonnepaneel(RGB* rgbLed);

    void requestFromSensor();

    float berekenVoltage(float waarde);
    void verwerkMetingen(float waarde);

private:
    RGB* rgbLed; // why does this contain  RGB? Omdat de omgerekende waardes van de zonnepaneel ook een lampje aan moeten sturen en daarbij al gebruik maakt van de al bestaande RGB-klasse
};

#endif
