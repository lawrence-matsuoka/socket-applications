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

  // ensure user provides all three arguments (filename, server ip address, port
  // number) argv[0] = filename
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

  // Initialize the variables for the calculations
  int firstNumber, secondNumber, operation, answer;

S:
  // Receive the prompt and send the first number to the server
  bzero(buffer, 255);
  n = read(sockfd, buffer, 255);
  if (n < 0) {
    error("Error reading from socket");
  }
  printf("First number sent to server: %s\n", buffer);
  scanf("%d", &firstNumber);
  write(sockfd, &firstNumber, sizeof(int));

  // Receive the prompt and send the second number to the server
  bzero(buffer, 255);
  n = read(sockfd, buffer, 255);
  if (n < 0) {
    error("Error reading from socket");
  }
  printf("Second number sent to server: %s\n", buffer);
  scanf("%d", &secondNumber);
  write(sockfd, &secondNumber, sizeof(int));

  // Receive the prompt and send the operation to the server
  bzero(buffer, 255);
  n = read(sockfd, buffer, 255);
  if (n < 0) {
    error("Error reading from socket");
  }
  printf("Operation sent to server: %s\n", buffer);
  scanf("%d", &operation);
  write(sockfd, &operation, sizeof(int));

  // If the user enters 5, exit the program
  if (operation == 5) {
    goto Q;
  }

  // Read the answer to the calculation from the server
  read(sockfd, &answer, sizeof(int));
  printf("Server: The answer is %d\n", answer);

  // Continue the program if the user does not exit
  if (operation != 5) {
    goto S;
  }

  // Close the socket
Q:
  printf("You have successfully exited the program");
  close(sockfd);
  return 0;
}
