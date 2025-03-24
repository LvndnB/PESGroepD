#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_size = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket fout");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 ||
        listen(server_fd, 5) < 0) {
        perror("Bind/Luisteren fout");
        return 1;
    }

    printf("Server actief...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if (client_fd < 0) continue;

        

        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE - 1);
        printf("Ontvangen van %s: %s", inet_ntoa(client_addr.sin_addr), buffer);
        
        // Stuur het bericht door naar de LED WeMos
        int led_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in led_addr;
        led_addr.sin_family = AF_INET;
        led_addr.sin_port = htons(PORT);
        inet_pton(AF_INET, "192.168.1.172", &led_addr.sin_addr); // IP van de LED WeMos

        if (connect(led_fd, (struct sockaddr*)&led_addr, sizeof(led_addr)) == 0) {
            send(led_fd, buffer, strlen(buffer), 0);
            printf("Verzonden naar 192.168.1.172: %s\n", buffer);
            close(led_fd);
        }

        close(client_fd);
    }

    return 0;
}
