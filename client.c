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
  for (int card = 0; card < 7; card++){
    read(from_server, white_cards[card], 50);
  }

  printf("White Cards:\n");
  for (int card = 0; card < 7; card++){
    printf("%d: %s\n", card, white_cards[card]);
  }

  free(response);
  free(white_cards);
}
