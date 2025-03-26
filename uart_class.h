#include <string>
class uart_class {

    private:
        int fd = -1;

    public:
        uart_class(std::string uartPath);
        ~uart_class();
        int send(void *buff, int size);
        int receive(void *buff, int buffsize );
        int get_fd();

};
