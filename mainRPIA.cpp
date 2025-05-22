#include "SocketServer.h"
#include "WemosDLEDenBuzzer.h"
#include "WemosEncoder.h"
#include "WemosPulseOxy.h"
#include "Logger.h"

#define PORT 12345
#define WEMOSDLEDENBUZZER_IP "192.168.0.106"
#define WEMOSENCODER_IP "192.168.0.100"
#define WEMOSPULSEOXY_IP "192.168.0.107"

int main() {
    Logger encoderLog("encoderLog.log");
    Logger pulseoxyLog("pulseoxyLog.log");
    WemosDLEDenBuzzer dualled(WEMOSDLEDENBUZZER_IP, PORT);
    WemosDLEDenBuzzer buzzer(WEMOSDLEDENBUZZER_IP, PORT);
    WemosEncoder encoder(WEMOSENCODER_IP, PORT, &dualled, &encoderLog);
    WemosPulseOxy pulseoxy(WEMOSPULSEOXY_IP, PORT, &buzzer, &dualled, &pulseoxyLog);
    

    SocketServer server(PORT, &encoder, &pulseoxy);
    server.start();

    

    
    return 0;
}
