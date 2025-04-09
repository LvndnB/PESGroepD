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
  //Maak de socket aan:
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
  //
    printf("De socket is aangemaakt...\n");
  //Bind de socket aan het protocol, de port en het ip address waar het naar moet luisteren. 0.0.0.0 voor any.
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
//Laat de socket luisteren op het juiste ip,port etc.
  do {
  int back = 256;
  int listn = listen(sockfd, back);
  if (listn < 0) {

    perror("Kan niet luisteren op deze port!");
    return 1;
  }
  printf("Er wordt nu geluisterd op de port...\n");
  socklen_t addrlen = sizeof(addr);
    //Accepteer de verbinding en maak een nieuwe socket aan voor deze verbinding genaamd "acc", die verbindingen Accepteert wanneer het het juiste address probeert te bereiken.
  int acc = accept(sockfd, (struct sockaddr*)&addr, &addrlen);
  if (acc < 0) {
  perror("Kan de connectie niet accepteren!");
    return 1;
  }
  printf("De connectie is geaccepteerd door de server.\n");
  do {
      //Gebruik de char rec samen met de functie recv om binnenkomende informatie op te vangen.
  rec = recv(acc, buf, sizeof(buf), 0);
    if (rec > 0) {
        sscanf(buf, "%49[^=]=%s[^\n]", key, val);
        //parse de key, zodat de rpi weet waar de informatie naartoe moet worden gestuurd.
	if (strcmp(key, "ENCODERRPM") == 0) {
          //Stuur de data door over uart:
		uart_class uart_1 = uart_class("/dev/ttyS0");
		uart_1.send(buf,sizeof(buf));
	}
//print beide waarden uit. Dit is momenteel handig voor eventuele debugging.
  printf("%s\n", key);
  printf("%s\n", val);
    }
  } while(rec < 0 && rec != '\n');
//Verstuur bericht over de socket.
  int msg = send(acc, recve, sizeof(recve), 0);
  if (msg < 0) {
    perror("Niks ontvangen");
    return 1;
  } 
//  printf("%s\n", recve);
//Sluit de socket met de andere rpi af.
  printf("De socket wordt gesloten...\n");
  int sock1 = close(acc);
  
  if (sock1 != 0) {
    perror("De Socket kan niet sluiten!");
    return 1;
  }
  printf("De socket is gesloten.\n");
//    ++count;
  } while (1);
  //Aan het eind van deze code wordt de socket die uitluisterd ook automatisch gesloten.
  return 0;
}
