/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

int MAX_PLAYER_COUNT;

int to_server, from_server;
char** white_cards;
char** submissions;
char* black_card;
int card_choice; // rename later if you want dumbo
int player_num;
int czar;

#include "pipe_networking.h"

void display_black_card() {
  printf("--------------------\n");
  printf("Black Card:\n\t%s\n", black_card);
}

void display_white_cards() {
  // display player's white cards
  printf("\nWhite Cards:\n");
  for (int i = 0; i < 7; i++){
    printf("\t%d: %s\n", i, white_cards[i]);
  }
  printf("\n");
}

void get_line(char * line) {
  fgets(line, 10, stdin);
  char * p = strchr(line, '\n');
  if (p) *p = 0;
}

void setup() {
  // black card memory allocation
  black_card = calloc(sizeof(char), 200);

  // player submissions memory allocation
  submissions = calloc(sizeof(char*), MAX_PLAYER_COUNT);

  from_server = client_handshake(&to_server);

  // gets player number
  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  printf("You are player #%s\n", response);
  player_num = atoi(response);

  // sets MAX_PLAYER_COUNT
  char* mpc_string = calloc(sizeof(char), 2);
  read(from_server, mpc_string, 2);
  MAX_PLAYER_COUNT = atoi(mpc_string);
  free(mpc_string);

  // gets white cards
  white_cards = calloc(sizeof(char*), 7);
  for (int i = 0; i < 7; i++){
    char* card = calloc(sizeof(char), 200);
    read(from_server, card, 200);
    white_cards[i] = card;
  }

  // display_white_cards();

  free(response);
}

/********************
  CZAR FUNCTIONS
********************/

void get_czar() {
  printf("start get_czar\n");
  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  printf("read %s\n", response);
  czar = atoi(response);
}

void get_player_submissions() {
  printf("start get_player_submissions\n");
  // might have a lot of memory leakage here...
  // clear previous submissions
  printf("YOU ARE THE CZAR\n");
  memset(submissions, 0, MAX_PLAYER_COUNT);
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* t = calloc(sizeof(char), 200);
    read(from_server, t, 200);
    printf("read %s\n", t);
    submissions[i] = t;
    printf("player #%d submitted |%s|\n", i, submissions[i]);
  }
}

void select_winner() {
  printf("start select_winner\n");
  int i;
  int winner;
  // loop until winner is selected
  // display player submissions
  printf("PLAYER SUBMISSIONS\n");
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != player_num) {
      printf("\t%d. %s\n", i, submissions[i]);
    }
  }

  // select winner
  printf("Winner: ");
  char* winner_string = calloc(sizeof(char), 10);
  get_line(winner_string);
  printf("read |%s| from user\n", winner_string);

  // send winner index to server
  write(to_server, winner_string, 2);
  printf("wrote |%s| to server\n", winner_string);

}

/********************
  NON-CZAR FUNCTIONS
********************/

void get_black_card() {
  printf("start get_black_card\n");
  read(from_server, black_card, 200);
  printf("read |%s|\n", black_card);
}

void submit_white_card() {
  printf("start submit_white_card\n");
  char * line = calloc(sizeof(char), 10);
  get_line(line);
  card_choice = atoi(line);
  printf("sending card #%d: %s\n", card_choice, white_cards[card_choice]);
  write(to_server, white_cards[card_choice], 200);
  printf("card sent\n");
}

void endgame_check() {
  printf("start endgame_check\n");
  // reading winning index, or -1 if nobody won
  char* winner = calloc(sizeof(char), 10);
  read(from_server, winner, 10);
  printf("read |%s|\n", winner);
  int winning_index = atoi(winner);
  free(winner);

  if (winning_index != -1) {
    // display who won and whatnot
    exit(EXIT_SUCCESS);
  }
  // else it must be -1, which means nobody won, so just keep going with the rest of the code
}

void get_white_card() {
  printf("start get_white_card\n");
  read(from_server, white_cards[card_choice], 200);
  printf("read |%s|\n", white_cards[card_choice]);
}

int main() {
  setup();
  // for (int n = 0; n < 2; n++){ // for testing purposes, runs 2 turns
  while (1) {
    get_czar();
    get_black_card();
    display_black_card();
    if (player_num == czar) {
      display_white_cards();
      get_player_submissions();
      select_winner();
    }
    else {
      display_white_cards();
      submit_white_card();
      get_white_card();
    }
    endgame_check();
  }
}
