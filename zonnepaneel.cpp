#include "zonnepaneel.h"
#include <algorithm>  // voor std::max en std::min
#include <cstdlib>


Zonnepaneel::Zonnepaneel(RGB* rgbLed) : rgbLed(rgbLed) {
}

void Zonnepaneel::requestFromSensor() {
  procflow bus = procflow("dev/ttyS0");

  rx_request_response rapport = bus.requestDataFromDevice(STM1, zon);

  if (rapport.rapport.error == 0) {
    verwerkMetingen(std::atof(rapport.msg.get()));

  } 
}


float Zonnepaneel::berekenAmpere(float waarde) {
    // Voorbeeld: raw ADC-waarde (0-1023) omzetten naar stroom in A
    // Stel: 0 = 0A, 1023 = 5A → schaal om naar ampère
    return (waarde / 1023.0f) * 5.0f;
}

void Zonnepaneel::verwerkMetingen(float rawValue) {
    float stroom = berekenAmpere(rawValue);

    int r, g, b;

    if (stroom <= 1.8f) {
        float t = (stroom - 0.5f) / (1.8f - 0.5f);
        t = std::max(0.0f, std::min(1.0f, t));
        r = 255;
        g = static_cast<int>(165 * t);
        b = 0;
    } else if (stroom <= 2.5f) {
        float t = (stroom - 1.8f) / (2.5f - 1.8f);
        t = std::max(0.0f, std::min(1.0f, t));
        r = static_cast<int>(255 * (1 - t));
        g = static_cast<int>(165 + 90 * t);
        b = 0;
    } else if (stroom <= 3.4f) {
        float t = (stroom - 2.5f) / (3.4f - 2.5f);
        t = std::max(0.0f, std::min(1.0f, t));
        r = static_cast<int>(255 * t);
        g = 255;
        b = 0;
    } else {
        r = 255;
        g = 255;
        b = 0;
    }
    // TODO: retun 
    // rgbLed->sendToActuator(r, g, b, STM1);
}
