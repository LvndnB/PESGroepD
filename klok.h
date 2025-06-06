#ifndef KLOCK_h
#define klock_H

#include "procflow.h"

class Zevensegmentendisplay 
{
public:
    Zevensegmentendisplay(device_t device);
    void sendint(int value, bool colum);
    void sendfloat(float value);
    void updateToCurrentTime();

private:
    device_t device;

};


#endif
