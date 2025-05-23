#include "procflow.h"
#include <unistd.h>
#include <cstdint>
#include <memory>


bool procflow::sendDataToDevice(device_t device, char *data, int lenght) {
    uint8_t pdu_part[] = {'s', device};
    bus.send(pdu_part, 2);   
    bus.send(data, lenght);
    uint8_t null = 0;
    bus.send(&null, 1);
    // TODO: ack
    return true;
}


rx_request_response procflow::requestDataFromDevice(device_t device, sensors_and_actuator_enum sensor) {
    uint8_t pdu_part[2] = {'r', device};
    bus.send(pdu_part, 2);
    //bus.send(key, key_size);

    char *recv = (char *) malloc(200);
    auto rapport = bus.receive_null_termenated(recv, 200);

    return {
        std::unique_ptr<char>(recv),
        rapport,
    };
}

