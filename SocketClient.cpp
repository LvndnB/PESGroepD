/**
 * @file SocketClient.cpp
 * @author PESGroepD
 * @brief 
 * @version 0.1
 * @date 2025-06-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "SocketClient.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


/**
 * @brief Construct a new SocketClient:: SocketClient object
 * 
 */
SocketClient::SocketClient() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket aanmaken mislukt");
    }
}

/**
 * @brief Destroy the Socket Client:: Socket Client object
 * 
 */
SocketClient::~SocketClient() {
    if (sock >= 0) {
        close(sock);
        std::cout << "Socket automatisch gesloten\n";
    }
}

/**
 * @brief Send socket message to device
 * 
 * @param ip Device ip-address
 * @param port Device socket port
 * @param message Message to be send
 */
void SocketClient::send(const std::string& ip, int port, const std::string& message) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        ::send(sock, message.c_str(), message.length(), 0);
    }
    std::cout << "Verzonden bericht " << message;
}