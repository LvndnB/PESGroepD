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
    void checkRGBSwitch(device_t switchDevice);

private:
    static int switchStatus;
    device_t device;

};


#endif
