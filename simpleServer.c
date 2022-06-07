#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "./ipAddr.h"

int main(int argc, char** argv) {
  // socket(domain, type, protocol)
  // domain = IP address family -> AF_INET
  // type = Virtual Circuit Service -> SOCK_STREAM
  // protocol = No variations of Virtual Circuit Service Exist -> 0
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Cannot create socket");
    return 0;
  }
  else {
    printf("Succesfully created socket with FD = %d\n", socket_fd);
  }

  // Bind(socket_fd, address w/ port, size of address)
  // socket_fd was returned by socket()
  // every address family has a different address strucutre provided by netinet lib
  // because each address structure is different, they have different sizes so we pass that in as well
  struct sockaddr_in address; // For IP we use sockaddr_in
  const int PORT = 8080;

  memset((char*)&address, 0, sizeof(address));
  address.sin_family = AF_INET; // address family for IP
  // htonl -> long int to network rep.
  // INADDR_ANY = any IP from one of your machine's network interfaces since 
  // server generally care which IP it is running off of only client cares about what IP to connect to for server
  // address.sin_addr.s_addr = htonl(INADDR_ANY);
  // can use INADDR_ANY and OS will pick IP addr of one of available interfaces 
  // to get from string to network byte order use inet_addr(char* addr)
  // to get string from network byte order use inet_ntoa(address.sin_addr);
  address.sin_addr.s_addr = inet_addr(IP_ADDR);
  // htons -> short int to network rep
  address.sin_port = htons(PORT);

  if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("bind failed");
    return 0;
  }
  else {
    printf("Successfully Bound to Socket FD = %d\n", socket_fd);
  }

  // listen(fd, backlog)
  // backlog is how many connections it should have pending before it starts refusing connections
  if (listen(socket_fd, 3) < 0) {
    perror("listen error");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Listen Success with backlog = 3\n");
  }

  char* hello = "Hi from server!\n";
  int clientSocket;
  long valRead;
  while (1) {
    // accept(fd, address, addrlen)
    int addrlen = sizeof(address);
    printf("\n++++++++++++Waiting for new connection++++++++++++\n\n");
    if ((clientSocket = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
      perror("failed to accept new client");
      exit(EXIT_FAILURE);
    }

    char buffer[30000] = { 0 };
    valRead = read(clientSocket, buffer, 30000);
    printf("%s\n", buffer);
    write(clientSocket, hello, strlen(hello));
    printf("------------------Hello Message Sent------------------");
    close(clientSocket);
  }

  return 0;

}