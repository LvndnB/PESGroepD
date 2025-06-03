#pragma once
#include <cstdint>
#include "uart_class.h"
#include <string>
#include <memory>

typedef enum : uint8_t {
    STM1 = 0x20,
    STM2 = 0x21,
    STM3 = 0x22,
} device_t;


typedef struct {
   std::unique_ptr<char> msg;
   uart_class::uart_rx_rapport rapport;
} rx_request_response;

typedef enum : uint8_t {
	klok = 'k',
	co2 = 'c',
	temp = 't',
    zon = 'z',
    lucht = 'l',
    vent = 'v',
    rgbTemp = 't',
    rgbZon = 'o',
    deur = 'd',
    deurKnop = 'q', // onlogisch, aanpassen naar 2 char's
    rgbKnop = 'e' //Deze ook
} sensors_and_actuator_enum;

class procflow {
    private:
        uart_class bus;
    public:
        procflow(std::string uart_path):bus(uart_path) {}

        /**
         * sync bus user's internal clock to the unix epoch with the accuracy of a decisecond
         */
        bool syncTime(device_t);

        bool sendDataToDevice (device_t device, char *data, int lenght);

        rx_request_response requestDataFromDevice(device_t device, sensors_and_actuator_enum sensor);
};
