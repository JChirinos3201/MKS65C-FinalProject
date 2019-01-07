/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

#include "pipe_networking.h"

int main() {
  // file descriptors for client-server communication
  int to_server;
  int from_server;

  from_server = client_handshake(&to_server);
  char* response = calloc(sizeof(char), 200);
  read(from_server, response, 200);
  printf("You are player #%s\n", response);
}
