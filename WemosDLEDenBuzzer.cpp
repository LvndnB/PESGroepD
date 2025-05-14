#include "WemosDLEDenBuzzer.h"
#include <iostream>

WemosDLEDenBuzzer::WemosDLEDenBuzzer(const std::string& ip, int port) : Wemos(ip, port) {}

void WemosDLEDenBuzzer::aan(const std::string& kleur) {
    makeSendMessage("DUALLED" + kleur, "1");
}

void WemosDLEDenBuzzer::uit(const std::string& kleur) {
    makeSendMessage("DUALLED" + kleur, "0");
}

void WemosDLEDenBuzzer::buzzer(int status) {
    makeSendMessage("BUZZER", std::to_string(status));
}
