/**
 * @file WemosDLEDenBuzzer.cpp
 * @author PESGroepD
 * @brief 
 * @version 0.1
 * @date 2025-06-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "WemosDLEDenBuzzer.h"
#include <iostream>
#include <cstring>

/**
 * @brief Construct a new WemosDLEDenBuzzer:: WemosDLEDenBuzzer object
 * 
 * @param ip 
 * @param port 
 */
WemosDLEDenBuzzer::WemosDLEDenBuzzer(const std::string& ip, int port) : Wemos(ip, port) {}

/**
 * @brief DualLED turn on handling
 * 
 * @param kleur Colour to be turned on
 */
void WemosDLEDenBuzzer::aan(const std::string& kleur) {
    if (kleur == "GROEN" && !ledroodstatus && !ledgroenstatus) {
        ledgroenstatus = true;
    } else if (kleur == "ROOD" && !ledroodstatus) {
        ledroodstatus = true;
        ledgroenstatus = false;
    }

    makeSendMessage("DUALLEDGROEN", std::to_string(ledgroenstatus));
    makeSendMessage("DUALLEDROOD", std::to_string(ledroodstatus));
}

/**
 * @brief DualLED turn off handling
 * 
 * @param kleur Colour to be turned off
 */
void WemosDLEDenBuzzer::uit(const std::string& kleur) {
    

    if (kleur == "GROEN" && ledgroenstatus) {
        ledgroenstatus = false;
    } else if (kleur == "ROOD" && ledroodstatus) {
        ledroodstatus = false;
    }

    makeSendMessage("DUALLEDGROEN", std::to_string(ledgroenstatus));
    makeSendMessage("DUALLEDROOD", std::to_string(ledroodstatus));
}

/**
 * @brief Send buzzer status
 * 
 * @param status Upcoming status of the buzzer
 */
void WemosDLEDenBuzzer::buzzer(int status) {
    makeSendMessage("BUZZER", std::to_string(status));
}

/**
 * @brief Check if DualLED is on
 * 
 * @param kleur Colour to be checked
 * @return true 
 * @return false 
 */
bool WemosDLEDenBuzzer::isAan(const std::string& kleur) {
    if (kleur == "GROEN") {
        return ledgroenstatus;
    } else if (kleur == "ROOD") {
        return ledroodstatus;
    }
    return false;

}