#ifndef WEMOSDUALLED_H
#define WEMOSDUALLED_H


#include "Wemos.h"

class WemosDualLED : public Wemos {
public:
    WemosDualLED(const std::string& ip, int port);
    void aan(const std::string& kleur);
    void uit(const std::string& kleur);
};

#endif