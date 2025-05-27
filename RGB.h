#ifndef RGB_H
#define RGB_H


#include "ActuatorInterface.h"

class RGB : public ActuatorInterface
{
private:


public:
    RGB();
    void sendToActuator(int r, int g, int b, device_t device);

private:

};


#endif