#include "SocketClient.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/**
 * opens a socket file descriptor
 */
SocketClient::SocketClient() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket aanmaken mislukt");
    }
}

/**
 * closes a file descriptor
 */
SocketClient::~SocketClient() {
    if (sock >= 0) {
        close(sock);
        printf("Socket automatisch gesloten\n");
    }
}

/**
 * connect and send the contents in a tcp
 */
void SocketClient::send(const std::string& ip, int port, const std::string& message) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        ::send(sock, message.c_str(), message.length(), 0);
    }
}
