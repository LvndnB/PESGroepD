#include "procflow.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "RGB.h"
#include "Temperatuur.h"
#include "servo.h"

int main(int argc, char **argv) {
    procflow bus = procflow("/dev/ttyS0");
    Servo servo;
    RGB rgbzon(STM3);

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

/*
        Temperatuur temperatuursensor;
        RGB rgbtemp(STM1);
        // fetch baseline;
        double baseline = temperatuursensor.requestFromSensor();
        sleep(2);
        rgbtemp.sendColorToActuator(temperatuursensor.requestAsColor(baseline-0.5, baseline+2)); // Twee graden warmer dan standaard komt niet vaak voor irl
        sleep(1);
        rgbtemp.sendColorToActuator(color(0,255,255));
        sleep(1);
        */

        
        servo.update(STM3);
        usleep(50000);

        rgbzon.checkRGBSchakelaar(STM3);
        rgbzon.sendColorToActuator(color(0, 255, 0)); // Standaard kleur groen
        sleep(1);
        rgbzon.sendColorToActuator(color(255, 0, 255));
        sleep(1);



        //servo.sendToActuator(STM3, 1); // Deur openen
        //sleep(1);
        //servo.sendToActuator(STM3, 0); // Deur sluiten
        //sleep(1);

        
        //usleep(10000000);
    }
}

