#include "WemosEncoder.h"
#include <cstdlib>
#include "WemosDLEDenBuzzer.h"

WemosEncoder::WemosEncoder(const std::string& ip, int port, WemosDLEDenBuzzer* led, Logger* encoderLog) : Wemos(ip, port), dualled(led), encoderLog(encoderLog) {}


void WemosEncoder::handleRPM(const std::string& rpm) {
    encoderLog->log("Encoder RPM: " + rpm);

    float rpmFloat = std::stof(rpm);
    
    if (rpmFloat == 0.0f && dualled->isAan("GROEN")) {
        dualled->uit("GROEN");
    } else if (rpmFloat > 0.0f && !dualled->isAan("GROEN")) {
        dualled->aan("GROEN");
    }


    
}