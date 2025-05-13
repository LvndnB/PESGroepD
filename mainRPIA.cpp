#include "SocketServer.h"
#include "WemosDLEDenBuzzer.h"
#include "WemosEncoder.h"

#define PORT 12345
#define WEMOSDLEDENBUZZER_IP "192.168.0.106"
#define WEMOSENCODER_IP "192.168.0.100"

int main() {
    WemosDLEDenBuzzer dualled(WEMOSDLEDENBUZZER_IP, PORT);
    WemosEncoder encoder(WEMOSENCODER_IP, PORT, &dualled);

    SocketServer server(PORT, &encoder);
    server.start();
    return 0;
}
