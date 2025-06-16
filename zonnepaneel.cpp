#include "zonnepaneel.h"
#include <algorithm>  // voor std::max en std::min
#include <cstdlib>
#include "SocketClient.h"
#include <iostream>
#include <cstring>

Zonnepaneel::Zonnepaneel(RGB* rgbLed) : rgbLed(rgbLed) {
}

void Zonnepaneel::update() {
  procflow bus = procflow("/dev/ttyS0");

  rx_request_response rapport = bus.requestDataFromDevice(STM1, zon);

  if (rapport.rapport.error == 0) {
    
    char key[20];
    char value[40];
    sscanf(rapport.msg.get(), "%[^=]=%s", key, value);


        if (strcmp(key, "zon") == 0){

            verwerkMetingen(std::atof(value));
              
        }
  } 
}


float Zonnepaneel::berekenVoltage(float waarde) {
    // Voorbeeld: raw ADC-waarde (0-4095) omzetten naar stroom in V
    return (waarde / 4095.0f) * 3.3f * 2.0f;   // 3.3V is de referentiespanning, 2.0 is de versterking van de stroom voor de spanningsdeler
}

void Zonnepaneel::verwerkMetingen(float rawValue) {
    float stroom = berekenVoltage(rawValue);

    printf("zonwaarde: %f\r\n\r\n", stroom);

    SocketClient client;
    char message[100];
    sprintf(message, "ZONNEPANEELLOG=%0.2f", stroom);
    client.send("192.168.0.101", 12345, message);

    int r, g, b;

    float stroomMin = 0.5f;
    float stroomMax = 3.4f;
    float t = (stroom - stroomMin) / (stroomMax - stroomMin);
    t = std::clamp(t, 0.0f, 1.0f);

    r = static_cast<int>(255 * (1.0f - t));
    g = static_cast<int>(255 * t);
    b = 0;

    // std::cout << stroom << std::endl;
    
    // TODO: retun 
    rgbLed->sendColorToActuator(color(r, g, b));
}
