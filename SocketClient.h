#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <string>

class SocketClient {

private: 
    int sock;
public:
    SocketClient();
    ~SocketClient();
    void send(const std::string& ip, int port, const std::string& message);
};

#endif