#ifndef ACTUATORINTERFACE_H
#define ACTUATORINTERFACE_H

#include "procflow.h"

class ActuatorInterface
{
public:
    virtual ~ActuatorInterface() {};
<<<<<<< HEAD
    virtual void sendToActuator(int r, int g, int b, device_t device) = 0;
=======
    virtual void sendToActuator() = 0;
>>>>>>> a7fdca2cacfe10120421fd22347cd5c20142f8f3
};

#endif