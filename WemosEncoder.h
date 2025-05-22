#ifndef WEMOSENCODER_H
#define WEMOSENCODER_H


#include "Wemos.h"
#include "WemosDLEDenBuzzer.h"
#include "Logger.h"

class WemosEncoder : public Wemos {
private:
WemosDLEDenBuzzer* dualled;
Logger* encoderLog;
public:
    WemosEncoder(const std::string& ip, int port, WemosDLEDenBuzzer* led, Logger* encoderLog);
    void handleRPM(const std::string& rpm);
};

#endif