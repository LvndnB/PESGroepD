#include "WemosDLEDenBuzzer.h"
#include <iostream>

WemosDLEDenBuzzer::WemosDLEDenBuzzer(const std::string& ip, int port) : Wemos(ip, port) {}

void WemosDLEDenBuzzer::aan(const std::string& kleur) {
    makeSendMessage("DUALLED" + kleur, "1");
    std::cout << "Verzonden naar WemosLED: DUALLEDGROEN=1" << "\n";
}

void WemosDLEDenBuzzer::uit(const std::string& kleur) {
    makeSendMessage("DUALLED" + kleur, "0");
    std::cout << "Verzonden naar WemosLED: DUALLEDGROEN=0" << "\n";

}

void WemosDLEDenBuzzer::buzzer(int status) {
    makeSendMessage("BUZZER", std::to_string(status));
}
