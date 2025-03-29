#include <string>
#include <termios.h> // provides baudrate divides
class uart_class {

    private:
        int fd = -1;
        
        // STM32 has in the specsheet an Auto baud rate detection. Maybe use that
        // STM32 baudrate is calculated with f<sub>clock</sub> / uartdiv
        //
        // Target rate is 3 int per refresh
        // a refresh every 80 ms (Can be slowed to 120ms)
        // 1000 / 80 = 12,5 times send in a second
        // key 10 Byte
        // value 15 Byte
        // 25*8*12,5 = 2400
        // minima baudrate is 2400 But then i need to be vigilant of when i transmit data.
        // And always look if i have massages to send. 3x should be enough.
        enum baudrate {
                b50	 = B50,
                b75 = B75,
                b110 = B110,
                b134 = B134,
                b150 = B150,
                b200 = B200,
                b300 = B300,
                b600 = B600,
                b1200 = B1200, // maximum baud for auto detect.
                b1800 = B1800, // I made uart 3 work with this boudrate
                b2400 = B2400,
                b4800 = B4800,
                b9600 = B9600,
                b19200 = B19200, // uart default and uart1 does support this. 
                b38400 = B38400,
        };

    public:
        uart_class(std::string uartPath);
        ~uart_class();
        int send(void *buff, int size);
        int receive(void *buff, int buffsize );
        int get_fd();

};
