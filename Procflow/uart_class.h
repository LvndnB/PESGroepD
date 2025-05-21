#pragma once
#include <string>
#include <termios.h> // provides baudrate divides
class uart_class {

    private:
        int fd = -1;
        
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
        uart_class(std::string uartPath);
        ~uart_class();
        int send(void *buff, int size);
        int receive(void *buff, int buffsize );
        int receive_null_termenated(char *buff, int buffsize);
        int get_fd();

};
