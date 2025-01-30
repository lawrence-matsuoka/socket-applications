#include "error.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Terminate the program if the port number is not provided
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Error! Port number not provided.\n");
    exit(1);
  }

  // Initialize the socket
  int sockfd, newsockfd, portNum, n;
  char buffer[255];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  // IPv4
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("Error opening socket");
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portNum = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portNum);

  // Bind the socket
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    error("Binding failed");
  }

  // Listen for clients and allow a maximum of 5
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  if (newsockfd < 0) {
    error("Error on accept");
  }

  // Launch the chat
  while (1) {
    // Read from the client
    bzero(buffer, 255);
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
      error("Error on read");
    }
    printf("Client: %s\n", buffer);
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);

    // Write to the client
    n = write(newsockfd, buffer, strlen(buffer));
    if (n < 0) {
      error("Error on write");
    }

    // Disconnect the server when "server-exit" is sent in chat
    int i = strncmp("server-exit", buffer, 11);
    if (i == 0) {
      break;
    }
  }

  // Close the socket
  close(newsockfd);
  close(sockfd);
  return 0;
}
