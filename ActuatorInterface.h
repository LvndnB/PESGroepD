#ifndef ACTUATORINTERFACE_H
#define ACTUATORINTERFACE_H

#include "procflow.h"

class ActuatorInterface
{
public:
    virtual ~ActuatorInterface() {};
    virtual void sendToActuator(int r, int g, int b, device_t device) = 0;
};

#endif