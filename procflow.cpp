#include "procflow.h"
#include <unistd.h>
#include <cstdint>


bool procflow::sendDataToDevice(device_t device, char *data, int lenght) {
    uint8_t pdu_part[] = {'s', device};
    bus.send(pdu_part, 2);   
    bus.send(data, lenght);
    uint8_t null = 0;
    bus.send(&null, 1);

    // TODO: ack
    return true;
}

char *procflow::requestDataFromDevice(device_t device, sensors_and_actuator_enum sensor) {
    uint8_t pdu_part[2] = {'r', device};
    bus.send(pdu_part, 2);
    //bus.send(key, key_size);


    char *recv = (char *) malloc(200);
    bus.receive_null_termenated(recv, 90);
    return recv;
}

