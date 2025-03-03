#include "error.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Initialize the socket
int main(int argc, char *argv[]) {
  int sockfd, portNum, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  // ensure user provides all three arguments (filename, server ip address, port number)
  // argv[0] = filename
  char buffer[255];
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(1);
  }

  // argv[2] = port number
  portNum = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("Error opening socket");
  }

  // argv[1] = server ip address
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "Error, no such host");
  }

  // Connect to the server
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portNum);
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    error("Connection failed");
  }

  // Launch the chat
  while (1) {
    // Write to the server
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
      error("Error on write");
    }

    // Read from the server
    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
      error("Error on read");
    }

    // Show incoming messages from the server
    printf("Server: %s", buffer);

    // Disconnect the client if "client-exit" is typed in chat by the server
    int i = strncmp("client-exit", buffer, 11);
    if (i == 0) {
      break;
    }
  }

  // Close the socket
  close(sockfd);
  return 0;
}
