#include <cstdio>
#include "uart_class.h"
#include <unistd.h>
#include <iostream>
#include <string.h>

//std::map<int, uart_class> fd_map;
uart_class *uartOne;

int test_rec();
int test_send();

int main(int argc, char **argv) {

    //if (argc < 2 ) {
    //    fprintf(stderr, "Usage %s ip\r\n", argv[0]);
    //    return 1;
    //}

    // connect to argv[1] ipaddr


    // connect to uart

    uartOne = new uart_class("/dev/ttyS0");
    //fd_map[uart_class.get_fd()] = uart_class;
    int res;

    // go to main_loop
    do {
        res = test_send();
    } while ( res >= 0 );

    delete uartOne;
}


int test_rec() {
    char data[200] = "";
    memset(&data, 0, 200);
    int size = uartOne->receive(data, 200);
    if (size > -1) {
        data[size] = 0;
        printf("data: \"%s\"\r\n", data);
    }
    
    usleep(10000);
    return 1;
}

int test_send() {

    char buff[] = "this is fake";
    
    uartOne->send(buff, strlen(buff));
    usleep(1000000);
    return 1;
}