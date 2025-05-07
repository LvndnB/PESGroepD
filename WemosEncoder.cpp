#include "WemosEncoder.h"
#include <cstdlib>
#include "WemosDualLED.h"

WemosEncoder::WemosEncoder(const std::string& ip, int port, WemosDualLED* led) : Wemos(ip, port), dualled(led) {}


void WemosEncoder::sendRPM(const std::string& rpm) {
    
}

void WemosEncoder::sendStatus(const std::string& status) {
    if (status == "1") {
        dualled->aan("GROEN");
    } else {
        dualled->uit("GROEN");
    }
    
}