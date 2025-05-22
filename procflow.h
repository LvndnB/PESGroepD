#pragma once
#include <cstdint>
#include "uart_class.h"
#include <string>

typedef enum {
    STM1 = 0x20,
    STM2 = 0x21,
    STM3 = 0x22,
} device_t;


typedef enum {
	klok = 'k',
	co2 = 'c',
	temp = 't'
} sensors_and_actuator_enum;

class procflow {
    private:
        uart_class bus;
    public:
        procflow(std::string uart_path):bus(uart_path) {}

        bool sendDataToDevice (device_t device, char *data, int lenght);
        char *requestDataFromDevice(device_t device, sensors_and_actuator_enum sensor);
};
