#ifndef RGB_H
#define RGB_H


#include "ActuatorInterface.h"

class RGB : public ActuatorInterface
{
private:


public:
    RGB();
    void sendToActuator(std::string key, std::string value, device_t device) override;
    void makeRGBMessage(int r, int g, int b);


private:

};


#endif