/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

int to_server, from_server;
char** white_cards;
char* black_card;

#include "pipe_networking.h"

int * card_selection; // rename later if you want dumbo

void display_white_cards(){
  // display player's white cards
  printf("\nWhite Cards:\n");
  for (int i = 0; i < 7; i++){
    printf("\t%d: %s\n", i, white_cards[i]);
  }
  printf("\n");
}

void setup() {
  // file descriptors for client-server communication
  int to_server;
  int from_server;

  // black card memory allocation
  black_card = calloc(sizeof(char), 200);

  from_server = client_handshake(&to_server);

  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  printf("You are player #%s\n", response);

  white_cards = calloc(sizeof(char*), 7);
  for (int i = 0; i < 7; i++){
    char* card = calloc(sizeof(char), 200);
    read(from_server, card, 200);
    white_cards[i] = card;
  }

  display_white_cards();

  free(response);
}

void get_black_card() {
  printf("Black Card:\n");
  char* black_card = calloc(sizeof(char), 200);
  read(from_server, black_card, 200);
  printf("\t%s\n", black_card);
}

void submit_white_card() {

}

int endgame_check() {

}

void get_white_card() {
  char* card = calloc(sizeof(char), 200);
  read(from_server, card, 200);
  white_cards[*card_selection] = card;
}

int main() {
  setup();
  // while (1) {
    get_black_card();
    display_white_cards();
    submit_white_card();
    // if (endgame_check()) {
    //   break;
    // }
    get_white_card();
  // }
}
