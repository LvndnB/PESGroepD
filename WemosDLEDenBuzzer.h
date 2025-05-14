#ifndef WEMOSDLEDENBUZZER_H
#define WEMOSDLEDENBUZZER_H


#include "Wemos.h"

class WemosDLEDenBuzzer : public Wemos {
public:
    WemosDLEDenBuzzer(const std::string& ip, int port);
    void aan(const std::string& kleur);
    void uit(const std::string& kleur);
    void buzzer(int status);
};

#endif
