#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H


#include "WemosEncoder.h"
#include "WemosPulseOxy.h"


class SocketServer {
private:
    int port;
    WemosEncoder* encoder;
    WemosPulseOxy* pulseoxy;

public:
    SocketServer(int port, WemosEncoder* encoder, WemosPulseOxy* pulseoxy);
    void start();
};

#endif