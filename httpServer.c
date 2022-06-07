#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "./ipAddr.h"

int main(int argc, char** argv) {
  int socketFD;
  struct sockaddr_in myAddress;

  socketFD = socket(AF_INET, SOCK_STREAM, 0);

  if (socketFD < 0) {
    perror("Cannot Create FD");
    return 0;
  }

  memset(&myAddress, '0', sizeof(myAddress));

  myAddress.sin_family = AF_INET;
  myAddress.sin_port = PORT;

  if (inet_pton(AF_INET, IP_ADDR, &myAddress.sin_addr) <= 0) {
    printf("Address Not Supported\n");
    return 0;
  }
  else {
    printf("%s\n", inet_ntoa(myAddress.sin_addr));
  }

}