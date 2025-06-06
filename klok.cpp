#include "klok.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


Zevensegmentendisplay::Zevensegmentendisplay(device_t device) : device(device) {}

void Zevensegmentendisplay::sendint(int value, bool colum)
{
    char msg[100];
    sprintf(msg, "tim=%c%d", colum?'|':':', value );

    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}

void Zevensegmentendisplay::sendfloat(float value)
{
    char msg[100];
    sprintf(msg, "tim=f%f", value );

    procflow bus = procflow("/dev/ttyS0");
    bus.sendDataToDevice(device, msg, strlen(msg));
}

void Zevensegmentendisplay::updateToCurrentTime() {
    time_t t = time(NULL);
    struct tm localtime = {0};
    localtime_r(&t, &localtime);

    int time = localtime.tm_hour*100 + localtime.tm_min;

    this->sendint(time, true);
}