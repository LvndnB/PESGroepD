#include "WemosDualLED.h"
#include <iostream>

WemosDualLED::WemosDualLED(const std::string& ip, int port) : Wemos(ip, port) {}

void WemosDualLED::aan(const std::string& kleur) {
    makeSendMessage("DUALLED" + kleur, "1");
    std::cout << "Verzonden naar WemosLED: DUALLEDGROEN=1" << "\n";
}

void WemosDualLED::uit(const std::string& kleur) {
    makeSendMessage("DUALLED" + kleur, "0");
    std::cout << "Verzonden naar WemosLED: DUALLEDGROEN=0" << "\n";

}