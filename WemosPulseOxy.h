#ifndef WEMOSPULSEOXY_H
#define WEMOSPULSEOXY_H

#include "Wemos.h"
#include "WemosDLEDenBuzzer.h"

class WemosPulseOxy : public Wemos {
    private:
    WemosDLEDenBuzzer* buzzer;
    WemosDLEDenBuzzer* dualled;

    int pulse;
    int alarm = 0;

    public:
    WemosPulseOxy(const std::string&ip, int port, WemosDLEDenBuzzer* buz, WemosDLEDenBuzzer* led);
    void checkPulse(const std::string& pulse);

};

#endif
