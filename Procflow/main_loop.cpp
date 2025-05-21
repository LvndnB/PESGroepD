#include "uart_class.h"
#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h>

int us_to_ms = 1000;

uart_class bus = uart_class("/dev/ttyS0");
int sock = 0;

int init_eh(int sock_in) {
    sock = sock_in;
}

// 0 = deur
// 1 = klok
// 2 = raam
// 3 = other pi
typedef enum {
    other = 0,
    klok = 0x20,
    deur = 0x21,
    sensors = 0x22,
} device_t;

int chipSelect = 0;


/*typedef struct {
   int device_size = 0;


} device_state_rapport_t;*/

bool sendDataToDevice(device_t device, char *data, int lenght) {
    uint8_t pdu_part[] = {'s', device};
    bus.send(pdu_part, 2);   
    bus.send(data, lenght);
    uint8_t null = 0;
    bus.send(&null, 1);

    // todo ack
}

char *requestDataFromDevice(device_t device, char *key, uint8_t key_size) {
    uint8_t pdu_part[2] = {'r', device};
    bus.send(pdu_part, 2);
    //bus.send(key, key_size);


    char *recv = (char *) malloc(200);
    bus.receive_null_termenated(recv, 90);
    return recv;
}

int loop() {
    std::cout << std::endl << "----" << std::endl;
    char *data = requestDataFromDevice(sensors, "co2", 4);
    //for (int i = 0; i < 80; i++) {
        std::cout << data/*[i]*/;
    //}
    std::cout << std::endl << "----" << std::endl;
    usleep(5000);
    sendDataToDevice(sensors, data, strlen(data));
    free(data);
    usleep(30000);
    //char *data2 = requestDataFromDevice(klok, "Zonnepaneel", 4);
    //std::cout << std::endl << "voltage: " << std::endl;
    //std::cout << data2 << std::endl;
    //free(data2);
    return 0;
}
