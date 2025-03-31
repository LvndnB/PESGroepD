#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define WEMOSLED_IP "192.168.1.172"
#define RPIB_IP "145.52.127.217"

char buffer[BUFFER_SIZE];
char key[50];
char value[20];

void encoderWemosRecieve()
{
    float encoderValue = atof(value);

    // Vragen om connectie met server: RPiB
    int RPiB_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in RPiB_addr;
    RPiB_addr.sin_family = AF_INET;
    RPiB_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, RPIB_IP, &RPiB_addr.sin_addr); // IP van RPiB

    if (connect(RPiB_fd, (struct sockaddr *)&RPiB_addr, sizeof(RPiB_addr)) == 0)
        {
            send(RPiB_fd, buffer, strlen(buffer), 0);
            close(RPiB_fd);
        }
}

void buttonWemosRecieve()
{
    if ((strcmp(value, "1") == 0))
    {

        // Vragen om connectie met server: wemosLED
        int wemosLED_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in wemosLED_addr;
        wemosLED_addr.sin_family = AF_INET;
        wemosLED_addr.sin_port = htons(PORT);
        inet_pton(AF_INET, WEMOSLED_IP, &wemosLED_addr.sin_addr); // IP van wemosLED

        // Led toggle
        int ledStatus = 0;
        if (ledStatus == 0)
        {
            strcpy(buffer, "DualLEDRood=1\n");
            ledStatus = 1;
        }
        else
        {
            strcpy(buffer, "DualLEDRood=0\n");
            ledStatus = 0;
        }

        // Zenden naar server: wemosLED
        if (connect(wemosLED_fd, (struct sockaddr *)&wemosLED_addr, sizeof(wemosLED_addr)) == 0)
        {
            send(wemosLED_fd, buffer, strlen(buffer), 0);
            close(wemosLED_fd);
        }
    }
}

int main()
{
    // Initialiseren listen socket
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Socket fout");
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ||
        listen(server_fd, 5) < 0)
    {
        perror("Bind/Luisteren fout");
        return 1;
    }

    printf("Server actief...\n");

    while (1)
    {
        // Connectie met een client
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_fd < 0)
            continue;

        // Ontvangen van een client
        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE - 1);
        printf("Ontvangen van %s: %s", inet_ntoa(client_addr.sin_addr), buffer);

        if (sscanf(buffer, "%49[^=]=%s[^\n]", key, value) == 2) // String parsing voor key = value
        {

            if ((strcmp(key, "ENCODERRPM") == 0))
            {
                encoderWemosRecieve();
                printf("Verzonden naar %s: %s=%s\n", RPIB_IP, key, value);
            }

            if ((strcmp(key, "BUTTON") == 0)) // Key connectie = BUTTON
            {
                buttonWemosRecieve();
                printf("Verzonden naar %s: %s=%s\n", WEMOSLED_IP, key, value);
            }

            
        }

        close(client_fd);
        printf("\n");
    }

    return 0;
}
