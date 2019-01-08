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
  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  printf("You are player #%s\n", response);

  char ** white_cards = calloc(sizeof(char), 200);
  for (int i = 0; i < 7; i++){
    char* card = calloc(sizeof(char), 200);
    read(from_server, card, 50);
    white_cards[i] = card;
  }

  printf("White Cards:\n");
  for (int i = 0; i < 7; i++){
    printf("%d: %s\n", i, white_cards[i]);
  }

  free(response);
  free(white_cards);
}
