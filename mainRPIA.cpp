#include "SocketServer.h"
#include "WemosDualLED.h"
#include "WemosEncoder.h"

#define PORT 12345
#define WEMOSDUALLED_IP "192.168.0.106"
#define WEMOSENCODER_IP "192.168.0.100"

int main() {
    WemosDualLED dualled(WEMOSDUALLED_IP, PORT);
    WemosEncoder encoder(WEMOSENCODER_IP, PORT, &dualled);

    SocketServer server(PORT, &encoder);
    server.start();
    return 0;
}
