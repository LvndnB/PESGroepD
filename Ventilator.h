#ifndef VENTILATOR_H
#define VENTILATOR_H

#include "procflow.h"

class Ventilator 
{
private:


public:
    Ventilator();

    void sendToActuator(device_t device, int speed);

private:

};


#endif
