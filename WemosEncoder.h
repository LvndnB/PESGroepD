#ifndef WEMOSENCODER_H
#define WEMOSENCODER_H


#include "Wemos.h"
#include "WemosDLEDenBuzzer.h"

class WemosEncoder : public Wemos {
private:
WemosDLEDenBuzzer* dualled;
public:
    WemosEncoder(const std::string& ip, int port, WemosDLEDenBuzzer* led);
    void sendRPM(const std::string& rpm);
    void sendStatus(const std::string& status);
};

#endif
