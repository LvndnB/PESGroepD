#include <stdio.h>                      // printf
#include <string>
#include <unistd.h>                     //Used for UART
#include <fcntl.h>                      //Used for UART
#include <termios.h>                    //Used for UART
#include <stdint.h>
#include <expected>
#include "uart_class.h"


uart_class::uart_class(std::string uart_path) {


        // Open in non blocking read/write mode
        fd = open(uart_path.c_str(), O_RDWR | O_NOCTTY);
        if (fd == -1)
        {
                printf("Error - Unable to open UART.  Ensure it is not in use by another application\r\nwirering has not been tested\r\n");
                return;
        }

        // O_NOCTTY: 
        // When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
        //
        // this is not a terminal... so no c tty

        //CONFIGURE THE UART
        // For all options see man://termios(3)
        //
        struct termios options;
        tcgetattr(fd, &options);
        
        cfmakeraw(&options);

        int baudspead = B19200;
        options.c_cflag = baudspead | CS8;
        // CS8 = word lenght of 8
        // if PARENB is pressent paraty is on otherwise off.
        // if PARODD is present parity is odd otherwise it is even.

        options.c_cflag |= (CLOCAL | CREAD);  // enable reader and remove modem controls 
        
        //options.c_cflag &= ~CRTSCTS; //Disable hardware flow control

        cfsetispeed(&options, baudspead); 
        cfsetospeed(&options, baudspead);
        
        // options.c_iflag = IGNPAR; // if parity error remove char.
        options.c_oflag = 0;
        options.c_lflag = 0;

        options.c_cc[VMIN] = 1; // minimal bytes that is needed for the read to finished.
        options.c_cc[VTIME] = 0; // set read timeout to x/10 seconds (Decisecond)
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &options);
}

uart_class::~uart_class() {
    close(fd);
}

int uart_class::send(void *buff, int size)
{

    if (size == 0)
    {

        return 0;
    }

    int count = write(fd, buff, size);

    // TODO: check if send count is the same as param size.

    if (count < 0)
    {
        // TODO: fetch errno and handle err.
        printf("UART TX error\n");
        return -1;
    }

    return count;
}

enum recvErr {
    file_not_open,
    nothing_recv,
};

int uart_class::receive(void *buff, int size)
{
    char *buff_ptr = (char *) buff;
    if (fd == -1)
    {
        return -2;
    }

    int buff_index = 0;

    while (true)
    {
        int rx_length = read(fd, buff_ptr+buff_index, 1);

        if (rx_length < 0) // is error
        { 
            if (errno == EAGAIN) // whould block or no data recv
            {
                continue;
            }

            // TODO: check errno and return different when common error

            printf("Error %d recv\r\n", errno);
            return -rx_length;
        }

        if (buff_index > size) {
            return rx_length;
        }
        buff_index++;
    }
}

int uart_class::receive_null_termenated(char *buff, int size)
{
    if (fd == -1)
    {
        return -2;
    }

    const int read_size = 100;
    int buff_index = 0;
    int max_times_outside_of_buff = 10;
    
    enum {
        finding_start,
        finding_end,
    } state = finding_start;

     while (true)
    {
        int rx_length = read(fd, buff+buff_index, read_size);

        if (rx_length < 0) // is error
        { 
            if (errno == EAGAIN) // whould block or no data recv
            {
                continue;
            }

            // TODO: check errno and return different when common error

            printf("Error %d recv\r\n", errno);

            return -rx_length - 1; // a minus sign before a var inverts the value. example a = 21; -a = -21; 
        }


        for (int i = 0; i < read_size; i++) {
            
            if (state == finding_end &&
                buff[buff_index+i] == 0) {

                return buff_index;
            }

            if (state == finding_start &&
                buff[buff_index+i] == 2) {
                buff_index -= read_size; // to counteract the first ++
                state = finding_end;
                // TODO: trim data so that buff_index+i would be the first char in string
            } 
        }
        if (state == finding_start)
        {
            max_times_outside_of_buff--;

            if (max_times_outside_of_buff == 0)
            {
                return -2; // timeout error
            }
        }

        if (state == finding_end) { // overwrite start of buffer if msg is not starded
            buff_index += read_size;
        }

        if (buff_index > size - read_size) { // prevent buffer offerflow
            buff[buff_index] = 0;
            return 0; 

        }
    }
}
