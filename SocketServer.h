#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H


#include "WemosEncoder.h"
#include "WemosPulseOxy.h"
#include "Logger.h"


class SocketServer {
private:
    int port;
    WemosEncoder* encoder;
    WemosPulseOxy* pulseoxy;
    Logger* luchtkwaliteitLog;
    Logger* zonnepaneelLog;

public:
    SocketServer(int port, WemosEncoder* encoder, WemosPulseOxy* pulseoxy, Logger* luchtkwaliteitLog, Logger* zonnepaneelLog);
    void start();
};

#endif