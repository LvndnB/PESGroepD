#pragma once
#include <string>
#include <termios.h> // provides baudrate divides
class uart_class {

    private:
        int fd = -1;
        
        // TODO: select an higher boudrate
        // STM32 has in the specsheet an Auto baud rate detection. Maybe use that
        // STM32 baudrate is calculated with f<sub>clock</sub> / uartdiv
        enum baudrate {
                b50	 = B50,
                b75 = B75,
                b110 = B110,
                b134 = B134,
                b150 = B150,
                b200 = B200,
                b300 = B300,
                b600 = B600,
                b1200 = B1200,
                b1800 = B1800,
                b2400 = B2400,
                b4800 = B4800,
                b9600 = B9600,
                b19200 = B19200, // uart default
                b38400 = B38400,
        };

    public:

        /**
         * this opens and configures the uart on B19200
         * @param uart_path The uri to the tty file which the uart is assigned. The default should be /dev/ttyS# but it could also be /dev/ttyAMA#
         */
        uart_class(std::string uartPath);

        ~uart_class();

        /**
         * send buff in as an array of uint8_t's
         */
        int send(void *buff, int size);

        /** 
         * receive a predetermined size of bytes.
         *
         * Warning this function assumes that all data can be fit in the given buffer
         *
         * @param buff the location to be written.
         * @param size aspected size of bytes to be received
         */
        int receive(void *buff, int size);

        /**
         * receive a none deterministic size of data
         */
        int receive_null_termenated(char *buff, int buffsize);

};
