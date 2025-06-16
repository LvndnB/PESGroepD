/**
 * @file WemosEncoder.cpp
 * @author PESGroepD
 * @brief 
 * @version 0.1
 * @date 2025-06-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "WemosEncoder.h"
#include <cstdlib>
#include "WemosDLEDenBuzzer.h"

/**
 * @brief Construct a new WemosEncoder:: WemosEncoder object
 * 
 * @param ip 
 * @param port 
 * @param led DualLED object
 * @param encoderLog Logging object
 */
WemosEncoder::WemosEncoder(const std::string& ip, int port, WemosDLEDenBuzzer* led, Logger* encoderLog) : Wemos(ip, port), dualled(led), encoderLog(encoderLog) {}

/**
 * @brief Check if RPM is > 0, then turn green DualLED on
 * 
 * @param rpm Encoder RPM
 */
void WemosEncoder::handleRPM(const std::string& rpm) {
    encoderLog->log("Encoder RPM: " + rpm);

    float rpmFloat = std::stof(rpm);
    
    if (rpmFloat == 0.0f && dualled->isAan("GROEN")) {
        dualled->uit("GROEN");
    } else if (rpmFloat > 0.0f && !dualled->isAan("GROEN")) {
        dualled->aan("GROEN");
    }


    
}