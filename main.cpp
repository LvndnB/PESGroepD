#include "procflow.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "RGB.h"

int main(int argc, char **argv) {
    procflow bus = procflow("/dev/ttyS0");
    while (true) {

        /*
        rx_request_response rapport = bus.requestDataFromDevice(STM3, co2);
        
        if (rapport.rapport.error == 0) {
            std::cout << "---\r\n";
            std::cout << rapport.msg.get();
            std::cout << "\r\n---\r\n";
            bus.sendDataToDevice(STM1, rapport.msg.get(), rapport.rapport.recieved_bytes);
        } else {
            std::cout << "[[ error " << rapport.rapport.error << " ] \r\n";
            std::cout << rapport.msg.get();
            std::cout << "\r\n[\\/\\/\\/]\r\n";
        }
            */


        RGB rgbtemp;
        rgbtemp.sendToActuator(255,0,255,STM1);
        sleep(1);
        rgbtemp.sendToActuator(0,255,255,STM1);
        sleep(1);
        
        //usleep(10000000);
    }
}

