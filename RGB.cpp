#include "RGB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int RGB::schakelaarStatus = 0;

RGB::RGB(device_t device) : device(device) {}


void RGB::checkRGBSchakelaar(device_t schakelaarDevice)
{
    procflow bus = procflow("/dev/ttyS0");

    rx_request_response rapport = bus.requestDataFromDevice(schakelaarDevice, deurKnop);

    if (rapport.rapport.error == 0)
    {
        char key[20];
        char value[40];
        sscanf(rapport.msg.get(), "%[^=]=%s", key, value);

        if (strcmp(key, "deurKnop") == 0)//moet veranderd worden naar rgbSchakelaar
        {

            schakelaarStatus = std::atoi(value);
        }
    }

    printf("done rx with return %d\r\n\r\n", rapport.rapport.error);
    for (int i = 0; i < rapport.rapport.recieved_bytes; i++)
    {
        printf("msg[%d]: %c\r\n", i, rapport.msg.get()[i]);
    }

    if (!schakelaarStatus)
    {
        
        color color(0, 0, 0);
        color.normalize();

        char msg[100];
        sprintf(msg, "rgb=#%02X%02X%02X", color.r, color.g, color.b);


        procflow bus = procflow("/dev/ttyS0");
        bus.sendDataToDevice(device, msg, strlen(msg));

        printf("%s\n", msg);
    }
}

void RGB::sendColorToActuator(color color)
{
    if (schakelaarStatus)
    {
        printf("Sending color to actuator: %d %d %d\r\n", color.r, color.g, color.b);
        color.normalize();

        char msg[100];
        sprintf(msg, "rgb=#%02X%02X%02X", color.r, color.g, color.b);

        procflow bus = procflow("/dev/ttyS0");
        bus.sendDataToDevice(device, msg, strlen(msg));

        printf("%s\n", msg);
    }
}
