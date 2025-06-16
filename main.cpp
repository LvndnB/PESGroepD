#include "procflow.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <vector>

#include "RGB.h"
#include "Temperatuur.h"
#include "servo.h"
#include "klok.h"
#include "SocketClient.h"
#include "Ventilator.h"
#include "zonnepaneel.h"
#include "Luchtkwaliteit.h"

int main(int argc, char **argv) {
    procflow bus = procflow("/dev/ttyS0");
    Servo servo;
    std::vector<RGB> rgbLeds = {
        RGB(STM1), // RGB voor temperatuur
        RGB(STM3)  // RGB voor zonnepaneel
    };	
    Zonnepaneel zonnepaneel(&rgbLeds[1]);
    Zevensegmentendisplay clock(STM2);
    Temperatuur temperatuursensor;
    Ventilator ventilator;
    Luchtkwaliteit luchtkwaliteit(&ventilator);

    while (true) {
        // ------------------------------------------------------------------------------------

        // HIERONDER STAAT DE CONCEPT MAIN LOOP VAN HET PROGRAMMA //


        printf("test clock\r\n");
        clock.updateToCurrentTime();
        usleep(50000);

        printf("test switch\r\n");
        rgbLeds[1].checkRGBSwitch(STM1);

        usleep(50000);

        zonnepaneel.requestFromSensor(); // stuurt RGB zon aan

        usleep(50000);

        printf("test temp\r\n");
        rgbLeds[0].sendColorToActuator(temperatuursensor.requestAsColor(25, 29)); // stuurt RGB temperatuur aan

        printf("test sun\r\n");
        usleep(50000);

        printf("test vent\r\n");
        luchtkwaliteit.requestFromSensor(); // ventilator aansturen op basis van luchtkwaliteit

        usleep(50000);
        
        printf("test servo\r\n");
        servo.update(STM3);

        usleep(50000);

        // ------------------------------------------------------------------------------------





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


        // fetch baseline;

        /*
        clock.updateToCurrentTime();
        double baseline = temperatuursensor.requestFromSensor();
        rgbzon.sendColorToActuator(color(0,0,0));
        usleep(400);
        rgbzon.sendColorToActuator(color(255,255,255));
        usleep(400);
        rgbzon.sendColorToActuator(color(0,0,0));
        usleep(400);
        rgbzon.sendColorToActuator(color(255,255,255));
        usleep(400);
        rgbzon.sendColorToActuator(color(0,0,0));
        sleep(3);
        rgbzon.sendColorToActuator(temperatuursensor.requestAsColor(baseline-0.5, baseline+2)); // Twee graden warmer dan standaard komt niet vaak voor irl
        sleep(4);
        */
        

        
        //servo.update(STM3);
        //usleep(50000);


/*
        rgbLeds[0].checkRGBSwitch(STM1);
        usleep(50000);
        rgbLeds[0].sendColorToActuator(color(0, 255, 0)); // Standaard kleur groen
        rgbLeds[1].sendColorToActuator(color(255, 0, 255));
        sleep(1);
        rgbLeds[0].sendColorToActuator(color(255, 0, 255));
        rgbLeds[1].sendColorToActuator(color(0, 255, 0));
        sleep(1);
*/

        //servo.sendToActuator(STM3, 1); // Deur openen
        //sleep(1);
        //servo.sendToActuator(STM3, 0); // Deur sluiten
        //sleep(1);



        /*
        SocketClient client;
        char message[100];
        sprintf(message, "TEMPLOG=20");
        client.send("192.168.0.101", 12345, message);
        SocketClient client1;
        sprintf(message, "CO2LOG=445");
        client1.send("192.168.0.101", 12345, message);
        SocketClient client2;
        sprintf(message, "LUCHTVOCHTIGHEIDLOG=31");
        client2.send("192.168.0.101", 12345, message);
        SocketClient client3;
        sprintf(message, "ZONNEPANEELLOG=20.50");
        client3.send("192.168.0.101", 12345, message);
        sleep(1);
        */
        
    }
}

