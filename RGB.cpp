#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int RGB::switchStatus = 0;

RGB::RGB(device_t device) : device(device) {}

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
    printf(switchStatus ? "Switch is on\r\n" : "Switch is off\r\n");
    printf("RGB status: %d\r\n", rgbStatus);
    if (!switchStatus && rgbStatus)
    {
        
    

        char msg[100];
        sprintf(msg, "rgb=#000000");
        printf("Switch is off, sending color off message: %s\r\n", msg);



        procflow bus = procflow("/dev/ttyS0");
        bus.sendDataToDevice(device, msg, strlen(msg));

        rgbStatus = 0;
    }


    if (switchStatus)
    {
        color.clamp();

        char msg[100];
        sprintf(msg, "rgb=#%02X%02X%02X", color.r, color.g, color.b);
        printf("Sending color message: %s\r\n", msg);

        if (!rgbStatus)
        {
            rgbStatus = 1;
        }

        procflow bus = procflow("/dev/ttyS0");
        bus.sendDataToDevice(device, msg, strlen(msg));
    }
}
