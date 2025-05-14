#include "SocketServer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define BUFFER_SIZE 1024

SocketServer::SocketServer(int port, WemosEncoder* encoder, WemosPulseOxy* pulseoxy) : port(port), encoder(encoder), pulseoxy(pulseoxy) {}

void SocketServer::start() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket fout");
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 ||
        listen(server_fd, 5) < 0) {
        perror("Bind/Luisteren fout");
        close(server_fd);
        return;
    }

    std::cout << "Server actief op poort " << port << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t addr_size = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if (client_fd < 0) continue;

        char buffer[BUFFER_SIZE] = {0};
        read(client_fd, buffer, BUFFER_SIZE - 1);
        std::cout << "Ontvangen van " << inet_ntoa(client_addr.sin_addr) << ": " << buffer;

        char key[50], value[20];
        if (sscanf(buffer, "%49[^=]=%19s", key, value) == 2) {
            std::string sKey(key), sValue(value);
            if (sKey == "ENCODERRPM") {
                encoder->sendRPM(sValue);
            } else if (sKey == "ENCODER_STATUS") {
                encoder->sendStatus(sValue);
            } else if (sKey == "HARTSLAGAVG") {
                // Implementatie van HARTSLAG bijv naar Buzzer en dualLed (dus check grenswaarde in WemosPulseOxy)
                pulseoxy->checkPulse(sValue);
            } else if (sKey == "ZUURSTOF") {
                // Implementatie van ZUURSTOF bijv naar datalogging
            }
        }

        close(client_fd);
        std::cout << "\n";



    }
    close(server_fd);
}
