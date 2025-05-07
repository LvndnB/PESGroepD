#ifndef WEMOSENCODER_H
#define WEMOSENCODER_H


#include "Wemos.h"
#include "WemosDualLED.h"

class WemosEncoder : public Wemos {
private:
    WemosDualLED* dualled;
public:
    WemosEncoder(const std::string& ip, int port, WemosDualLED* led);
    void sendRPM(const std::string& rpm);
    void sendStatus(const std::string& status);
};

#endif