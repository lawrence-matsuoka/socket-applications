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

  // Initialize variables for the calculator
  int firstNum, secondNum, answer, operation;

S:
  // Prompt the user for the first number
  n = write(newsockfd,
            "Enter the first number: ", strlen("Enter the first number: "));
  if (n < 0) {
    error("Error writing to socket");
  }
  // Read the reply from the client
  read(newsockfd, &firstNum, sizeof(int));
  printf("The first number entered by the client is: %d\n", firstNum);

  // Prompt the user for the second number
  n = write(newsockfd,
            "Enter the second number: ", strlen("Enter the second number: "));
  if (n < 0) {
    error("Error writing to socket");
  }
  // Read the reply from the client
  read(newsockfd, &secondNum, sizeof(int));
  printf("The second number entered by the client is: %d\n", secondNum);

  // Prompt the user for the operation
  char operationPrompt[] =
      "Enter a number for the operation:\n1. Addition\n2. Subtraction\n3. "
      "Multiplication\n4. Division\n5. Exit";
  n = write(newsockfd, operationPrompt, strlen(operationPrompt));
  // Read the reply from the client
  read(newsockfd, &operation, sizeof(int));
  printf("The operation entered by the client is: %d\n", operation);

  // Perform the chosen operation
  switch (operation) {
  case 1:
    answer = firstNum + secondNum;
    break;
  case 2:
    answer = firstNum - secondNum;
    break;
  case 3:
    answer = firstNum * secondNum;
    break;
  case 4:
    answer = firstNum / secondNum;
    break;
  case 5:
    goto Q;
    break;
  }

  // Send the answer to the client
  write(newsockfd, &answer, sizeof(int));
  if (operation != 5)
    goto S;

  // Close the socket
Q:
  close(newsockfd);
  close(sockfd);
  return 0;
}
