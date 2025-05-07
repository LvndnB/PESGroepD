#include "Wemos.h"

Wemos::Wemos(const std::string& ip, int port) : ip(ip), port(port) {}

void Wemos::makeSendMessage(const std::string& key, const std::string& value) {
    SocketClient client;
    std::string message = key + "=" + value + "\n";
    client.send(ip, port, message);
}