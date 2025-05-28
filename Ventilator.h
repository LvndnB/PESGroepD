#ifndef VENTILATOR_H
#define VENTILATOR_H


#include "ActuatorInterface.h"

class Ventilator : public ActuatorInterface
{
private:


public:
    Ventilator();
    void sendToActuator(std::string key, std::string value, device_t device) override;
    void makeSpeedMessage(int speed);

private:

};


#endif