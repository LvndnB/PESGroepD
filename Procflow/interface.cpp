#include <unistd.h>
#include <sys/socket.h>
#include "main_loop.h"
#include "uart_class.h"
#include <iostream>
// using namespace procflow;
int init_procflow() {

}

class procflow
{
public:
    /***
     * procflow is a library made for a schoolproject. To allow multible client on a uart connection. And to make sure
     * that enaught data has been
     */
    int deamon_socket = 0;

    procflow()
    {
        int sockets[2];

        int succ = socketpair(AF_UNIX, 0, 0, sockets);
        int procid = fork();

        // I am making two processes so us

        switch (procid)
        {
        case -1: // error
            break;

        case 0: // is Child
            std::cout << "forking success. Entering loop" << std::endl;
            loop();
            break;

        default: // is Parrent
            std::cout << "forking success. Child id: " << procid << std::endl;
            deamon_socket = sockets[0];
            break;
        }
    }

    void get_value(int id) {


    }

    void send(int key, char *val) {

    }

};