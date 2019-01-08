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
    read(from_server, card, 200);
    printf("card = %s\n", card);
    white_cards[i] = card;
  }

  // display player's white cards
  printf("White Cards:\n");
  int i = 0;
  while(white_cards[i]){
    printf("%d: %s\n", i, white_cards[i]);
    i++;
  }

  free(response);
  free(white_cards);
}
