#ifndef WEMOSPULSEOXY_H
#define WEMOSPULSEOXY_H

#include "Wemos.h"
#include "WemosDLEDenBuzzer.h"
#include "Logger.h"

class WemosPulseOxy : public Wemos {
    private:
    WemosDLEDenBuzzer* buzzer;
    WemosDLEDenBuzzer* dualled;
    Logger* pulseoxyLog;

    int pulse;
    int alarm = 0;
    int pulsegrenswaarde = 90;

    public:
    WemosPulseOxy(const std::string&ip, int port, WemosDLEDenBuzzer* buz, WemosDLEDenBuzzer* led, Logger* pulseoxyLog);
    void checkOxy(const std::string& oxy);
    void checkPulse(const std::string& pulse);

};

#endif