#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// socket

// bind

// listen

// accept connection from client

// read the client write

// write to client

// close

// Error function
void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Port number not provided. Terminating program.\n");
    exit(1);
  }

  int sockfd, newsockfd, portnum, n;
  char buffer[255];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("Error opening socket");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portnum = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portnum);

  if (bind(sockfd, (struct sockaddr * ) &serv_addr, sizeof(serv_addr)) < 0) {
    error("Binding failed");
  }
}
