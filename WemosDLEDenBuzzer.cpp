#include "WemosDLEDenBuzzer.h"
#include <iostream>
#include <cstring>

WemosDLEDenBuzzer::WemosDLEDenBuzzer(const std::string& ip, int port) : Wemos(ip, port) {}

void WemosDLEDenBuzzer::aan(const std::string& kleur) {
    if (kleur == "GROEN" && !ledroodstatus) {
        ledgroenstatus = true;
    } else if (kleur == "ROOD") {
        ledroodstatus = true;
        ledgroenstatus = false;
    }

    makeSendMessage("DUALLEDGROEN", std::to_string(ledgroenstatus));
    makeSendMessage("DUALLEDROOD", std::to_string(ledroodstatus));
}

void WemosDLEDenBuzzer::uit(const std::string& kleur) {
    

    if (kleur == "GROEN" && ledgroenstatus) {
        ledgroenstatus = false;
    } else if (kleur == "ROOD" && ledroodstatus) {
        ledroodstatus = false;
    }

    makeSendMessage("DUALLEDGROEN", std::to_string(ledgroenstatus));
    makeSendMessage("DUALLEDROOD", std::to_string(ledroodstatus));
}

void WemosDLEDenBuzzer::buzzer(int status) {
    makeSendMessage("BUZZER", std::to_string(status));
}

bool WemosDLEDenBuzzer::isAan(const std::string& kleur) {
    if (kleur == "GROEN") {
        return ledgroenstatus;
    } else if (kleur == "ROOD") {
        return ledroodstatus;
    }

}
