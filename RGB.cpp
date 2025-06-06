#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int RGB::switchStatus = 0;

RGB::RGB(device_t device) : device(device) {}

/**
 * @param value: min = 0; max= 100
 */
void RGB::SendFromColorspace(int value)
{
    // TODO: implement this
}

void RGB::checkRGBSwitch(device_t switchDevice)
{
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(switchDevice, rgbSwitch);

    if (rapport.rapport.error == 0)
    {
        char key[20];
        char value[40];
        sscanf(rapport.msg.get(), "%[^=]=%s", key, value);

        if (strcmp(key, "rgbSwitch") == 0)
        {

            switchStatus = std::atoi(value);
            printf("Switch status: %d\r\n", switchStatus);
        }
    }


    
}

void RGB::sendColorToActuator(color color)
{
    if (!switchStatus && rgbStatus)
    {
        
    

        char msg[100];
        sprintf(msg, "rgb=#000000");


        procflow bus = procflow("/dev/ttyS0");
        bus.sendDataToDevice(device, msg, strlen(msg));

        rgbStatus = 0;
    }


    if (switchStatus)
    {
        //printf("Sending color to actuator: %d %d %d\r\n", color.r, color.g, color.b);
        color.normalize();

        char msg[100];
        sprintf(msg, "rgb=#%02X%02X%02X", color.r, color.g, color.b);

        if (!rgbStatus)
        {
            rgbStatus = 1;
        }
        
  

        procflow bus = procflow("/dev/ttyS0");
        bus.sendDataToDevice(device, msg, strlen(msg));
    }
}
