#ifndef WEMOS_H
#define WEMOS_H


#include <string>
#include "SocketClient.h"

class Wemos {
protected:
    std::string ip;
    int port;
    
public:
    Wemos(const std::string& ip, int port);
    void makeSendMessage(const std::string& key, const std::string& value);
};

#endif