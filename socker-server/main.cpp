#include <sys/socket.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "uart_class.h"
int main() {
  char val[50];
  char key[50];
  int count = 0;
  char recve[50] = "...";
  char buf[50];
  int rec = 0;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
    perror("Kan de socket niet aanmaken");
      return 1;
    }

//if (count > 0) {
  //int opt = 1;
    //if (setsockopt(acc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    //perror("Hergebruiken van de socket addres is mislukt");
   //   return 1;
//}
 // }
    printf("De socket is aangemaakt...\n");
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");

  int binden = bind(sockfd,(struct sockaddr*)&addr, sizeof(addr));
  if (binden < 0) {
    perror("Kan de socket niet verbinden");
    return 1;
  }
  printf("De socket is verbonden...\n");
do {
  int back = 256;
  int listn = listen(sockfd, back);
  if (listn < 0) {

    perror("Kan niet luisteren op deze port!");
    return 1;
  }
  printf("Er wordt nu geluisterd op de port...\n");
  socklen_t addrlen = sizeof(addr);
  int acc = accept(sockfd, (struct sockaddr*)&addr, &addrlen);
  if (acc < 0) {
  perror("Kan de connectie niet accepteren!");
    return 1;
  }
  printf("De connectie is geaccepteerd door de server.\n");
  do {
  rec = recv(acc, buf, sizeof(buf), 0);
    if (rec > 0) {
        sscanf(buf, "%49[^=]=%s[^\n]", key, val);
	if (strcmp(key, "ENCODERRPM") == 0) {
		uart_class uart_1 = uart_class("/dev/ttyS0");
		uart_1.send(buf,sizeof(buf));
	}

  printf("%s\n", key);
  printf("%s\n", val);
    }
  } while(rec < 0 && rec != '\n');

  int msg = send(acc, recve, sizeof(recve), 0);
  if (msg < 0) {
    perror("Niks ontvangen");
    return 1;
  } 
//  printf("%s\n", recve);

  printf("De socket wordt gesloten...\n");
  int sock1 = close(acc);
  
  if (sock1 != 0) {
    perror("De Socket kan niet sluiten!");
    return 1;
  }
  printf("De socket is gesloten.\n");
//    ++count;
  } while (1);
  return 0;
}
