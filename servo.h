#ifndef SERVO_H
#define SERVO_H

#include "procflow.h"
#include <sys/time.h>

class Servo
{
    private:
        /**
         * 0 = closed
         * 1 = opened
         */
        int knopStatus = 0;

        /**
         * 0 = closed
         * 1 = opened
         */
        int deurOpen = 0;

        struct timeval laatsteKnopDruk = {0, 0};

    public:
        Servo();
        void checkServoKnop(device_t device);
        void sendToActuator(device_t device, int deurStatus);

        /**
         * First Checks if the button is pressed. If so open the door and changed laatsteKnopDruk to now. 
         *
         * if laatsteKnopDruk is bigger than 3 second. Then close the door.
         *
         */
        void update(device_t device);


};

#endif
