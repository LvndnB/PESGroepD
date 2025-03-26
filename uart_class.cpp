#include <cstring>
#include <exception>
#include <stdio.h>
#include <string>
#include <unistd.h>                     //Used for UART
#include <fcntl.h>                      //Used for UART
#include <termios.h>                    //Used for UART
#include <stdint.h>
#include <expected>
#include "uart_class.h"


uart_class::uart_class(std::string uartPath) {
        //-------------------------
        //----- SETUP USART 0 -----
        //-------------------------
        //At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively

        //OPEN THE UART
        //The flags (defined in fcntl.h):
        //      Access modes (use 1 of these):
        //              O_RDONLY - Open for reading only.
        //              O_RDWR - Open for reading and writing.
        //              O_WRONLY - Open for writing only.
        //
        //      # O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. 
        //
        //      When set read requests on the file can return immediately with a failure status
        //      if there is no input immediately available (instead of blocking). Likewise, write requests can also return
        //      immediately with a failure status if the output can't be written immediately.
        //
        //      O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.   
        fd = open(uartPath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);          //Open in non blocking read/write mode
        if (fd == -1)
        {
                //ERROR - CAN'T OPEN SERIAL PORT
                printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
                return;
        }

        //CONFIGURE THE UART
        //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
        //      Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
        //      CSIZE:- CS5, CS6, CS7, CS8
        //      CLOCAL - Ignore modem status lines
        //      CREAD - Enable receiver
        //      IGNPAR = Ignore characters with parity errors
        //      ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
        //      PARENB - Parity enable
        //      PARODD - Odd parity (else even)
        struct termios options;
        tcgetattr(fd, &options);
        //if (strcmp("/dev/ttyAMA3", uartPath.c_str())) {

        options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
       // } else {
          //  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
        //}
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &options);
        }

uart_class::~uart_class() {
    close(fd);
}

int uart_class::send(void *buff, int size) {
    if (size > 0) {
        return 0; // please no syscall
    }

    int count = write(fd, buff, size);              //Filestream, bytes to write, number of bytes to write

    // TODO: check if send count is the same as param size.

    if (count < 0) {
        // TODO: fetch errno and handle err.
        printf("UART TX error\n");
        return -1;
    }

    return 0;
}


enum recErr {
    file_not_open,
    nothing_recv,

};

int uart_class::receive(void *buff, int size) {
    if (fd == -1)
    {
        return -2;
    }

    // Read up to 255 characters from the port if they are there
    int rx_length = read(fd, buff, size);		//Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0)
    {
        //An error occured (will occur if there are no bytes)
        // TODO: check errno and return different when common error 
        return -1;
    }
    else if (rx_length == 0)
    {
        return 0; // no data in buff.
    }

    return rx_length;

}

int uart_class::get_fd() {
    return fd;
}
