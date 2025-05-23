#include "procflow.h"
#include <ctime>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sys/time.h>

bool procflow::syncTime(device_t device) {
    struct timeval timeval; 

    time_t t = time(NULL);
    struct tm localtime = {0};
    localtime_r(&t, &localtime);

    int timezone_minute_offset = localtime.tm_gmtoff/60;

    int state = gettimeofday(&timeval, NULL);
    return false;
}

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

