#include "WemosPulseOxy.h"
#include <cstdlib>
#include "WemosDLEDenBuzzer.h"
#include <string>
#include <iostream>


WemosPulseOxy::WemosPulseOxy(const std::string& ip, int port, WemosDLEDenBuzzer* buz, WemosDLEDenBuzzer* led, Logger* pulseoxyLog) : Wemos(ip, port), buzzer(buz), dualled(led), pulseoxyLog(pulseoxyLog) {
    
}

void WemosPulseOxy::checkOxy(const std::string& oxyString){
    pulseoxyLog->log("Bloedzuurstofgehalte in %: " + oxyString);
}

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