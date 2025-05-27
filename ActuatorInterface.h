#ifndef ACTUATORINTERFACE_H
#define ACTUATORINTERFACE_H

#include "procflow.h"

class ActuatorInterface
{
public:
    virtual ~ActuatorInterface() {};
    virtual void sendToActuator() = 0;
};

#endif