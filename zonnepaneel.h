#ifndef ZONNEPANEEL_H
#define ZONNEPANEEL_H
#include "RGB.h"

class Zonnepaneel {
public:
    Zonnepaneel(RGB* rgbLed);

    /**
     * fetch data
     * adc to voltage by bereken Voltage
     * convert to color
     * send color to this->rgbLed
     */
    void update();

    float berekenVoltage(float waarde);
    void verwerkMetingen(float waarde);

private:
    RGB* rgbLed; // why does this contain  RGB? Omdat de omgerekende waardes van de zonnepaneel ook een lampje aan moeten sturen en daarbij al gebruik maakt van de al bestaande RGB-klasse

};

#endif
