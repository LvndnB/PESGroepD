#ifndef RGB_H
#define RGB_H

#include "procflow.h"
#include "color.h"

class RGB
{
public:
    RGB(device_t device);

    /**
     * This clamps and transmit the color over the uartbus.
     * Warning if `swichstatus
     */
    void sendColorToActuator(color color);

    /**
     *  Dit stuurt een bericht naar switchDevice om de staat van de swich te vragen. En 
     */
    void checkRGBSwitch(device_t switchDevice);

private:

    /**
     * 1 = on
     * 0 = off 
     */
    static int switchStatus;

    /**
     * 1 = on
     * 0 = off 
     */
    int rgbStatus = 0; 

    /**
     * The device id the led is on.
     */
    device_t device;
};


#endif
