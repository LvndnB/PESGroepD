#include <cstdio>
#include "uart_class.h"
#include <unistd.h>

//std::map<int, uart_class> fd_map;
uart_class *uartOne;

int main_loop();

int main(int argc, char **argv) {

    if (argc < 2 ) {
        fprintf(stderr, "Usage %s ip\r\n", argv[0]);
        return 1;
    }

    // connect to argv[1] ipaddr


    // connect to uart

    uartOne = new uart_class("/dev/serial0");
    //fd_map[uart_class.get_fd()] = uart_class;
    int res;

    // go to main_loop
    do {
        res = main_loop();
    } while ( res >= 0 );

    delete uartOne;
}


int main_loop() {
    char buff[20];
    int rec = uartOne->receive(buff, sizeof( buff)-1);
    buff[rec] = 0;
    printf("recieved: %s\r\n", buff);
    char data[] = "test send";
    uartOne->send(data, 9);
    usleep(70000);

    return 1;
}
