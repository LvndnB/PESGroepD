/**
 * @file Wemos.cpp
 * @author PESGroepD
 * @brief 
 * @version 0.1
 * @date 2025-06-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Wemos.h"

/**
 * @brief Construct a new Wemos:: Wemos object
 * 
 * @param ip Wemos IP-address
 * @param port Wemos socket port
 */
Wemos::Wemos(const std::string& ip, int port) : ip(ip), port(port) {}

/**
 * @brief Make a message with key & value to be send via socket
 * 
 * @param key Key of the message in CAPS
 * @param value Value of the message
 */
void Wemos::makeSendMessage(const std::string& key, const std::string& value) {
    SocketClient client;
    std::string message = key + "=" + value + "\n";
    client.send(ip, port, message);
}