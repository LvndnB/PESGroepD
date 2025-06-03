#ifndef SERVO_H
#define SERVO_H

#include "procflow.h"
#include <sys/time.h>

class Servo
{
    private:
    int knopStatus = 0;
    int deurOpen = 0;
    struct timeval laatsteKnopDruk = {0, 0};

    public:
    Servo();
    void checkServoKnop(device_t device);
    void sendToActuator(device_t device, int deurStatus);
    void update(device_t device);


};

#endif