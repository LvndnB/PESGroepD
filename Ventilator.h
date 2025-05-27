#ifndef VENTILATOR_H
#define VENTILATOR_H


#include "ActuatorInterface.h"

class Ventilator : public ActuatorInterface
{
private:


public:
    Ventilator();
    void sendToActuator(int speed, device_t device);

private:

};


#endif