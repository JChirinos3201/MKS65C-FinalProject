/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

#include "pipe_networking.h"

void setup() {
  // file descriptors for client-server communication
  int to_server;
  int from_server;

  from_server = client_handshake(&to_server);

  char ** white_cards = calloc(sizeof(char*), 7);
  for (int i = 0; i < 7; i++){
    char* card = calloc(sizeof(char), 200);
    read(from_server, card, 200);
    white_cards[i] = card;
  }

  // display player's white cards
  printf("White Cards:\n");
  int i = 0;
  while(white_cards[i]){
    printf("%d: %s\n", i, white_cards[i]);
    i++;
  }

  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  printf("You are player #%s\n", response);

  free(response);
  free(white_cards);
}

int main() {
  setup();
  while (1) {
    get_black_card();
    submit_white_card();
    if (endgame_check()) {
      break
    }
    get_white_card();
  }
}
