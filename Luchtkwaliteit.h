#ifndef LUCHTKWALITEIT_H
#define LUCHTKWALITEIT_H

#include "Ventilator.h"

class Luchtkwaliteit
{
private:


public:
    Luchtkwaliteit(Ventilator* ventilator);

    /**
     * Deze functie vraagt de temperatuur, luchtvochtigheid en co2 waarde en stuurt
     * die informatie automatisch naar luchtkwaliteitNaarSpeed gestuurd.
     * 
     * Ook stuurt deze functie alle gemeten waardes naar de log 
     */
    void requestFromSensor();

    /**
     * Deze functie berekent met de hand van de gemeten waardes de snelheid van de ventilator.
     * Wanneer ie klaar is met berekenen verstuurd het de snelheid naar  Ventilator*
     */
    void luchtkwaliteitNaarSpeed();

private:
    float temperatuur;
    int co2value;
    float luchtvochtigheid;
    Ventilator* ventilator;


};


#endif
