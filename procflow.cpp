#include "procflow.h"
#include <ctime>
#include <unistd.h>
#include <stdio.h>
#include <cstdint>
#include <memory>
#include <sys/time.h>
#include <cstring>

bool procflow::syncTime(device_t device) {
    /* fetch time data */
    struct timeval timeval; 
    gettimeofday(&timeval, NULL);

    /* -- time sensitive part -- */
    char decisecond_str[4] = {0}; 
    snprintf(decisecond_str, 4, "%2lx", timeval.tv_usec/10);
    bus.send(decisecond_str, strlen(decisecond_str));

    /* -- interrupt -- */
    uint8_t pdu_part[] = {'c', device};
    bus.send(pdu_part, 2); 

    /* -- send rest of time */
    char second_str[3] = {0}; 
    snprintf(second_str, 3, "%lx", timeval.tv_sec);
    bus.send(second_str, std::strlen(second_str));


    /* -- timezone -- */
    time_t t = time(NULL);
    struct tm localtime = {0};
    localtime_r(&t, &localtime);
    int timezone_hour_offset = localtime.tm_gmtoff / 60 /*to minute */ / 60 /*to hour*/;

    char timezone_str[2] = {0}; 
    snprintf(timezone_str, 2, "%1x", timezone_hour_offset);
    bus.send(timezone_str, std::strlen(timezone_str)); 

    // TODO: ack
    return true; 
}

bool procflow::sendDataToDevice(device_t device, char *data, int lenght) {
    uint8_t pdu_part[] = {'s', device};
    bus.send(pdu_part, 2);   
    bus.send(data, lenght);
    std::uint8_t null = 0;
    bus.send(&null, 1);
    // TODO: ack
    return true;
}


rx_request_response procflow::requestDataFromDevice(device_t device, sensors_and_actuator_enum sensor) {
    uint8_t pdu_part[2] = {'r', device};
    bus.send(pdu_part, 2);
    bus.send(&sensor, 1);

    char *recv = (char *) malloc(200);
    auto rapport = bus.receive_null_termenated(recv, 200);

    return {
        std::unique_ptr<char>(recv),
        rapport,
    };
}

