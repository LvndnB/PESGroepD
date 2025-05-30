#ifndef RGB_H
#define RGB_H

#include "procflow.h"
#include <string>

class RGB
{
public:
    RGB();
    void sendToActuator(int r, int g, int b, device_t device);
    void SendFromColorspace(int value);

private:

};


#endif
