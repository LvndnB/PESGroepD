#ifndef KLOCK_h
#define klock_H

#include "procflow.h"

class Zevensegmentendisplay 
{
public:
    Zevensegmentendisplay(device_t device);
    /**
     * @param value: The number to be displayed
     * @param colum: if : is shown
     */
    void sendint(int value, bool colum);

    /**
     * displays floats in the following format "40:32" 
     */
    void sendfloat(float value);

    /**
     * automatically populating sendint values with current value and show :
     */
    void updateToCurrentTime();

private:
    device_t device;

};


#endif
