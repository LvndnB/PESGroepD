#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#include "procflow.h"

class SensorInterface
{
public:
    virtual ~SensorInterface() {};
    virtual void requestFromSensor() = 0;
};

#endif