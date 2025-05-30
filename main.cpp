#include "procflow.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "RGB.h"
#include "Temperatuur.h"

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


        Temperatuur Temperatuursensor;
        RGB rgbtemp(STM1);
        // fetch baseline;
        double baseline = Temperatuursensor.requestFromSensor();
        sleep(2);
        rgbtemp.sendColorToActuator(Temperatuursensor.requestAsColor(baseline-0.5, baseline+2)); // Twee grade warmer dan standaard komt niet vaak voor iirc
        sleep(1);
        rgbtemp.sendColorToActuator(color(0,255,255));
        sleep(1);
        
        //usleep(10000000);
    }
}

