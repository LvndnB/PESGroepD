#include "WemosPulseOxy.h"
#include <cstdlib>
#include "WemosDLEDenBuzzer.h"
#include <string>
#include <iostream>


WemosPulseOxy::WemosPulseOxy(const std::string& ip, int port, WemosDLEDenBuzzer* buz, WemosDLEDenBuzzer* led) : Wemos(ip, port), buzzer(buz), dualled(led) {}

void WemosPulseOxy::checkPulse(const std::string& pulseString){
    pulse = std::stoi(pulseString);

    if ((pulse > 90) && (alarm == 0)) {
        buzzer->buzzer(1);
        dualled->aan("ROOD");
        alarm = 1;

    } else if((alarm == 1) && (pulse < 91) ) {
        buzzer->buzzer(0);
        dualled->uit("ROOD");
        alarm = 0;
    }

}
