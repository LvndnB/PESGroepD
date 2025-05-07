#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H


#include "WemosEncoder.h"


class SocketServer {
private:
    int port;
    WemosEncoder* encoder;

public:
    SocketServer(int port, WemosEncoder* encoder);
    void start();
};

#endif