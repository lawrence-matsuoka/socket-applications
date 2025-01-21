#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

int main() {

  // Set the socket to IPv4, TCP socket, and protocol
  int socketFd = socket(AF_INET, SOCK_STREAM, 0);

  // Create an IPv4 address
  struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));

  // Set IP address and port variables
  char *ip = "142.250.188.46";
  address.sin_port = htons(80);

  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

  // Determine the connection status
  int result = connect(socketFd, address, sizeof(address));

  if (result == 0) {
    printf("Successfully connected!\n");
  }

  return 0;
}
