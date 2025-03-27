#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main() {
  char buf[50] = { "test" };
  char rec[50];
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
    perror("Kan de socket niet aanmaken");
      return 1;
    }
    printf("De socket is aangemaakt...\n");
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
  perror("Socket kan niet verbinden");
    return 1;
  }
  printf("De Socket is verbonden...\n");

  printf("De socket is verbonden...\n");
  socklen_t addrlen = sizeof(addr);

  //Hier moet de main code komen...
  int msg = send(sockfd, buf, sizeof(buf), 0);
  if (msg < 0) {
    perror("Kon het bericht niet versturen");
    return 1;
  }
  printf("Het bericht is verstuurd.\n");
  int receive = recv(sockfd, rec, sizeof(rec), 0);
  if (receive < 0) {
    perror("Er is geen bericht ontvangen");
    return 1;
  }
  printf("%s\n", rec);

  printf("De Socket wordt gesloten.");
  close(sockfd);

  return 0;
}
