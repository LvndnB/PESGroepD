#ifndef RGB_H
#define RGB_H

#include "procflow.h"
#include "color.h"

class RGB
{
public:
    RGB(device_t device);
    void sendColorToActuator(color color);
    void SendFromColorspace(int value);

private:
    device_t device;

};


#endif
