#include "procflow.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

int main(int argc, char **argv) {
    procflow bus = procflow("/dev/ttyS0");
    while (true) {
        std::cout << std::endl << "----" << std::endl;
        char *data = bus.requestDataFromDevice(STM2, co2);
        std::cout << data;
        std::cout << std::endl << "----" << std::endl;
        usleep(5000);
        bus.sendDataToDevice(STM1, data, strlen(data));
        free(data);
        usleep(30000);
    }
}

