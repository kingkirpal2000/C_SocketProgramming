#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "./ipAddr.h"
#include "./error.h"
#include <unistd.h>
#include <netinet/in.h>

int socketFD;
struct sockaddr_in myAddress;


error_t createSocket() {
  socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD < 0) {
    perror("Cannot Create FD");
    return ERROR;
  }

  return SUCCESS;
}

error_t setAddress() {
  memset(&myAddress, '0', sizeof(myAddress));

  myAddress.sin_family = AF_INET;
  myAddress.sin_port = htons(PORT);

  if (inet_pton(AF_INET, IP_ADDR, &myAddress.sin_addr) <= 0) {
    printf("Address Not Supported\n");
    return ERROR;
  }
  else {
    printf("%s, %d\n", inet_ntoa(myAddress.sin_addr), myAddress.sin_port);
  }

  return SUCCESS;
}

error_t bindSocket() {
  int result = bind(socketFD, (const struct sockaddr*)&myAddress, sizeof(myAddress));
  return result >= 0 ? SUCCESS : ERROR;
}

error_t setListenState() {
  int result = listen(socketFD, 3);
  printf("%d\n", result);
  return result >= 0 ? SUCCESS : ERROR;
}

error_t acceptNewConnections() {
  int clientSocketFD;

  long valRead;
  char* hello = "Hello From Server\n";
  while (1) {
    int addrlen = sizeof(myAddress);
    printf("\n++++++++++++Waiting for new connection++++++++++++\n\n");
    if ((clientSocketFD = accept(socketFD, (struct sockaddr*)&myAddress, (socklen_t*)&addrlen)) < 0) {
      perror("failed to accept new client");
      exit(EXIT_FAILURE);
    }
    char buffer[30000] = { 0 };
    valRead = read(clientSocketFD, buffer, 30000);
    printf("%s\n", buffer);
    write(clientSocketFD, hello, strlen(hello));
    printf("------------------Hello Message Sent------------------");
    close(clientSocketFD);
  }
}

int main(int argc, char** argv) {

  if (createSocket() == ERROR) return 0;
  else printf("Successfully Created File Descriptor\n..........\n");
  if (setAddress() == ERROR) return 0;
  else printf("Set address to %s\n..........\n", inet_ntoa(myAddress.sin_addr));
  if (bindSocket() == ERROR) return 0;
  else printf("Successfully Bound Socket File Descriptor to Address\n..........\n");
  if (setListenState() == ERROR) return 0;
  else printf("Server now listening\n..........\n");
  if (acceptNewConnections() == ERROR) return 0;
}

// socket() -> bind() -> listen() -> accept()
// read up on http server from here
