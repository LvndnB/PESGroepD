/**
 * @file WemosPulseOxy.cpp
 * @author PESGroepD
 * @brief 
 * @version 0.1
 * @date 2025-06-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "WemosPulseOxy.h"
#include <cstdlib>
#include "WemosDLEDenBuzzer.h"
#include <string>
#include <iostream>

/**
 * @brief Construct a new WemosPulseOxy:: WemosPulseOxy object
 * 
 * @param ip 
 * @param port 
 * @param buz Buzzer object
 * @param led DualLED object
 * @param pulseoxyLog Logging object
 */
WemosPulseOxy::WemosPulseOxy(const std::string& ip, int port, WemosDLEDenBuzzer* buz, WemosDLEDenBuzzer* led, Logger* pulseoxyLog) : Wemos(ip, port), buzzer(buz), dualled(led), pulseoxyLog(pulseoxyLog) {
    
}

/**
 * @brief Possibility to check bloodoxygen and logging
 * 
 * @param oxyString Oxygen value in string
 */
void WemosPulseOxy::checkOxy(const std::string& oxyString){
    pulseoxyLog->log("Bloedzuurstofgehalte in %: " + oxyString);
}

/**
 * @brief Checking heartbeat pulse and handling if to high, turns DualLED red and Buzzer on/off
 * 
 * @param pulseString Heartbeat pulse in string
 */
void WemosPulseOxy::checkPulse(const std::string& pulseString){

    
    

    pulse = std::stoi(pulseString);

    if ((pulse > pulsegrenswaarde) && !alarm) {
        buzzer->buzzer(1);
        dualled->aan("ROOD");
        alarm = 1;

        pulseoxyLog->log("GRENSWAARDE HARTSLAG OVERSCHREDEN! Hartslag in BPM: " + pulseString);

    } else if(alarm && (pulse <= pulsegrenswaarde) ) {
        buzzer->buzzer(0);
        dualled->uit("ROOD");
        alarm = 0;

        pulseoxyLog->log("Hartslag in BPM: " + pulseString);
    }

}