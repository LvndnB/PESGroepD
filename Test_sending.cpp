#include <stdio.h>
#include "uart_class.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

    uart_class s1 = uart_class("/dev/ttyS0");
    uart_class s3 = uart_class("/dev/ttyAMA3");

    while (true) {
        char msg[] = "yeahea";
        s1.send(msg, strlen(msg));

        char msg2[] = "boo";
        s3.send(msg2, strlen(msg));

        printf("send");

        usleep(30000);
    }


}