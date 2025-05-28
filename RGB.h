#ifndef RGB_H
#define RGB_H


#include "ActuatorInterface.h"

class RGB : public ActuatorInterface
{
private:


public:
    RGB();
<<<<<<< HEAD
    void sendToActuator(int r, int g, int b, device_t device) override;
=======
    void sendToActuator(int r, int g, int b, device_t device);
>>>>>>> a7fdca2cacfe10120421fd22347cd5c20142f8f3

private:

};


#endif