#ifndef RGB_H
#define RGB_H

#include "procflow.h"
#include "color.h"

class RGB
{
public:
    RGB(device_t device);
    void sendColorToActuator(color color);
    void checkRGBSwitch(device_t switchDevice);

private:
    static int switchStatus;
    int rgbStatus = 0; // 0 = off, 1 = on
    device_t device;

};


#endif
