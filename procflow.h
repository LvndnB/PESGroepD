#pragma once
#include <cstdint>
#include "uart_class.h"
#include <string>
#include <memory>

/*
 * Enum that contains the device id's
 */
typedef enum : uint8_t {
    STM1 = 0x20,
    STM2 = 0x21,
    STM3 = 0x22,
} device_t;


/**
 * 
 */
typedef struct {
    /**
     * response of the request. May be noise
     */
   std::unique_ptr<char> msg;

   /**
    * This contains the state of the msg. If the msg is completed or has a timeout.
    */
   uart_class::uart_rx_rapport rapport;
} rx_request_response;

/**
 * Enum of all the sensors that can be transmitted 
 */
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
    rgbSwitch = 'e' //Deze ook
} sensors_and_actuator_enum;

/**
 * this class is responsible for requesting and setting data to the STM chips. This is realized
 * by composing uart_class methods.
 */
class procflow {
    private:
        uart_class bus;
    public:

        /**
         * @param uart_path The uri to the tty file which the uart is assigned. The default should be /dev/ttyS# but it could also be /dev/ttyAMA#
         */
        procflow(std::string uart_path):bus(uart_path) {}

        /**
         * @deprecated
         * not used or tested 
         * sync bus user's internal clock to the unix epoch with the accuracy of a decisecond
         */
        bool syncTime(device_t);

        bool sendDataToDevice (device_t device, char *data, int lenght);

        /**
          * Requesting the desired data.
          *
          * @param device: to which device you want to send the pdu
          * @param sensors_and_actuator_enum: what should the device retreve
         */
        rx_request_response requestDataFromDevice(device_t device, sensors_and_actuator_enum sensor);
};
