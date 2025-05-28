#ifndef ACTUATORINTERFACE_H
#define ACTUATORINTERFACE_H

#include "procflow.h"

class ActuatorInterface
{
public:
    virtual ~ActuatorInterface() {};
    virtual void sendToActuator(std::string key, std::string value, device_t device) = 0;
};

#endif