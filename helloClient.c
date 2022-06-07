#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080
// Made just to check if server1.c works

char buffer[50000] = { 0 };

int main(int argc, char** argv) {
  int socketFd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketFd < 0) {
    perror("Cannot create FD");
    return 0;
  }

  struct sockaddr_in serverAddress;

  memset(&serverAddress, '0', sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "192.168.1.3", &serverAddress.sin_addr) <= 0) {
    perror("Address Translation Error");
    return 0;
  }

  if (connect(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    perror("Cannot Connect to Server");
    return 0;
  }
  char* msg = "Hello From Client";
  send(socketFd, msg, strlen(msg), 0);
  printf("Hello Message Sent\n");
  int valread = read(socketFd, buffer, 50000);
  printf("Received Message:\t");
  printf("%s\n", buffer);
  return 0;
}